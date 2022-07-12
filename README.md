# vision-module
Vision Module for Jetson TX2 image input

This is the vision module for path perception on the Crawler

The code is divided into sub-modules, each implementing a required feature to the Crawler's vision system

acquisition/    -  this is responsible for image acquisition, be it from a camera or from a dataset. <br />
communication/  -  deals with input/output communication: ROS and data streams. <br />
control/        -  implements control routines for keeping this module running. <br />
log/            -  implements logging for telemetry and develop purposes. <br /> 
model/          -  basic model classes. <br />
neuralnet/      -  network files to be used to run vision, perception, image segmentation etc. <br />
occupancy_grid/ -  code to calculate OG for a segmented frame. <br />
segmentation/   -  core segmentation algorithm, using neuralnet files to run image segmentation + image manipulation. <br />
tests           -  code for testing each submodule and the whole vision module. <br />
utils           -  utils for develop / testing. <br />
<br />
<br />
## How to compile. <br />
clone the repository, then in the rootdir do:.<br />
./build.sh. <br />

