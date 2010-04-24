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

#ifndef INCLUDED_SSTVI_morse_code_state_machine_H_
#define INCLUDED_SSTVI_morse_code_state_machine_H_

#include "sstvi_timing_error.h"
#include "sstvi_morse_code_info.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <utility>

class sstvi_morse_code_state_machine;
typedef boost::shared_ptr<sstvi_morse_code_state_machine>
  sstvi_morse_code_state_machine_sptr;

sstvi_morse_code_state_machine_sptr
sstvi_make_morse_code_state_machine( uint_t wpm );



/*!

 */
class sstvi_morse_code_state_machine
{
private:

  sstvi_morse_code_state_machine( uint_t wpm  );

  uint_t            d_wpm;
  morse_code_info   d_info;

  typedef std::list< m_symbol_t > morse_seq_t;

  typedef enum
  {
    START
  } states_t;

  states_t              d_state;
  morse_seq_t::const_iterator    d_data_ptr;

  morse_seq_t           d_morse_seq;


  time_duration         d_expected_duration;

  bool translate_data( std::vector< char > );
  bool translate_letter( char );
  bool insert_gap();

  typedef enum
  {
    TIMING_SCANLINE
  } timing_id;

  time_duration d_ticbuf[3];

  void tic( time_duration const & start, timing_id ind )
  {
    d_ticbuf[ind] = start;
  }

  time_duration toc( time_duration const stop, timing_id ind )
  {
    return stop - d_ticbuf[ind];
  }

public:

  virtual bool set_data( std::vector< char > data );

  virtual void init();

  virtual m_symbol_t next_symbol( time_duration const & );

  virtual time_duration get_expected_duration();

  virtual bool ready();



  static sstvi_morse_code_state_machine_sptr
  create( uint_t wpm );

  virtual ~sstvi_morse_code_state_machine() {};

};

#endif /* INCLUDED_SSTVI_morse_code_state_machine_H_ */
