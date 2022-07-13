# vision-module
Vision Module for Jetson TX2 image input<br />
<br />
This is the vision module for path perception on the Crawler<br />
<br />
The code is divided into sub-modules, each implementing a required feature to the Crawler's vision system<br />
<br />
module | description
--- | ---
acquisition/ | this is responsible for image acquisition, be it from a camera or from a dataset.
communication/ | deals with input/output communication: ROS and data streams.
control/ | implements control routines for keeping this module running.
log/ | implements logging for telemetry and develop purposes.
model/ | basic model classes.
neuralnet/ | network files to be used to run vision, perception, image segmentation etc.
occupancy_grid/ | code to calculate OG for a segmented frame.
segmentation/ | core segmentation algorithm, using neuralnet files to run image segmentation + image manipulation.
tests | code for testing each submodule and the whole vision module.
utils | utils for develop / testing.

## How to compile.
<br />

### to compile
make

### to clean
make clean

### to compile and run tests (if in tests/)
make test