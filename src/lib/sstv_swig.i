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



%include "gnuradio.i"			// the common stuff

%include exception.i


%{
#include <stdexcept>
#include <gr_block.h>
#include <gr_sync_block.h>
#include <gr_sync_interpolator.h>
#include <gr_sync_decimator.h>
#include <sstvi_offline_compute_sstv_signal.h>
#include <sstvi_mux.h>
#include <sstvi_martin_m1_state_machine.h>
#include <sstvi_state_machine.h>
%}

%exception {
    try
    {
      $action
    }
    catch( std::logic_error const & ex )
    {
      SWIG_exception(SWIG_SystemError, ex.what());
    }
    catch( std::runtime_error const & ex )
    {
      SWIG_exception(SWIG_RuntimeError,ex.what());
    }
    catch(...)
    {
      SWIG_exception(SWIG_RuntimeError,"Unknown exception");
    }
}

%define EXPORT_CPP_CLASS(PKG, BASE_NAME)
_EXPORT_CPP_CLASS_HELPER(PKG, PKG ## _ ## BASE_NAME, BASE_NAME)
%enddef

%define _EXPORT_CPP_CLASS_HELPER(PKG, NAME, BASE_NAME)
class NAME;
typedef boost::shared_ptr<NAME> NAME ## _sptr;
%template(NAME ## _sptr) boost::shared_ptr<NAME>;
%rename(BASE_NAME) PKG ## _make_ ## BASE_NAME;

%ignore NAME;
%enddef


%include "sstvi_offline_compute_sstv_signal.i"
%include "sstvi_mux.i"
%include "sstvi_state_machine.i"
%include "sstvi_martin_m1_state_machine.i"

