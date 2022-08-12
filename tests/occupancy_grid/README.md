# driveless <br/>
UFSC driveless car project repository <br/>

## Occupancy Grid Testing <br/>
This test case runs an image input/ image output test for the occupancy grid compute function.<br/>
The aim here is to debug it isolated from the rest of the vision module<br/>
<br/>
To compile it, just run<br/>
\$ make<br/>
A tst-og binary will be generated <br/>
<br/>
** This code can run outside of a Jetson ** <br/>
<br/>
If your PC is not setup for OpenCV2, cmake and g++, please use our develop container
<br/>

### Using a develop container<br/>

1. Clone the dev-env repo. <br/>
2. On the driveless-dev-cuda folder, run ./build.sh <br/>
3. Install the Visual Studio Code <br/>
4. In the  Visual Studio Code, install an extension called Remote Containers (identifier ms-vscode-remote.remote-containers) <br/>
5. Reopen the  driveless-dev-cuda folder using visual studio code and chose the option Reopen in container. <br/>
6. Use Open folder to locate the code you wish to work with <br/>
7. That's it, you're working with Vscode inside a container fully ready to run C/C++ code for this project. <br/>

