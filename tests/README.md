# driveless <br/>
UFSC driveless car project repository <br/>

## Tests <br/>
Here you find code for testing the vision module. Each sub-division of this module has (or should have) a test that is in a folder with the same name<br/>
For example, to test acquisition, please refer to tests/acquisition<br/>
<br/>
In any case, you should compile the code using the correspondent Makefile (command make). <br/>
For example, if you wish to compile the acquisition tests, you should enter the tests/acquisition folder and then <br/>
\$ make <br/>


<br/>
tests/integration are tests that run the entire codebase for the vision module, including all of its submodules such as acquisition, control, communication, log, occupancy_grid, segmentation etc.<br/>
<br/>

This folder is subdivided in 4 sub-tests:<br/>

subfolder | description
--- | ---
gen_img_dataset/ | reads a video file and generates a dataset of original frames and segmented frames that can be used to debug our algorithms.
with_dataset/ | a test that runs the vision module connected with a video as input (instead of a camera). Outputs the same way a normal vision module would, using the stream servers provided by the communication submodule.
with_images/ | a test that runs the vision module connected with a list of images as input (instead of a camera). Outputs the same way a normal vision module would, using the stream servers provided by the communication submodule.
with_rtp/ | a test that runs the vision module connected with a RTP source camera. Can be used to receive image from simulators. Outputs the same way a normal vision module would, using the stream servers provided by the communication submodule.

