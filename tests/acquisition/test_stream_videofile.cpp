#include <memory>
#include <string>
#include <iostream>

#include <stdlib.h>
#include <string.h>

#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <../../acquisition/source_camera.h>
#include <../../acquisition/source_video_dataset.h>


int main(int argc, char **argv) {
    SourceCamera * source = new SourceVideoDatasetImpl("../imgs/20210603_162058.mp4");



    delete source;
}