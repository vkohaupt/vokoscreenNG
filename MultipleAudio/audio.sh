#!/bin/bash

function_1()
{
rm *.dot
export GST_DEBUG_DUMP_DOT_DIR=.

audio1=alsa_output.pci-0000_00_1b.0.analog-stereo.monitor
audio2=alsa_input.usb-046d_0809_A6307261-02.analog-mono

# -v -e
# GST_DEBUG=2 
gst-launch-1.0 \
   audiomixer name=mix mix. \
   ! audioconvert ! audiorate ! lamemp3enc ! filesink location=voko.mp3 \
   pulsesrc device=$audio1 ! mix. \
   pulsesrc device=$audio2 ! mix. \
   

dot -Grankdir=BTRL -Tpdf *.PLAYING_PAUSED.dot > pipeline.pdf

exit
}


function_2()
{
   rm *.dot
   export GST_DEBUG_DUMP_DOT_DIR=.

   audio1=alsa_output.pci-0000_00_1b.0.analog-stereo.monitor
   audio2=alsa_input.usb-046d_0809_A6307261-02.analog-mono
   
   gst-launch-1.0 -e ximagesrc display-name=:0 use-damage=false show-pointer=true startx=0 starty=0 endx=1919 endy=1079 ! \
          capsfilter caps=video/x-raw,framerate=25/1 ! \
          videoconvert ! \
          videorate ! \
          x264enc qp-min=17 qp-max=17 speed-preset=superfast threads=4 ! \
          mux. audiomixer name=mix mix. \
          ! audioconvert ! audiorate ! lamemp3enc ! mux. \
          pulsesrc device=$audio1 ! queue ! mix. \
          pulsesrc device=$audio2 ! queue ! mix. \
          matroskamux name=mux ! filesink location=voko.mkv
          
dot -Grankdir=BTRL -Tpdf *.PLAYING_PAUSED.dot > pipeline.pdf
}


function_2
