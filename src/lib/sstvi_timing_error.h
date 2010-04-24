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



#ifndef SSTVI_TIMING_ERROR_H_
#define SSTVI_TIMING_ERROR_H_

#include "common_data_types.h"

#include <stdexcept>
#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace sstvi
{
  typedef boost::posix_time::time_duration time_duration;

  void check_timing( time_duration const & is, time_duration const & expected,
                     time_duration const & tolerance, int  const & state,
                     std::string const & msg );

  struct timing_error : public std::logic_error
  {
    time_duration is;
    time_duration expected;
    time_duration tolerance;
    int           state;

    std::string   msg;

    timing_error( time_duration const &_is, time_duration const &_expected,
                  time_duration const & _tolerance, int const & _state,
                  std::string const & _msg )
      : std::logic_error( "SSTV timing error" )
      , is( _is )
      , expected( _expected )
      , tolerance( _tolerance )
      , state( _state )
    {
      std::stringstream m;
      m << std::logic_error::what()
        << " " << _msg
        << ", is: " << is.total_nanoseconds() << " ns, expected: "
        << expected.total_nanoseconds() << " ns, tolerance: "
        << tolerance.total_nanoseconds() << " ns, state was "
        << state;
      msg = m.str();
    }

    virtual const char * what() const throw()
    {
      return msg.c_str();
    }

    ~timing_error() throw()
    {}
  }; //  class timing:error

}; // namespace sstvi

#endif /* SSTVI_TIMING_ERROR_H_ */
