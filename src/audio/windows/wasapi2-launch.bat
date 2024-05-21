C:\gstreamer\1.0\msvc_x86_64\bin\gst-launch-1.0.exe -e ^
    d3d11screencapturesrc monitor-handle=65537 show-cursor=true ^
        ! video/x-raw, framerate=25/1 ^
        ! videoconvert ^
        ! videorate ^
        ! queue max-size-bytes=1073741824 max-size-time=10000000000 max-size-buffers=1000 ^
        ! openh264enc qp-min=23 qp-max=23 usage-type=camera complexity=low multi-thread=4 slice-mode=auto ^
        ! h264parse ^
        ! queue ^
        ! mux. ^
    wasapi2src low-latency=true device="\\\\\?\\SWD\#MMDEVAPI\#\{0.0.1.00000000\}.\{2dd319ce-b622-4b87-b789-cc74e2b0d457\}\#\{2eef81be-33fa-4800-9670-1cd474972c3f\}" ^
        ! audioconvert ^
        ! audiorate ^
        ! queue max-size-bytes=1000000 max-size-time=10000000000 max-size-buffers=1000 ^
        ! vorbisenc ^
        ! queue ^
        ! mux. ^
    matroskamux name=mux writing-app=vokoscreenNG_4.2.0-beta-01 ^
        ! filesink location="C:/Users/vk/Videos/vokoscreenNG-2024-05-15_04-24-51.mkv"