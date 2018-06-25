set ( ROOT .. )
include_directories ( ../include )
include_directories ( ${OPENCV_INCLUDE_DIR} )

set ( FILES_HEADER_IMGWALL ${ROOT}/include/locaIden.h)
set ( FILES_SAMPLE_IMGWALL ${ROOT}/test/testLocaIden.cpp)

source_group( "include_LocaIden"       FILES ${FILES_HEADER_IMGWALL} )
source_group( "sample_LocaIden"        FILES ${FILES_SAMPLE_IMGWALL} )

#######################################################################################
# project sample_hiar
#######################################################################################
set ( SRC
    ${FILES_HEADER_IMGWALL}
    ${FILES_SAMPLE_IMGWALL}
)
add_executable( testLocaIden ${SRC} )
target_link_libraries( testLocaIden locaIden)
target_link_libraries( testLocaIden ${OPENCV_LIB_DIR}/opencv_world300d.lib)
SET_TARGET_PROPERTIES(testLocaIden  PROPERTIES  RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
SET_TARGET_PROPERTIES(locaIden  PROPERTIES  RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#target_link_libraries( testLocaIden ${OPENCV_LIB_DIR}/opencv_world300d.lib)
#SET_TARGET_PROPERTIES(locaIden  PROPERTIES    #表示生成的执行文件所在路径  
#    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
#configure_file( ${OPENCV_LIB_DIR}/../bin/opencv_world300d.dll ${CMAKE_BINARY_DIR}/bin/Debug/ COPYONLY) 
#else()
#target_link_libraries( testLocaIden ${OPENCV_LIB_DIR}/opencv_world300.lib)
#SET_TARGET_PROPERTIES(locaIden  PROPERTIES    #表示生成的执行文件所在路径  
#    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
#configure_file( ${OPENCV_LIB_DIR}/../bin/opencv_world300.dll ${CMAKE_BINARY_DIR}/bin/Release/ COPYONLY) 
#endif ()
