# simple_cv_lib

A minimal OpenCV based C++ library packaged as an ament_cmake package, built with colcon.

## Contents

The `simple_cv_lib::ImageProcessor` class wraps the following operations around OpenCV:

- `toGrayscale`: converts an image to single channel grayscale.
- `gaussianBlur`: applies OpenCV's Gaussian blur with a configurable kernel size.
- `convolutionFilter`: applies a user supplied kernel using OpenCV's `filter2D`.
- `convolutionFilterManual`: a from scratch implementation of 2D convolution that does not use `filter2D`.
- `fourierTransform`: computes the log magnitude Fourier spectrum of an image using OpenCV's `dft`.

A `demo` executable exercises these methods on a supplied image, or on a synthetic image if none is given, and can write the results to disk.

## Dependencies

- ROS 2 (tested with Jazzy)
- `colcon`
- `ament_cmake` (buildtool dependency, provided by the ROS 2 distribution)
- OpenCV (system dependency, resolved via rosdep as `OpenCV` in `package.xml`)
- A C++17 compiler

## Build

This package expects to live under a colcon workspace's `src` directory.

```bash
mkdir -p ~/ros2_ws/src
ln -s /path/to/this/repository ~/ros2_ws/src/simple_cv_lib

cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
colcon build --packages-select simple_cv_lib
source install/setup.bash
```

## Run the demo

```bash
ros2 run simple_cv_lib demo

ros2 run simple_cv_lib demo path/to/input.jpg /tmp/out
```

If no image path is given, the demo generates a synthetic input. If an output prefix is given as the second argument, the demo writes the blurred image and the Fourier spectrum image to disk using that prefix.

On startup, the demo asks whether to build a custom convolution kernel:

```
make custom kernel?
```

Entering `0` skips this step. Entering `1` prompts for the kernel size, then each kernel coefficient in row major order, then a divisor to scale the kernel. If a custom kernel is built, the demo also writes the results of `convolutionFilter` and `convolutionFilterManual` applied with that kernel.

## Running without ROS 2

The ROS 2 dependency comes entirely from the packaged build system (`ament_cmake` in `CMakeLists.txt` and `package.xml`). The library and demo source themselves only depend on OpenCV and the C++ standard library, so they can also be compiled directly, without CMake, colcon, or ROS 2:

```bash
g++ -std=c++17 -Iinclude $(pkg-config --cflags opencv4) -c src/image_processor.cpp -o image_processor.o
g++ -std=c++17 -Iinclude $(pkg-config --cflags opencv4) -c examples/demo.cpp -o demo.o
g++ image_processor.o demo.o $(pkg-config --libs opencv4) -o demo
./demo path/to/input.jpg /tmp/out
```

If `opencv4` is not the name of the OpenCV `pkg-config` module on your system, run `pkg-config --list-all | grep -i opencv` to find the correct name.

## Use the library from another ament_cmake package

```cmake
find_package(simple_cv_lib REQUIRED)
target_link_libraries(your_target PRIVATE simple_cv_lib::simple_cv_lib)
```

```xml
<depend>simple_cv_lib</depend>
```
