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

#include "sstvi_morse_code_state_machine.h"


#include <iostream>
#include <stdexcept>
#include <sstream>


#define DEBUG 0


using boost::posix_time::seconds;
using boost::posix_time::milliseconds;
using boost::posix_time::microseconds;
using boost::posix_time::nanoseconds;



symbol_t
sstvi_morse_code_state_machine ::
  next_symbol( time_duration const & sym_start )
{
  symbol_t sym;
  states_t next = d_state;
  freq_t f;
  time_duration dt;

  time_duration const zero(0,0,0,0);

  // state machine for morse code

  try
  {
    switch( d_state )
    {
    case VIS_START:

      tic( sym_start, TIMING_TOTAL );

      sym = d_vis_code_sm->next_symbol( sym_start );
      next = VIS_SEND;

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
sstvi_morse_code_state_machine ::
  init()
{
  d_state = VIS_START;
  d_data_ptr = d_morse_seq.begin();
}


bool
sstvi_morse_code_state_machine ::
  set_data( std::vector< char > data )
{
  if( data.size() == 0 )
  {
    throw std::length_error( "Provide at least one character" );
  }

  d_data = data;
  return true;
}

bool
sstvi_morse_code_state_machine ::
  translate_data( std::vector< char > data )
{
  d_morse_seq.clear();

  for( uint_t i = 0; i < data.size(); ++i )
  {
    if( ! translate_letter( data[i] ) )
      return false;
    if( ! insert_gap() )
      return false;
  }

  d_morse_seq.push_back( m_symbol_t( false, seconds(0) ) );

  d_expected_duration = seconds( 0 );
  for( morse_seq_t::const_iterator it = d_morse_seq.begin();
       it != d_morse_seq.end(); ++it )
  {
    d_expected_duration += it->second();
  }
}

bool
sstvi_morse_code_state_machine ::
  translate_letter( char x )
{
  Morse * ptr = MorseTable;

  for( ; ptr->code != 0; ++ptr )
  {
    if( ptr->code != x )
      continue;

    for( uint_t i = 0; ptr->data[i] != NIL; ++i )
    {
      if( ptr->data[i] == DIH )
        d_morse_seq.push_back( d_info.dih );
      else if( ptr->data[i] == DAH )
        d_morse_seq.push_back( d_info.dah );
      else
        return false;

      d_morse_seq.push_back( d_info.intra_character_gap );
    }

    break;
  }

  return true;
}

bool
sstvi_morse_code_state_machine ::
  insert_gap( )
{
  // NOTE: we assume that a space ' ' doesn't insert any symbol
  // and that after every dih/dah, there is a intra character gap
  // hence, we always insert a short gap between letters, that effectively
  // gives a long gap between words
  // consider "a b" -> "dih . dah ." ... "" ... "dah . dih . dih . dih ."

  d_morse_seq.push_back( d_info.short_gap );
  return true;
}


time_duration
sstvi_morse_code_state_machine ::
  get_expected_duration()
{
  return d_expected_duration;
}


bool
sstvi_morse_code_state_machine ::
  ready()
{
  return d_data.size() > 0;
}


sstvi_morse_code_state_machine ::
sstvi_morse_code_state_machine( uint_t wpm )
  : d_wpm( wpm )
  , d_state( VIS_START )
  , d_data_ptr( 0 )
{
  d_info.base_freq              = 800;
  d_info.unit                   = milliseconds(1200) / wpm;

  d_info.t_dih                  = d_info.unit * 1;
  d_info.t_dah                  = d_info.unit * 3;
  d_info.t_intra_character_gap  = d_info.unit * 1;
  d_info.t_short_gap            = d_info.unit * 3;
  d_info.t_long_gap             = d_info.unit * 7;

  d_info.dih                    = m_symbol_t( true, d_info.t_dih );
  d_info.dah                    = m_symbol_t( true, d_info.t_dah );
  d_info.intra_character_gap    = m_symbol_t( false, d_info.t_intra_character_gap );
  d_info.short_gap              = m_symbol_t( false, d_info.t_short_gap );
  d_info.long_gap               = m_symbol_t( false, d_info.t_long_gap );
}


sstvi_morse_code_state_machine_sptr
sstvi_make_morse_code_state_machine( uint_t wpm )
{
  return sstvi_morse_code_state_machine::create( wpm );
}


sstvi_morse_code_state_machine_sptr
sstvi_morse_code_state_machine ::
create( uint_t wpm )
{
  try
  {
    sstvi_morse_code_state_machine_sptr tmp(
      new sstvi_morse_code_state_machine( wpm ) );

    return tmp;
  }
  catch ( ... )
  {
    std::cerr << "[ERROR] Caught exception at creation of "
              << "sstvi_morse_code_state_machine" << std::endl;
    throw;
  } // catch ( ... )
}



