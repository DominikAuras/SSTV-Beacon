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

#ifndef SSTVI_MARTIN_M1_SSTV_INFO_H_
#define SSTVI_MARTIN_M1_SSTV_INFO_H_

#include "common_data_types.h"


struct martin_m1_sstv_info
{
  freq_t        sync_pulse_freq;
  time_duration sync_pulse;

  freq_t        sync_porch_freq;
  time_duration sync_porch;

  freq_t        separator_pulse_freq;
  time_duration separator_pulse;

  freq_t        pixel_base_freq;
  freq_t        pixel_freq_hub;
  time_duration pixel;

  uint_t        width;
  uint_t        height;
  uint_t        color_components;
  uint_t        bytes_per_img;

  time_duration duration;

  time_duration scanline_duration;
  time_duration scanline_timing_tolerance;
  time_duration total_timing_tolerance;
  time_duration pixels_duration;
  time_duration pixels_timing_tolerance;
};


#endif /* SSTVI_MARTIN_M1_SSTV_INFO_H_ */
