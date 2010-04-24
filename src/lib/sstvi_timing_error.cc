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



#include "sstvi_timing_error.h"

namespace sstvi
{

  void
  check_timing( time_duration const & is, time_duration const & expected,
                time_duration const & tolerance, int const & state,
                std::string const & msg )
  {
    time_duration diff = expected - is;
    if( diff.is_negative() )
      diff.invert_sign();

    if( diff <= tolerance )
      return;

    timing_error ex( is, expected, tolerance, state, msg );
    throw ex;
  }

}
