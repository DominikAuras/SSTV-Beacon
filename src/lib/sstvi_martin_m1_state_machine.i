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

EXPORT_CPP_CLASS(sstvi,martin_m1_state_machine)
sstvi_martin_m1_state_machine_sptr
  sstvi_make_martin_m1_state_machine(  );

class sstvi_martin_m1_state_machine : public sstvi_state_machine
{
private:
  sstvi_martin_m1_state_machine(  );

public:
  ~sstvi_martin_m1_state_machine();

  sstvi_state_machine_sptr
  to_baseptr( sstvi_martin_m1_state_machine_sptr x );
};
