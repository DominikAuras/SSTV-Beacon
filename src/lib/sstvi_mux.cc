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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_block.h>
#include <gr_io_signature.h>
#include "sstvi_mux.h"

#include <iostream>
#include <algorithm>

#include <boost/thread/thread.hpp>

#define DEBUG 0


sstvi_mux ::
sstvi_mux(  )
  : gr_block(
      "mux",
      gr_make_io_signature(
         1,  -1,
        sizeof( float ) ),
      gr_make_io_signature(
        1, 1,
        sizeof( float ) ) )

  , d_default_port( 0 )
  , d_default_length( 0 )

  , d_cur_port( d_default_port )
  , d_cur_length( 0 )
  , d_cur( 0 )

  , d_next_port( 0 )
  , d_next_length( 0 )
  , d_next_set( false )

  , d_nports( 0 )
{
}

bool
sstvi_mux ::
  start()
{
  get_next_port_activation();

  return true;
}


bool
sstvi_mux ::
  check_topology( int ninputs, int noutputs )
{
  d_nports = ninputs;
  return true;
}


void
sstvi_mux ::
forecast(
  int             noutput_items,
  gr_vector_int & ninput_items_required )
{
  std::fill( ninput_items_required.begin(), ninput_items_required.end(), 0 );

//  ninput_items_required.at( d_cur_port ) = 1;
}


void
sstvi_mux ::
  get_next_port_activation()
{
  d_cur_port = d_default_port;
  d_cur_length = d_default_length;
  d_cur = 0;

  if( d_next_set )
  {
    d_cur_port = d_next_port;
    d_cur_length = d_next_length;
    d_next_set = false;
  }

//  std::cout << "next port activation: port " << d_cur_port << " len "
//            << d_cur_length << std::endl;

  assert( d_cur_length > 0 );
}


int
sstvi_mux ::
general_work(
  int                         noutput_items,
  gr_vector_int             & ninput_items,
  gr_vector_const_void_star & input_items,
  gr_vector_void_star       & output_items )
{
  float * out = static_cast< float * >( output_items[0] );

  for(;;)
  {
    boost::this_thread::interruption_point();

    float const * in =
      static_cast< float const * >( input_items[ d_cur_port ] );

    int const nleft = d_cur_length - d_cur;
    assert( nleft >= 0 );

    if( nleft == 0 )
    {
      get_next_port_activation();
      continue;
    }

    int const nin = std::min( nleft, ninput_items[ d_cur_port ] );
    int const ncopy = std::min( nin, noutput_items );

//    std::cout << "ncopy " << ncopy
//              << " nin " << nin
//              << " nleft " << nleft
//              << " nout " << noutput_items
//              << std::endl;

    if( ncopy == 0 )
      return 0;

    std::copy( in, in + ncopy, out );
    d_cur += ncopy;
    consume( d_cur_port, ncopy );

    if( nleft == ncopy )
    {
      get_next_port_activation();
    }

    return ncopy;

  } // to loop on activation

  return -1;
}


sstvi_mux_sptr
sstvi_make_mux(  )
{
  return sstvi_mux::create(  );
}


sstvi_mux_sptr
sstvi_mux ::
create(  )
{
  try
  {
    sstvi_mux_sptr tmp(
      new sstvi_mux(  ) );

    return tmp;
  }
  catch ( ... )
  {
    std::cerr << "[ERROR] Caught exception at creation of "
              << "sstvi_mux" << std::endl;
    throw;
  } // catch ( ... )
}



