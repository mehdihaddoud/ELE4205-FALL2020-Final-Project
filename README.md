# ELE4205 FALL2020 : FINAL PROJECT 

# Project description

This is a TCP-IP application where an Odroid-c2 acts as a server that connects with a client (Virtual Machine under Linux Ubuntu). A camera and a button are connected to the Odroid-c2.
Each 30 ms, the server sends 3 types of flags :
- READY : ambient light is sufficiently bright, the server sends an image.
- IDOWN : ambient light is too low, the server does not send anything.
- PUSHB : ambient light is sufficiently bright AND the button is pushed, the server sends an image.

On the client side, each time an image is received, the client sends an acknowledgement to the server. When the acknowledgement is received by the server, the server sends the next image.

After detecting a PUSHB flag, the client saves the received image in the Virtual Machine. Furthermore, if the image contains a QR code, the data string contained in the QR code will be sent to the server.

When the server receives the data string, it plays the data string as Morse code by using a buffer.

# Steps to run the application

In order to launch this application, make sure to have a functional Odroid-c2 that communicates through SSH with a system under Linux.
You need to connect a camera compatible with Linux to the Odroid-c2. 
You need to connect a photoresistor with the AIN0 pin of the Odroid-c2 (pin 40).
You need to connect a switch button to GPIO #228 of the Odroid-c2 (pin 29).

Open a terminal in the Odroid-c2 and execute the following commands to open the GPIO pin : <br/>
1 - echo 228 > /sys/class/gpio/export <br/>
2 - echo in > /sys/class/gpio/gpio228/direction

In the same terminal, execute the following command to launch the server :

./Server

Open a terminal in the client side and execute the following command :

./Client

### Contribution
This application was designed by two students of Polytechnique Montréal :
- Mehdi Haddoud
- Michel Wilfred Essono

With the help of :
- The teaching assistant Étienne Beauchamp
- The course instructor Richard Gourdeau

### References
OpenCV tutorial C++ :
- https://www.opencv-srf.com/p/introduction.html


Derek Molloy's website :
- Beaglebone: Video Capture and Image Processing on Embedded Linux
  using OpenCV
- Streaming Video using RTP on the Beaglebone Black

TCP/IP :
- TCP/IP Sockets in C (Second Edition)

- https://stackoverflow.com/questions/20314524/c-opencv-image-sending-through-socket
