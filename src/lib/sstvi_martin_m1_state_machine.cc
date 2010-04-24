//
// Copyright 2009 Dominik Auras
//
// This file is part of sstv-beacon
//
// SSTV-Beacon is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// SSTV-Beacon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Radio; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sstvi_martin_m1_state_machine.h"
#include "sstvi_vis_code_state_machine.h"

#include <iostream>
#include <stdexcept>
#include <sstream>


#define DEBUG 0


using boost::posix_time::seconds;
using boost::posix_time::milliseconds;
using boost::posix_time::microseconds;
using boost::posix_time::nanoseconds;



symbol_t
sstvi_martin_m1_state_machine ::
  next_symbol( time_duration const & sym_start )
{
  symbol_t sym;
  states_t next = d_state;
  freq_t f;
  time_duration dt;

  time_duration const zero(0,0,0,0);

  // state machine for Martin M1 mode

  try
  {
    switch( d_state )
    {
    case VIS_START:

      tic( sym_start, TIMING_TOTAL );

      sym = d_vis_code_sm->next_symbol( sym_start );
      next = VIS_SEND;

      break;

    case VIS_SEND:

      sym = d_vis_code_sm->next_symbol( sym_start );
      if( sym.second == zero )
      {
        tic( sym_start, TIMING_SCANLINE );

        sym = symbol_t( d_mode.sync_pulse_freq, d_mode.sync_pulse );
        next = SYNC_PORCH;
      }

      break;

    case SYNC_PULSE:

      tic( sym_start, TIMING_SCANLINE );

      sym = symbol_t( d_mode.sync_pulse_freq, d_mode.sync_pulse );
      next = SYNC_PORCH;

      break;

    case SYNC_PORCH:

      sym = symbol_t( d_mode.sync_porch_freq, d_mode.sync_porch );
      next = GREEN_SCAN;

      break;

    case GREEN_SCAN:

      if( ( d_data_ptr % d_mode.width ) == 0 ) // first column
        tic( sym_start, TIMING_PIXELS );

      f = d_data.at( d_data_ptr++ ) * d_mode.pixel_freq_hub +
          d_mode.pixel_base_freq;
      sym = symbol_t( f, d_mode.pixel );

      if( ( d_data_ptr % d_mode.width ) == 0 )
      {
        dt = toc( sym_start + d_mode.pixel, TIMING_PIXELS );
        sstvi::check_timing( dt, d_mode.pixels_duration,
          d_mode.pixels_timing_tolerance, d_state, "GREEN_PIXELS" );

        next = SYNC_SEP1;
      }
      break;

    case SYNC_SEP1:

      next = BLUE_SCAN;
      sym = symbol_t( d_mode.separator_pulse_freq, d_mode.separator_pulse );

      break;

    case BLUE_SCAN:

      if( ( d_data_ptr % d_mode.width ) == 0 ) // first column
        tic( sym_start, TIMING_PIXELS );

      f = d_data.at( d_data_ptr++ ) * d_mode.pixel_freq_hub +
          d_mode.pixel_base_freq;
      sym = symbol_t( f, d_mode.pixel );

      if( ( d_data_ptr % d_mode.width ) == 0 )
      {
        dt = toc( sym_start + d_mode.pixel, TIMING_PIXELS );
        sstvi::check_timing( dt, d_mode.pixels_duration,
          d_mode.pixels_timing_tolerance, d_state, "BLUE_PIXELS" );

        next = SYNC_SEP2;
      }
      break;

    case SYNC_SEP2:

      next = RED_SCAN;
      sym = symbol_t( d_mode.separator_pulse_freq, d_mode.separator_pulse );

      break;

    case RED_SCAN:

      if( ( d_data_ptr % d_mode.width ) == 0 ) // first column
        tic( sym_start, TIMING_PIXELS );

      f = d_data.at( d_data_ptr++ ) * d_mode.pixel_freq_hub +
          d_mode.pixel_base_freq;
      sym = symbol_t( f, d_mode.pixel );

      if( ( d_data_ptr % d_mode.width ) == 0 )
      {
        dt = toc( sym_start + d_mode.pixel, TIMING_PIXELS );
        sstvi::check_timing( dt, d_mode.pixels_duration,
          d_mode.pixels_timing_tolerance, d_state, "RED_PIXELS" );

        next = SYNC_SEP3;
      }
      break;


    case SYNC_SEP3:

      next = SYNC_PULSE;
      sym = symbol_t( d_mode.separator_pulse_freq, d_mode.separator_pulse );

      ++d_line_no;

      dt = toc( sym_start + d_mode.separator_pulse, TIMING_SCANLINE );
      sstvi::check_timing( dt, d_mode.scanline_duration,
        d_mode.scanline_timing_tolerance, d_state, "SCANLINE_TIMING");


      if( d_data_ptr == d_mode.bytes_per_img )
        next = SSTV_EXIT;

      break;


    case SSTV_EXIT:

      dt = toc( sym_start, TIMING_TOTAL );
      sstvi::check_timing( dt, get_expected_duration(),
        d_mode.total_timing_tolerance,
        d_state, "TOTAL" );

      sym = symbol_t( 0, seconds(0) );
      break;



    default:

      throw std::logic_error( "MARTIN M1 SM: Not existing state" );
      break;


    }
  }
  catch( std::out_of_range const & ex )
  {
    std::cerr << "ERROR out of range" << std::endl
              << "d_data_ptr=" << d_data_ptr << " "
              << "d_state=" << d_state << " "
              << "next=" << next << " "
              << "h w " << d_mode.height << ":" << d_mode.width << std::endl;
    throw;
  }
  catch( sstvi::timing_error const & ex )
  {
    std::cerr << "ERROR timing check failed, MARTIN M1" << std::endl
              << ex.what() << std::endl;
    throw;
  }

  d_state = next;
  return sym;
}


void
sstvi_martin_m1_state_machine ::
  init()
{
  d_state = VIS_START;
  d_data_ptr = 0;
  d_line_no = 0;

  d_vis_code_sm->init();
}


bool
sstvi_martin_m1_state_machine ::
  set_data( std::vector< unsigned char > data )
{
  if( data.size() != d_mode.bytes_per_img )
  {
    std::stringstream msg;
    msg << "Data size mismatch, is: " << data.size() << " "
        << "and expected: " << d_mode.bytes_per_img;
    throw std::length_error( msg.str() );
  }

  d_data = data;
  return true;
}


time_duration
sstvi_martin_m1_state_machine ::
  get_expected_duration()
{
  return d_mode.duration + d_vis_code_sm->get_expected_duration();
}


bool
sstvi_martin_m1_state_machine ::
  ready()
{
  return d_vis_code_sm->ready() && ( d_data.size() == d_mode.bytes_per_img );
}


sstvi_martin_m1_state_machine ::
sstvi_martin_m1_state_machine(  )
  : d_state( VIS_START )
  , d_data_ptr( 0 )
  , d_line_no( 0 )
  , d_vis_code_sm( new vis_code_state_machine( martin_m1_vis_code ) )
{

  d_mode.sync_pulse_freq        = 1200;
  d_mode.sync_pulse             = microseconds(4862);

  d_mode.sync_porch_freq        = 1500;
  d_mode.sync_porch             = microseconds(572);

  d_mode.separator_pulse_freq   = 1500;
  d_mode.separator_pulse        = microseconds(572);

  d_mode.pixel_base_freq        = 1500;
  d_mode.pixel_freq_hub         = 800. / 255;
  d_mode.pixel                  = nanoseconds(457600);


  d_mode.width                  = 320;
  d_mode.height                 = 256;
  d_mode.color_components       = 3;
  d_mode.bytes_per_img          = d_mode.width * d_mode.height *
                                  d_mode.color_components;

  d_mode.pixels_duration = d_mode.pixel * d_mode.width;
  d_mode.pixels_timing_tolerance = microseconds(10);

  d_mode.scanline_duration =
    d_mode.sync_pulse + d_mode.sync_porch +
    d_mode.separator_pulse * 3 +
    d_mode.pixels_duration * d_mode.color_components;

  d_mode.duration = d_mode.scanline_duration * d_mode.height;

  d_mode.scanline_timing_tolerance = microseconds(10);
  d_mode.total_timing_tolerance    = milliseconds(1);
}


sstvi_martin_m1_state_machine_sptr
sstvi_make_martin_m1_state_machine(  )
{
  return sstvi_martin_m1_state_machine::create(  );
}


sstvi_martin_m1_state_machine_sptr
sstvi_martin_m1_state_machine ::
create(  )
{
  try
  {
    sstvi_martin_m1_state_machine_sptr tmp(
      new sstvi_martin_m1_state_machine(  ) );

    return tmp;
  }
  catch ( ... )
  {
    std::cerr << "[ERROR] Caught exception at creation of "
              << "sstvi_martin_m1_state_machine" << std::endl;
    throw;
  } // catch ( ... )
}



