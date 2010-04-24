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



#ifndef INCLUDED_SSTVI_OFFLINE_COMPUTE_SSTV_SIGNAL_H_
#define INCLUDED_SSTVI_OFFLINE_COMPUTE_SSTV_SIGNAL_H_

#include "common_data_types.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <vector>
#include <stdexcept>
#include <sstream>

// forward declarations
class sstvi_state_machine;


class sstvi_offline_compute_sstv_signal;
typedef boost::shared_ptr<sstvi_offline_compute_sstv_signal>
  sstvi_offline_compute_sstv_signal_sptr;

sstvi_offline_compute_sstv_signal_sptr
sstvi_make_offline_compute_sstv_signal( unsigned int sample_rate_hz,
  boost::shared_ptr< sstvi_state_machine > sm );




/*!

 */
class sstvi_offline_compute_sstv_signal
{
private:

  sstvi_offline_compute_sstv_signal( unsigned int sample_rate_hz,
    boost::shared_ptr< sstvi_state_machine > sm);


  unsigned int  d_sample_rate_hz;
  time_duration  d_Ts;

  std::vector< float > d_signal;
  uint_t               d_signal_ctr;

  bool d_done;
  bool d_success;


  boost::thread d_computing_thread;
  boost::recursive_mutex mutable d_data_mutex;

  void work();

  double phase_pulse_fn( time_duration dt, time_duration T ) const;



  freq_t d_base_freq;

  void init_state_machine();

  void add_signal_value( double val );


  time_duration d_expected_tx_time;
  uint_t        d_expected_sample_no;

  boost::shared_ptr< sstvi_state_machine > d_sm;


public:

  bool set_data( std::vector< unsigned char > data );

  std::vector< float > get_signal() const;

  bool start();

  bool done() const;

  bool successful();

  void join();
  bool timed_join();

  void cancel();


  static sstvi_offline_compute_sstv_signal_sptr
  create( unsigned int sample_rate_hz,
    boost::shared_ptr< sstvi_state_machine > sm );

  virtual ~sstvi_offline_compute_sstv_signal() {};


};

// TODO put in separate file
struct data_size_mismatch : public std::runtime_error
{
  unsigned int is;
  unsigned int expected;

  std::string   msg;

  data_size_mismatch( unsigned int _is, unsigned int _expected )
    : std::runtime_error( "SSTV data size mismatch" )
    , is( _is )
    , expected( _expected )
  {
    std::stringstream m;
    m << std::runtime_error::what()
      << ", is: " << is << ", expected: "
      << expected;
    msg = m.str();
  }

  virtual const char * what() const throw()
  {
    return msg.c_str();
  }

  ~data_size_mismatch() throw()
  {}
};

struct data_not_set : public std::runtime_error
{
  data_not_set( )
    : std::runtime_error( "SSTV data not set" )
  {
  }

  ~data_not_set() throw() {}
};

#endif /* INCLUDED_SSTVI_OFFLINE_COMPUTE_SSTV_SIGNAL_H_ */
