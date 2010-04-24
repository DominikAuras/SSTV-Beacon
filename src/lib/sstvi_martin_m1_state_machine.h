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

#ifndef INCLUDED_SSTVI_MARTIN_M1_STATE_MACHINE_H_
#define INCLUDED_SSTVI_MARTIN_M1_STATE_MACHINE_H_

#include "sstvi_state_machine.h"
#include "sstvi_martin_m1_sstv_info.h"
#include "sstvi_timing_error.h"

#include <boost/shared_ptr.hpp>

#include <vector>

class sstvi_martin_m1_state_machine;
typedef boost::shared_ptr<sstvi_martin_m1_state_machine>
  sstvi_martin_m1_state_machine_sptr;

sstvi_martin_m1_state_machine_sptr
sstvi_make_martin_m1_state_machine(  );


// prototypes
class vis_code_state_machine;

/*!

 */
class sstvi_martin_m1_state_machine : public sstvi_state_machine
{
private:

  sstvi_martin_m1_state_machine(  );

  martin_m1_sstv_info d_mode;




  typedef enum
  {
    VIS_START,
    VIS_SEND,
    SYNC_PULSE,
    SYNC_PORCH,
    GREEN_SCAN,
    SYNC_SEP1,
    BLUE_SCAN,
    SYNC_SEP2,
    RED_SCAN,
    SYNC_SEP3,
    SSTV_EXIT
  } states_t;

  states_t      d_state;
  uint_t        d_data_ptr;
  uint_t        d_line_no;

  std::vector< unsigned char > d_data;



  static int const martin_m1_vis_code = 44;
  boost::shared_ptr< vis_code_state_machine > d_vis_code_sm;


  typedef enum
  {
    TIMING_SCANLINE,
    TIMING_PIXELS,
    TIMING_TOTAL
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

  virtual bool set_data( std::vector< unsigned char > data );

  virtual void init();

  virtual symbol_t next_symbol( time_duration const & );

  virtual time_duration get_expected_duration();

  virtual bool ready();

  sstvi_state_machine_sptr
  to_baseptr( sstvi_martin_m1_state_machine_sptr x )
  {
    return sstvi_state_machine_sptr( x );
  }


  static sstvi_martin_m1_state_machine_sptr
  create(  );

  virtual ~sstvi_martin_m1_state_machine() {};

};

#endif /* INCLUDED_SSTVI_MARTIN_M1_STATE_MACHINE_H_ */
