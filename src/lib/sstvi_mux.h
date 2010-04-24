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

#ifndef INCLUDED_SSTVI_MUX_H_
#define INCLUDED_SSTVI_MUX_H_

#include "gr_block_fwd.h"// forward declarations

class sstvi_mux;
typedef boost::shared_ptr<sstvi_mux>
  sstvi_mux_sptr;

sstvi_mux_sptr
sstvi_make_mux(  );

/*!

 */
class sstvi_mux : public gr_block
{
private:

  sstvi_mux(  );

  int   d_default_port;
  int   d_default_length;

  int   d_cur_port;
  int   d_cur_length;
  int   d_cur;

  int   d_next_port;
  int   d_next_length;
  bool  d_next_set;

  int   d_nports;


  void
  forecast( int noutput_items,
    gr_vector_int &ninput_items_required );

  void get_next_port_activation();

public:

  void set_default_signal_length( int x ) { d_default_length = x; }

  bool activate_port( int port, int length )
  {
    if( ! ready() )
      return false;

    if( port < 0 || port >= d_nports )
      return false;

    d_next_port = port;
    d_next_length = length;
    d_next_set = true;

    return true;
  }

  bool ready() { return ! d_next_set; }

  bool using_default() { return d_cur_port == d_default_port; }


  bool start();

  virtual bool check_topology(int ninputs, int noutputs);


  static sstvi_mux_sptr
  create(  );

  virtual ~sstvi_mux() {};

  int
  general_work(
    int                         noutput_items,
    gr_vector_int             & ninput_items,
    gr_vector_const_void_star & input_items,
    gr_vector_void_star       & output_items );

};

#endif /* INCLUDED_SSTVI_MUX_H_ */
