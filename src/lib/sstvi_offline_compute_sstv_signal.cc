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

#include "sstvi_offline_compute_sstv_signal.h"
#include "sstvi_martin_m1_state_machine.h"

#include <iostream>
#include <cmath>
#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/mem_fn.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

using boost::posix_time::seconds;
using boost::posix_time::milliseconds;
using boost::posix_time::microseconds;
using boost::posix_time::nanoseconds;


static const double pi = M_PI;


#define DEBUG 0

#define Ts d_Ts

sstvi_offline_compute_sstv_signal ::
sstvi_offline_compute_sstv_signal(
    unsigned int sample_rate_hz
  , boost::shared_ptr< sstvi_state_machine > sm )

  : d_sample_rate_hz( sample_rate_hz )
  , d_Ts( seconds(1) / sample_rate_hz )
  , d_done( false )
  , d_success( false )
  , d_base_freq( 1900. )

  , d_sm( sm ) //sstvi_make_martin_m1_state_machine() )
{
  // TODO check sample rate



//  std::cout << "fs: " << sample_rate_hz << " Hz" << std::endl;
//  std::cout << "Ts: " << d_Ts.total_nanoseconds() << " ns" << std::endl;
//  std::cout << "basefreq: " << d_base_freq << " Hz" << std::endl;

  assert( boost::posix_time::time_duration::num_fractional_digits() == 9 );

  d_expected_tx_time = d_sm->get_expected_duration();

//  std::cout << "expected tx time: " << d_expected_tx_time.total_nanoseconds()
//            << " ns" << std::endl;
  unsigned long long tt = d_expected_tx_time.total_nanoseconds();
  unsigned long long sample_rate = d_sample_rate_hz;
  unsigned long long samples = tt * sample_rate / 1000000000ULL + 1ULL;
//  std::cout << "expected samples " << samples
//            << std::endl;

  d_expected_sample_no = samples;
}




bool
sstvi_offline_compute_sstv_signal ::
  set_data( std::vector< unsigned char > data )
{
  d_sm->set_data( data );

  {
    boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
    d_done = false;
    d_success = false;
  }

  return true;
}


bool
sstvi_offline_compute_sstv_signal ::
  start()
{
  if( ! d_sm->ready() )
    throw std::runtime_error( "State machine not ready" );

  try
  {
    boost::thread computing_thread(
        boost::mem_fn(&sstvi_offline_compute_sstv_signal::work), this );
    d_computing_thread = computing_thread.move();
  }
  catch( boost::thread_resource_error const & ex )
  {
    std::cerr << "FAILED to start computing thread" << std::endl;
    return false;
  }
  catch( ... )
  {
    std::cerr << "ERROR unknown when creating computing thread" << std::endl;
    return false;
  }

  if( ! d_computing_thread.joinable() )
    return false;

  return true;
}



bool
sstvi_offline_compute_sstv_signal ::
  done() const
{
  boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );

  return d_done;
}



void
sstvi_offline_compute_sstv_signal ::
  join()
{
  if( d_computing_thread.joinable() )
    d_computing_thread.join();
}

bool
sstvi_offline_compute_sstv_signal ::
  timed_join()
{
  if( d_computing_thread.joinable() )
    return d_computing_thread.timed_join(
        boost::get_system_time() + milliseconds(500) );

  return true;
}

void
sstvi_offline_compute_sstv_signal ::
  cancel()
{
  d_computing_thread.interrupt();
}



void
sstvi_offline_compute_sstv_signal ::
  work()
{
  {
    boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
    d_done = false;
    d_success = false;
  }

  init_state_machine();
  d_signal.clear();
  d_signal.resize( d_expected_sample_no, 0.0 );

  time_duration const zero(0,0,0,0);
  time_duration t = zero;

  double phase_acc = 0.0;

  time_duration sym_start = t;
  symbol_t      sym       = d_sm->next_symbol( sym_start );
  time_duration sym_end   = sym_start + sym.second;

  long sample_ctr = 0;

  try
  {
    while( sym.second != zero )
    {
      time_duration const T = sym.second;
      freq_t const delta_freq = sym.first - d_base_freq;
      double const phase_shift =
        2 * pi * delta_freq * ( T.total_nanoseconds() * 1e-9 );

      while( t <= sym_end )
      {
        time_duration dt = t - sym_start;

        double const dphi = phase_shift * phase_pulse_fn( dt, T );
        double const mod_phase = phase_acc + dphi;
        double const carrier_phase =
          fmod( 2 * pi * ( t.total_nanoseconds() * 1e-9 ) * d_base_freq, 2 * pi );
        double const sig_phase = carrier_phase + mod_phase;

        double const sig_value = std::cos( sig_phase );

        add_signal_value( sig_value );


        ++sample_ctr;
        t = seconds(sample_ctr) / d_sample_rate_hz;

        boost::this_thread::interruption_point();
      }

      phase_acc += phase_shift * phase_pulse_fn( T, T );
      if( fabs(phase_acc) > 16 * pi )
      {
        phase_acc = fmod( phase_acc, 2 * pi );
      }

      sym_start = sym_end;
      sym = d_sm->next_symbol( sym_start );
      sym_end = sym_start + sym.second;

    }
  }
  catch( boost::thread_interrupted const & ex )
  {
    std::cout << "Interrupted computation" << std::endl;
    {
      boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
      d_done = true;
    }
    return;
  }
  catch( ... )
  {
    // indicate error state
    {
      boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
      d_done = true;
    }
    return;
  }

  std::cout << "t " << t.total_nanoseconds() << " ns" << std::endl;

  {
    boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
    d_success = true;
    d_done = true;
  }
}


void
sstvi_offline_compute_sstv_signal ::
  add_signal_value( double val )
{
  // TODO may fail if out of memory
  try
  {
    d_signal.at( d_signal_ctr++ ) = val;
  }
  catch( std::out_of_range const & ex )
  {
    std::cerr << "WARNING signal vector access out of range" << std::endl;
    std::cerr << " d_signal_ctr=" << d_signal_ctr
              << " signal.size=" << d_signal.size() << std::endl
              << ex.what() << std::endl;

    d_signal.push_back( 0.0 );
    d_signal.at( d_signal_ctr ) = val;
  }
}

double
sstvi_offline_compute_sstv_signal ::
  phase_pulse_fn( time_duration dt, time_duration T ) const
{
  // in [0,1]
  return (double)dt.total_nanoseconds() / T.total_nanoseconds();
}


void
sstvi_offline_compute_sstv_signal ::
  init_state_machine()
{
  d_sm->init();
  d_signal_ctr = 0;
}



std::vector< float >
sstvi_offline_compute_sstv_signal ::
  get_signal() const
{
  if( done() )
    return d_signal;
  else // TODO: throw
    return std::vector< float >();
}


bool
sstvi_offline_compute_sstv_signal ::
  successful()
{
  boost::lock_guard< boost::recursive_mutex > lock( d_data_mutex );
  return done() && d_success;
}


sstvi_offline_compute_sstv_signal_sptr
sstvi_make_offline_compute_sstv_signal( unsigned int sample_rate_hz,
  boost::shared_ptr< sstvi_state_machine > sm)
{
  return sstvi_offline_compute_sstv_signal::create( sample_rate_hz, sm );
}


sstvi_offline_compute_sstv_signal_sptr
sstvi_offline_compute_sstv_signal ::
create( unsigned int sample_rate_hz,
  boost::shared_ptr< sstvi_state_machine > sm )
{
  try
  {
    sstvi_offline_compute_sstv_signal_sptr tmp(
      new sstvi_offline_compute_sstv_signal( sample_rate_hz, sm ) );

    return tmp;
  }
  catch ( ... )
  {
    std::cerr << "[ERROR] Caught exception at creation of "
              << "sstvi_offline_compute_sstv_signal" << std::endl;
    throw;
  } // catch ( ... )
}



