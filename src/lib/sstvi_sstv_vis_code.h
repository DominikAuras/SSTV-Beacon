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



#ifndef SSTVI_SSTV_VIS_CODE_H_
#define SSTVI_SSTV_VIS_CODE_H_

#include "common_data_types.h"

#include <vector>


struct sstv_vis_code_info
{
  freq_t        leader_freq;
  time_duration leader;

  freq_t        break_freq;
  time_duration break_time;

  freq_t        start_bit_freq;
  time_duration start_bit;

  freq_t        stop_bit_freq;
  time_duration stop_bit;

  freq_t        bit0_freq;
  freq_t        bit1_freq;
  time_duration bit;

  std::vector< char > vis_code;

  time_duration duration;

  time_duration timing_tolerance;

};

#endif /* SSTVI_SSTV_VIS_CODE_H_ */
