cmake_minimum_required(VERSION 3.24)
project(ImageColorization)

set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(ImageColorization src/main.cpp src/Toolbar.cpp src/Toolbar.h src/Content.cpp src/Content.h src/ImagePainter.cpp src/ImagePainter.h src/Colorize.h src/Colorize.h src/ImageColorizer.h src/ImageColorizer.cpp)

find_package(Wt REQUIRED)
if(Wt_FOUND)
    message(STATUS "Wt libraries found")
    target_include_directories(ImageColorization PRIVATE ${WT_INCLUDE_DIRS})
    target_link_libraries(ImageColorization PRIVATE ${WT_LIBRARIES})
else()
    message(FATAL_ERROR "Wt libraries not found!")
endif()

find_package(OpenCV REQUIRED)
target_include_directories(ImageColorization PRIVATE ${OpenCV_INCLUDE_DIRS})

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(ImageColorization PRIVATE ${OpenCV_LIBS} Eigen3::Eigen)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()