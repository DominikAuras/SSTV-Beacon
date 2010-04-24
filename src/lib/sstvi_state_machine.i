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



class sstvi_state_machine;
typedef boost::shared_ptr<sstvi_state_machine> sstvi_state_machine_sptr;
%template(sstvi_state_machine_sptr) boost::shared_ptr<sstvi_state_machine>;
%ignore sstvi_state_machine;


class sstvi_state_machine
{
protected:
  sstvi_state_machine();

};
