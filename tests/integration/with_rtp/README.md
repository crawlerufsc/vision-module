# Integration Testing

## With RTP 

A test that runs the vision module connected with a RTP source camera. Can be used to receive image from simulators. Outputs the same way a normal vision module would, using the stream servers provided by the communication submodule.

## How to test it integrated with Carla:

Step 1. Initialize the carla simulator. If you dont have one in your system, please check one of the provided containers in our project.
<br/><br/>
Step 2. On the jetson side, go to the tests/integration/with_rtp folder and type $ make
<br/><br/>
Step 3. In your carla simulator container or in any system instalation or container that has the carla client installed, as well as the gstream and python packages set up, run the python script working as the simulator controller. This script will create a virtual webcam and stream it on port X
<br/><br/>
Step 4. Back to the jetson side, go to the tests/integration/with_rtp and run the bin vision-tst with the carla simulator container's (o equivalent) RTP-URI address, for example:
<br/><br/>
$ ./vision-tsts rtp://10.0.0.15:18000
<br/><br/>
Now the Jetson is connected to the simulator's virtual camera, on a created tesla car with a single virtual camera. The car is set to run by itself and stream the virtual camera's image to the port 18000
<br/><br/>
Step 5. To visualize the Jetson's code output in another machine, go to the tests/streaming folder and do a 
$ make
<br/><br/>
run an udp gstream sink server to receive the Jetson's output (udpstream_tst_server1.sh will create one on port 20000, udpstream_tst_server2.sh on port 20001, udpstream_tst_server3.sh on port 20002...) 
<br/><br/>
finally, tell Jetson that you'll be listening to one of its output ports in a port that runs the udp test stream in your machine. To do that, please compile the test_receive_streams.cpp with the command 
$ make<br/>
then you can execute build/stream_tst $IP (Jetsons output stream port) (gstreamer local stream port)<br/>
<br/><br/>
By default, the Jetson's code will open:<br/>
20000 for original input, on jetson's input resolution<br/>
20001 for the image segmentation<br/>
20002 for the image mask<br/>
20003 for the occupancy grid<br/>
<br/><br/>
you can also refer to the test_request_streams.sh<br/>

