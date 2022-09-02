# Integration Testing

## Generation of an image dataset for testing/debugging

This test generates a set of original and segmented images from the input video, frame by frame.<br/>
<br/>
This generated dataset can be used to run the tests on tests/occupancy_grid<br/>


## How to generate it:

\$ make <br/>
<br/>

## How to run it: <br/>

\$ ./vision-tst [input video file] [number of frames to skip] [number of images to generate before stopping]
 <br/>

