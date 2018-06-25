cmake_minimum_required (VERSION 2.8.9)

project (LocaIden)


set ( OPENCV_INCLUDE_DIR           $ENV{OPENCV_ROOT}           CACHE PATH "OpenCV include directory" )
set ( OPENCV_LIB_DIR           $ENV{OPENCV_ROOT}           CACHE PATH "OpenCV lib directory" )

set ( USE_OPENCV ON)

if(USE_OPENCV)
    if(NOT ${linked_opencv})
        target_link_libraries( ${target} ${OpenCV_LIBS} )
    endif()
else()
    message("Warning: not using opencv")
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -frtti -fexceptions" )  # -fdump-class-hierarchy : dump RTTI etc.
  # set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread" )
endif ()

add_subdirectory ( src )
add_subdirectory ( test )
