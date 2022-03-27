cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(istation)

include_directories( include )

set( CMAKE_CXX_FLAGS "-std=c++14 -Wall -fopenmp " )
set( CMAKE_CXX_FLAGS_DEBUG "-g -std=c++14 -Wall -fopenmp" )
message( "Linux settings chosen..." )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
find_package(Torch REQUIRED)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(ncnn_DIR ${CMAKE_SOURCE_DIR}/include/ncnn-20220216-ubuntu-2004/lib/cmake/ncnn)
find_package(ncnn REQUIRED)

file ( GLOB SOURCES "./src/*.cpp" "./include/*.h" )

add_executable( ${PROJECT_NAME} ${SOURCES} )

# target_link_libraries(istation ${TORCH_LIBRARIES} ${OpenCV_LIBS} ncnn)
target_link_libraries(istation ${OpenCV_LIBS} ncnn ${TORCH_LIBRARIES})
set_property(TARGET istation PROPERTY CXX_STANDARD 14)

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET istation
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:istation>)
endif (MSVC)