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



#ifndef INCLUDED_SSTVI_STATE_MACHINE_H_
#define INCLUDED_SSTVI_STATE_MACHINE_H_


#include "common_data_types.h"

#include <vector>

#include <boost/shared_ptr.hpp>


class sstvi_state_machine;
typedef boost::shared_ptr< sstvi_state_machine > sstvi_state_machine_sptr;

/*!

 */
class sstvi_state_machine
{
public:

  virtual void init() = 0;

  virtual symbol_t next_symbol( time_duration const & ) = 0;

  virtual time_duration get_expected_duration() = 0;

  virtual bool ready() = 0;

  virtual bool set_data( std::vector< unsigned char > data )  = 0;
};

#endif /* INCLUDED_SSTVI_MARTIN_M1_STATE_MACHINE_H_ */
