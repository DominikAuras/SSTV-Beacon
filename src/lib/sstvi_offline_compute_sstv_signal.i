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



EXPORT_CPP_CLASS(sstvi,offline_compute_sstv_signal)
sstvi_offline_compute_sstv_signal_sptr
  sstvi_make_offline_compute_sstv_signal( unsigned int sample_rate_hz,
    boost::shared_ptr< sstvi_state_machine > sm);

class sstvi_offline_compute_sstv_signal
{
private:
  sstvi_offline_compute_sstv_signal( unsigned int sample_rate_hz,
    boost::shared_ptr< sstvi_state_machine > sm);

public:

  bool set_data( std::vector< unsigned char > data );

  std::vector< float > get_signal() const;

  bool start();

  bool done();

  bool successful();

  void join();
  bool timed_join();
  void cancel();

};


%inline %{
void join_unlocked(sstvi_offline_compute_sstv_signal_sptr r) throw (std::runtime_error)
{
    Py_BEGIN_ALLOW_THREADS;             // release global interpreter lock
    r->join();
    Py_END_ALLOW_THREADS;               // acquire global interpreter lock
}
%}

