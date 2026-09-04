# simple_cv_lib

A minimal OpenCV C++ library packaged as an `ament_cmake` package, built with `colcon`.

## Contents

- `simple_cv_lib::ImageProcessor` — grayscale conversion, Gaussian blur, and Canny edge
  detection wrapped around OpenCV.
- `demo` executable — runs the processor on a supplied image (or a synthetic one if none
  is given) and optionally writes the results to disk.

## Layout

```
simple_cv_lib/
├── CMakeLists.txt
├── package.xml
├── include/simple_cv_lib/image_processor.hpp
├── src/image_processor.cpp
└── examples/demo.cpp
```

## Dependencies

- `ament_cmake` (buildtool)
- OpenCV (system dependency, resolved via rosdep as `OpenCV` in `package.xml`)

## Build

This package expects to live under a colcon workspace's `src/` directory:

```bash
mkdir -p ~/ros2_ws/src
ln -s ~/simple_cv_lib ~/ros2_ws/src/simple_cv_lib   # or cp -r

cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
colcon build --packages-select simple_cv_lib
source install/setup.bash
```

## Run the demo

```bash
ros2 run simple_cv_lib demo

./install/simple_cv_lib/lib/simple_cv_lib/demo path/to/input.jpg /tmp/out
```

## Use the library from another ament_cmake package

```cmake
find_package(simple_cv_lib REQUIRED)
target_link_libraries(your_target PRIVATE simple_cv_lib::simple_cv_lib)
```

```xml
<depend>simple_cv_lib</depend>
```
