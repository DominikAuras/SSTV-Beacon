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



#include "sstvi_vis_code_state_machine.h"

#include <stdexcept>
#include <iostream>


using boost::posix_time::seconds;
using boost::posix_time::milliseconds;
using boost::posix_time::microseconds;
using boost::posix_time::nanoseconds;


void
vis_code_state_machine ::
  init()
{
  d_state = VIS_LEADER0;
  d_vis_ptr = 0;
}


symbol_t
vis_code_state_machine ::
  next_symbol( time_duration const & sym_start )
{
  symbol_t sym;
  states_t next = d_state;
  time_duration dt;

  // state machine for VIS code

  try
  {
    switch( d_state )
    {
    case VIS_LEADER0:

      tic( sym_start, TIMING_VIS );

      sym = symbol_t( d_vis.leader_freq, d_vis.leader );
      next = VIS_BREAK;

      break;

    case VIS_BREAK:

      sym = symbol_t( d_vis.break_freq, d_vis.break_time );
      next = VIS_LEADER1;

      break;

    case VIS_LEADER1:

      sym = symbol_t( d_vis.leader_freq, d_vis.leader );
      next = VIS_START;

      break;

    case VIS_START:

      sym = symbol_t( d_vis.start_bit_freq, d_vis.start_bit );
      next = VIS_BITS;

      break;

    case VIS_BITS:

      if( d_vis.vis_code.at( d_vis_ptr++ ) == 0 )
        sym = symbol_t( d_vis.bit0_freq, d_vis.bit );
      else
        sym = symbol_t( d_vis.bit1_freq, d_vis.bit );

      if( d_vis_ptr == 8 )
        next = VIS_STOP;

      break;

    case VIS_STOP:

      sym = symbol_t( d_vis.stop_bit_freq, d_vis.stop_bit );
      next = VIS_EXIT;

      break;


    case VIS_EXIT:

      dt = toc( sym_start, TIMING_VIS );
      sstvi::check_timing( dt, d_vis.duration, d_vis.timing_tolerance, d_state,
        "VIS_CODE" );

      sym = symbol_t( 0, seconds(0) );
      break;



    default:

      throw std::logic_error( "VIS SM: Not existing state" );
      // state machine not reset?
      break;


    }
  }
  catch( std::out_of_range const & ex )
  {
    std::cerr << "ERROR out of range" << std::endl
              << "d_state=" << d_state << " "
              << "next=" << next
              << std::endl;
    throw;
  }
  catch( sstvi::timing_error const & ex )
  {
    std::cerr << "ERROR timing check failed, VIS CODE" << std::endl
              << ex.what() << std::endl;
    throw;
  }

  // pass std::runtime_error up

  d_state = next;
  return sym;
}


time_duration
vis_code_state_machine ::
  get_expected_duration()
{
  return d_vis.duration;
}


vis_code_state_machine ::
vis_code_state_machine( int vis_code )
  : d_state( VIS_LEADER0 )
  , d_vis_ptr( 0 )
{
  // Leader1, Break, Leader2, Startbit, 8 bits, Stopbit

  d_vis.leader_freq             = 1900;
  d_vis.leader                  = milliseconds(300);

  d_vis.break_freq              = 1200;
  d_vis.break_time              = milliseconds(10);

  d_vis.start_bit_freq          = 1200;
  d_vis.start_bit               = milliseconds(30);

  d_vis.stop_bit_freq           = 1200;
  d_vis.stop_bit                = milliseconds(30);

  d_vis.bit0_freq               = 1300;
  d_vis.bit1_freq               = 1100;
  d_vis.bit                     = milliseconds(30);


  d_vis.duration                = d_vis.leader * 2 + d_vis.break_time +
                                  d_vis.start_bit + d_vis.stop_bit +
                                  d_vis.bit * 8;

  d_vis.timing_tolerance        = milliseconds(1);

  d_vis.vis_code.push_back( ( vis_code >> 0 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 1 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 2 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 3 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 4 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 5 ) & 1 );
  d_vis.vis_code.push_back( ( vis_code >> 6 ) & 1 );
  d_vis.vis_code.push_back( 0 );
  d_vis.vis_code[7] = d_vis.vis_code[0] ^ d_vis.vis_code[1] ^
                      d_vis.vis_code[2] ^ d_vis.vis_code[3] ^
                      d_vis.vis_code[4] ^ d_vis.vis_code[5] ^
                      d_vis.vis_code[6] ^ d_vis.vis_code[7]; // parity


}
