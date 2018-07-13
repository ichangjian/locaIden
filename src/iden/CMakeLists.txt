set ( ROOT ../.. )
include_directories ( ${OPENCV_INCLUDE_DIR} )
include_directories ( ${ROOT}/include )

set ( FILES_HEADER_IDEN ${ROOT}/include/locaIden.h)

set ( FILES_SAMPLE_IDEN ${ROOT}/src/iden/locaIden.cpp)

source_group( "include_LocaIden"       FILES ${FILES_HEADER_IDEN} )
source_group( "src_LocaIden"        FILES ${FILES_SAMPLE_IDEN} )

set ( SRC
    ${FILES_HEADER_IDEN}
    ${FILES_SAMPLE_IDEN}
)
ADD_LIBRARY( locaIden SHARED ${SRC} )
target_link_libraries ( locaIden ${OPENCV_LIB_DIR}/opencv_world300.lib ) 
SET_TARGET_PROPERTIES(locaIden  PROPERTIES    #表示生成的执行文件所在路径  
    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)