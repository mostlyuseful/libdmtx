cmake_minimum_required (VERSION 2.6)
project (dmtx)

# find_package (Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})

# set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
# set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
# set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")

set(DMTX_SOURCES
    dmtx.c
)

set(DMTX_HEADERS
    dmtx.hpp
    dmtx.h
)

set(DMTX_EXTRA_DEPENDENCY
    ${CMAKE_SOURCE_DIR}/dmtxencode.c
    ${CMAKE_SOURCE_DIR}/dmtxencodestream.c
    ${CMAKE_SOURCE_DIR}/dmtxencodescheme.c
    ${CMAKE_SOURCE_DIR}/dmtxencodeoptimize.c
    ${CMAKE_SOURCE_DIR}/dmtxencodeascii.c
    ${CMAKE_SOURCE_DIR}/dmtxencodec40textx12.c
    ${CMAKE_SOURCE_DIR}/dmtxencodeedifact.c
    ${CMAKE_SOURCE_DIR}/dmtxencodebase256.c
    ${CMAKE_SOURCE_DIR}/dmtxdecode.c
    ${CMAKE_SOURCE_DIR}/dmtxdecodescheme.c
    ${CMAKE_SOURCE_DIR}/dmtxmessage.c
    ${CMAKE_SOURCE_DIR}/dmtxregion.c
    ${CMAKE_SOURCE_DIR}/dmtxsymbol.c
    ${CMAKE_SOURCE_DIR}/dmtxplacemod.c
    ${CMAKE_SOURCE_DIR}/dmtxreedsol.c
    ${CMAKE_SOURCE_DIR}/dmtxscangrid.c
    ${CMAKE_SOURCE_DIR}/dmtximage.c
    ${CMAKE_SOURCE_DIR}/dmtxbytelist.c
    ${CMAKE_SOURCE_DIR}/dmtxtime.c
    ${CMAKE_SOURCE_DIR}/dmtxvector2.c
    ${CMAKE_SOURCE_DIR}/dmtxmatrix3.c
    ${CMAKE_SOURCE_DIR}/dmtxstatic.h
)

set_source_files_properties(dmtx.c PROPERTIES OBJECT_DEPENDS "${DMTX_EXTRA_DEPENDENCY}")

if(MSVC)
    add_definitions(-D_VISUALC_)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(dmtx SHARED ${DMTX_SOURCES} ${DMTX_HEADERS})
set_target_properties(dmtx PROPERTIES DEFINE_SYMBOL DMTX_BUILD_DLL)

install(TARGETS dmtx
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(FILES ${DMTX_HEADERS} DESTINATION include)
