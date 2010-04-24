#
# Copyright 2004,2005,2006,2008,2009 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

include $(top_srcdir)/Makefile.common

AM_CPPFLAGS += $(BOOST_CPPFLAGS) -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

# C/C++ headers get installed in ${prefix}/include/gnuradio
grinclude_HEADERS =	

###################################
# SWIG Python interface and library

TOP_SWIG_IFILES = 		\
	sstv_swig.i

# Install so that they end up available as:
#   import gnuradio.sstv
# This ends up at:
#   ${prefix}/lib/python${python_version}/site-packages/gnuradio/sstv
sstv_swig_pythondir_category =	\
	gnuradio/sstv

# additional arguments to the SWIG command
#sstv_swig_swig_args =		\
#	-g

# additional sources for the SWIG-generated library
sstv_swig_la_swig_sources = \
  sstvi_offline_compute_sstv_signal.cc	\
  sstvi_timing_error.cc	\
  sstvi_martin_m1_state_machine.cc	\
  sstvi_vis_code_state_machine.cc \
  sstvi_mux.cc

# additional arguments to the SWIG command
sstv_swig_la_swig_args =		\
	$(SWIG_PYTHON_ARGS)

# additional libraries for linking with the SWIG-generated library
#sstv_swig_la_swig_libadd =		\
#	libm.dylib

# additional LD flags for linking the SWIG-generated library
#sstv_swig_la_swig_ldflags =	\
#	-L/opt/local/bin

# additional Python files to be installed along with the SWIG-generated one
sstv_swig_python =			\
	__init__.py  \
	modulator.py  \
	audio_interface.py	\
	image_loader.py

# additional SWIG files to be installed
sstv_swig_swiginclude_headers =	\
	sstvi_offline_compute_sstv_signal.i	\
	sstvi_mux.i \
	sstvi_state_machine.i \
	sstvi_martin_m1_state_machine.i

include $(top_srcdir)/Makefile.swig

# add some of the variables generated inside the Makefile.swig.gen
BUILT_SOURCES = $(swig_built_sources)

# Do not distribute the output of SWIG
no_dist_files = $(swig_built_sources)
