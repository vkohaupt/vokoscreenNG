 
g++ -Wall -lpthread test1.c -o helloworld $(pkg-config --cflags --libs gstreamer-1.0 )
