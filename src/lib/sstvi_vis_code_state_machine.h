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



#ifndef SSTVI_VIS_CODE_STATE_MACHINE_H_
#define SSTVI_VIS_CODE_STATE_MACHINE_H_

#include "sstvi_state_machine.h"
#include "sstvi_sstv_vis_code.h"
#include "sstvi_timing_error.h"

#include <boost/shared_ptr.hpp>

#include <vector>

class vis_code_state_machine : public sstvi_state_machine
{
  typedef enum
  {
    VIS_LEADER0,
    VIS_BREAK,
    VIS_LEADER1,
    VIS_START,
    VIS_BITS,
    VIS_STOP,
    VIS_EXIT
  } states_t;

  states_t              d_state;
  sstv_vis_code_info    d_vis;
  int                   d_vis_ptr;

  typedef unsigned int uint_t;

  typedef enum
  {
    TIMING_VIS
  } timing_id;

  time_duration d_ticbuf[1];

  void tic( time_duration const & start, timing_id ind )
  {
    d_ticbuf[ind] = start;
  }
  time_duration toc( time_duration const stop, timing_id ind )
  {
    return stop - d_ticbuf[ind];
  }

public:

  // Martin M1: 44 (decimal)

  vis_code_state_machine( int vis_code );

  virtual void init();

  virtual symbol_t next_symbol( time_duration const & );

  virtual bool ready() { return true; }

  virtual bool set_data( std::vector< unsigned char > data ) { return true; };

  virtual time_duration get_expected_duration();
};


#endif /* SSTVI_VIS_CODE_STATE_MACHINE_H_ */
