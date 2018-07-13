set ( ROOT ../.. ) 
include_directories ( ${OPENCV_INCLUDE_DIR} )
include_directories ( ${ROOT}/include )

set ( FILES_HEADER_CAMERA ${ROOT}/include/usbCamera.h)

set ( FILES_SAMPLE_CAMERA ${ROOT}/src/usbCamera/usbCamera.cpp)

source_group( "include_usbCamera"       FILES ${FILES_HEADER_CAMERA} )
source_group( "src_usbCamera"        FILES ${FILES_SAMPLE_CAMERA} )

set ( SRC
    ${FILES_HEADER_CAMERA}
    ${FILES_SAMPLE_CAMERA}
)
ADD_LIBRARY( usbCamera SHARED ${SRC} )
target_link_libraries ( usbCamera ${OPENCV_LIB_DIR}/opencv_world300.lib ) 
SET_TARGET_PROPERTIES(usbCamera  PROPERTIES    #表示生成的执行文件所在路径  
    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)