#!/usr/bin/env python

#
# Copyright 2009 Dominik Auras
#
# This file is part of sstv-beacon
#
# SSTV-Beacon is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# SSTV-Beacon is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

try:
  from image_loader import image_loader
  from modulator import offline_sstv_modulator
  from audio_interface import audio_server
except:
  try:
    from gnuradio.sstv.image_loader import image_loader
    from gnuradio.sstv.modulator import offline_sstv_modulator
    from gnuradio.sstv.audio_interface import audio_server
  except:
    raise SystemExit, "SSTV library not found, FAIL"


try:
  from ditndah import MorseGen
  import ditndah
except:
  raise SystemExit, "Could not load python library ditndah"

try:
  from numpy import concatenate, cos, arange
  from math import pi
  import numpy

  from PIL import ImageDraw, ImageFont

  import time
  import os

  import gnuradio.gr.gr_threading as _threading
except:
  raise SystemExit, "Was not able to load supporting libraries, CHECK!"

class beacon:
  def __init__( self ):

    # TODO catch exceptions

    # TODO read from config
    self.sample_rate = int(48000)
    self.imgsize = (320,256)
    self.colorcomp = 3

    morse_text = "MORSETEXT "

    # Choosing SYNC frequency, to ease manual frequency synchronization
    # before SSTV transmission
    self.morse_freq = 1500 #Hz

    self.pause = 2 # seconds

    self.beacon_time = 5 # min



    # beacon signal, continuous wave
    default_signal = cos( arange( 0, self.sample_rate ) * 2. * pi * self.morse_freq / self.sample_rate )

    self.audio = audio_server( self.sample_rate, default_signal )
    self.id_sstv_src = self.audio.add_source()


    m = MorseGen( pitch=self.morse_freq,
                  sampleRate=self.sample_rate,
                  wpm=13,
                  fwpm=13,
                  format=ditndah.FORMAT_S16LE,
                  numChannels=1
                 )

    # Generate Morse code signal, append and prepend pauses

    self.morse_signal = numpy.fromstring( m.textToAudioData( morse_text ),
                                          dtype=numpy.int16 )
    self.morse_signal = numpy.array( self.morse_signal, dtype=numpy.float64 )
    self.morse_signal = self.morse_signal / numpy.max( self.morse_signal )
    self.morse_signal = concatenate( [ [0.]*(self.sample_rate*self.pause),
                                       self.morse_signal,
                                       [0.]*(self.sample_rate) ] )

    self.morse_time = len( self.morse_signal ) / float(self.sample_rate)

    print "Morse code time: ",self.morse_time

    self.id_morse_src = self.audio.add_source( self.morse_signal )


    self.imgloader = image_loader( self.imgsize )

    # TODO different font?
    try:
      self.myfont = ImageFont.truetype( "OCR-A.ttf", 12 )
      print "Using OCR-A truetype font"
    except:
      try:
        self.myfont = ImageFont.truetype( "/usr/local/share/sstv/OCR-A.ttf", 12 )
        print "Using OCR-A truetype font"
      except:
        self.myfont = ImageFont.load_default()
        print "Using default font"

    # TODO catch errors
    try:
      self.modulator = offline_sstv_modulator( self.sample_rate )
    except:
      raise SystemExit, "Could not create SSTV modulator"
      # TODO: guarantee beacon, retry instantiation later

    beacon_interval = 60 # seconds
    self.minute_timer = timer( beacon_interval )
    assert( self.morse_time < beacon_interval )

    try:
      self.audio.start()

    except:
      self.audio.stop()

    print "returned"

  def run(self):
    last_state = -1
    beacon_rep = 0

    self.state = states.start_image_loader

    try:
      while True:
        if last_state != self.state:
          print "State: ", self.state
          last_state = self.state

        if self.state == states.start_image_loader:

          # TODO flexible, catch errors
          self.imgloader.start_get_image()

          self.state = states.wait_on_image_loader


        elif self.state == states.wait_on_image_loader:

          # TODO: timeout ??
          if not self.imgloader.done():
            time.sleep(0.1)
          else:
            self.state = states.get_image

        elif self.state == states.get_image:

          print "get image"
          self.img = self.imgloader.get_image()
          print "get image returned"
          if self.img is None:
            print "FAILED"
            self.state = states.pre_morse_activation # FAILED, skip this time

          self.state = states.prepare_image

        elif self.state == states.prepare_image:

          # TODO on fail, guarantee morse+beacon operation
          self.draw_text( self.img )
          data = self.convert_img_to_raw( self.img, self.imgsize )

          if not self.modulator.set_data(data):
            self.state = states.pre_morse_activation
          else:
            self.modulator.start()
            self.state = states.wait_on_modulator

        elif self.state == states.wait_on_modulator:

          # wait on modulator
          # TODO: timeout?
          self.modulator.wait()
          self.state = states.wait_on_sstv_queue

        elif self.state == states.wait_on_sstv_queue:

          # wait on src
          if not self.audio.src_ready( self.id_sstv_src ):
            time.sleep( 0.1 )
          else:
            self.state = states.sstv_enqueue

        elif self.state == states.sstv_enqueue:

          if not self.modulator.successful():
            self.state = states.pre_morse_activation

          signal = self.modulator.get_signal()
          signal = list( concatenate( [ [0.]*(self.sample_rate*self.pause), signal ] ) )

          if not self.audio.enqueue_signal( self.id_sstv_src, signal ):
            self.state = states.pre_morse_activation

          self.state = states.pre_sstv_activation

        elif self.state == states.pre_sstv_activation:

          # wait on audio scheduler
          if not self.audio.scheduler_ready():
            time.sleep( 0.1 )
          else:
            self.state = states.sstv_activation

        elif self.state == states.sstv_activation:

          assert( self.audio.schedule_src( self.id_sstv_src ) )
          self.state = states.pre_morse_activation
          beacon_rep = 0

        elif self.state == states.pre_morse_activation:

          # wait on audio scheduler
          if not self.audio.scheduler_ready():
            time.sleep( 0.1 )
          else:
            self.state = states.morse_activation

        elif self.state == states.morse_activation:

          # TODO morse activation is critical to beacon operation
          assert( self.audio.schedule_src( self.id_morse_src ) )
          self.state = states.post_morse_activation

        elif self.state == states.post_morse_activation:

          if not self.audio.scheduler_ready():
            time.sleep( 0.1 )
          else:
            self.state = states.start_minute_timer

        elif self.state == states.start_minute_timer:

          self.minute_timer.start()
          self.state = states.wait_on_minute_timer

        elif self.state == states.wait_on_minute_timer:

          if not self.minute_timer.timed_out():
            time.sleep( 0.1 )
          else:
            beacon_rep += 1
            if beacon_rep < self.beacon_time:
              self.state = states.pre_morse_activation
            else:
              self.state = states.start_image_loader



    except Exception, ex:

      print repr(ex)

    except KeyboardInterrupt:

      pass

    finally:

      self.audio.stop()
      if not self.minute_timer.timed_out():
        self.minute_timer.stop()




  def wait(self):
    self.audio.wait()


  def draw_text( self, img ):
    #TODO customize text

    draw = ImageDraw.Draw(img)

    up = uptime()

    #text = "DB0LTG  JO31TB  " + up +  " uptime"
    text = "TEST DL5KBG via DB0LTG"
    textsize = draw.textsize( text, font=self.myfont )
    print "textsize ", textsize
    draw.rectangle( (0,0,320,16), fill=(255,255,255) )
    draw.text( (10,0), text, font=self.myfont, fill=(0,0,0) )



  def convert_img_to_raw( self, img, imgsize ):
    #check that we have three color components
    # check that order is RGB
    data = []
    bands = img.getbands()

    color_ind = [0,1,2]
    for i in range(2):
      if bands[i] == 'g' or bands[i] == 'G':
        color_ind[0] = i
      if bands[i] == 'b' or bands[i] == 'B':
        color_ind[1] = i
      if bands[i] == 'r' or bands[i] == 'R':
        color_ind[2] = i

    for y in range( imgsize[1] ):
      for c in range( 3 ): #color
        for x in range( imgsize[0] ):
          pp = img.getpixel( (x,y) )
          data.append( int(pp[color_ind[c]]) )
    assert( len(data) == (imgsize[0]*imgsize[1]*3) )
    return list(data)




def uptime():
     try:
         f = open( "/proc/uptime" )
         contents = f.read().split()
         f.close()
     except:
        return "uptime FAILED"

     total_seconds = float(contents[0])

     MINUTE  = 60
     HOUR    = MINUTE * 60
     DAY     = HOUR * 24

     days    = int( total_seconds / DAY )
     hours   = int( ( total_seconds % DAY ) / HOUR )
     minutes = int( ( total_seconds % HOUR ) / MINUTE )
     seconds = int( total_seconds % MINUTE )

     string = ""
     if days> 0:
         string += str(days) +  "d "

     string += "%02d:%02d:%02d" % ( hours, minutes, seconds )

     return string;



class timer:
  def __init__(self, timeout):
    self.event = _threading.Event()
    self.timer = None
    self.timeout = timeout

  def start(self):
    self.event.clear()
    self.timer = _threading.Timer( self.timeout, self.run )
    self.timer.start()

  def run(self):
    self.event.set()
    print "Timed out"

  def stop(self):
    if self.timer is not None:
      self.timer.cancel()

  def timed_out(self):
    return self.event.isSet()


class states:
  start_image_loader = 0
  wait_on_image_loader = 1
  get_image = 2
  prepare_image = 3
  wait_on_modulator = 4
  wait_on_sstv_queue = 5
  sstv_enqueue = 6
  pre_sstv_activation = 7
  sstv_activation = 8
  pre_morse_activation = 9
  morse_activation = 10
  post_morse_activation = 11
  start_minute_timer = 12
  wait_on_minute_timer = 13


if __name__ == '__main__':
    t = beacon()
    t.run()
    raise SystemExit, "Returned with no error, must raise to respawn"
    # when we return, upstart will respawn our process

