# Cpp-Computer-Vision-Tutorial
Computer vision tutorial for installing and using PyTorch, OpenCV and NCNN in C++.

# Installation
## OpenCV
[OpenCV](https://opencv.org/) (Open Source Computer Vision Library) is a library of programming functions mainly aimed at real-time computer vision. Originally developed by Intel, it was later supported by Willow Garage then Itseez (which was later acquired by Intel)

First step is the install OpenCV to your local environment which is based from [link](https://vitux.com/opencv_ubuntu/) and works well in Ubuntu 20.04 (Do not guarantee for other ubuntu versions) (Do not need to do this installation in your project folder).
```
sudo apt-get update
sudo apt-get upgrade

sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
                libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
                libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
                gfortran openexr libatlas-base-dev python3-dev python3-numpy \
                libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
                libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev

mkdir ~/opencv_build && cd ~/opencv_build
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

cd ~/opencv_build/opencv
mkdir -p build && cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D INSTALL_C_EXAMPLES=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
-D BUILD_EXAMPLES=ON ..

make -j8

sudo make install

pkg-config --modversion opencv4

# Expected Output
4.4.0
```
Then add commands that given below will be include OpenCV library in your codebase.
```
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(example-app ${OpenCV_LIBS})
```

## PyTorch
[PyTorch](https://pytorch.org/) is an open source machine learning (ML) framework based on the Python programming language and the Torch library. It is one of the preferred platforms for deep learning research. The framework is built to speed up the process between research prototyping and deployment.
Can reach official installing C++ distributions of PyTorch from [link](https://pytorch.org/cppdocs/installing.html). Our installation based on this documentation.

```
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip

mv libtorch include/
rm -r libtorch-shared-with-deps-latest.zip
```

Then properties of library will be added to `CMakeLists.txt` manually. `example-app` will be your project name. Example of only using libtorch in C++, `CMakeLists.txt` as shown in below.

```
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example-app)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(example-app example-app.cpp)
target_link_libraries(example-app "${TORCH_LIBRARIES}")
set_property(TARGET example-app PROPERTY CXX_STANDARD 14)

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET example-app
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:example-app>)
endif (MSVC)
```

## ncnn
[ncnn](https://github.com/Tencent/ncnn) is a high-performance neural network inference computing framework optimized for mobile platforms. ncnn is deeply considerate about deployment and uses on mobile phones from the beginning of design.

ncnn installation based on its own installation [guide](https://github.com/Tencent/ncnn/releases). You should find best fit for your environment. In our repository, we are going to work with Ubuntu 20.04 so will download dependencies with respect to this direction. Imagine downloaded `ncnn-20220216-ubuntu-2004.zip` from releases. (Replace ncnn folder name if necessary)
```
unzip ncnn-20220216-ubuntu-2004.zip
mv ncnn-20220216-ubuntu-2004 include
rm -r ncnn-20220216-ubuntu-2004.zip
```
Then, add required to `CMakeLists.txt`.
```
set(ncnn_DIR ${CMAKE_SOURCE_DIR}/include/ncnn-20220216-ubuntu-2004/lib/cmake/ncnn)
find_package(ncnn REQUIRED)

target_link_libraries(example-app ${TORCH_LIBRARIES} ${OpenCV_LIBS} ncnn)
```