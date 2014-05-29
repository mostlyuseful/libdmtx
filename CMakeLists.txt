cmake_minimum_required (VERSION 2.6)
project (dmtx)

# find_package (Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})

# set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
# set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
# set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")

set(DMTX_SRCS
    dmtx.c
)

set(DMTX_HDRS
    dmtx.hpp
    dmtx.h
)

if(MSVC)
    add_definitions(-D_VISUALC_)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(dmtx SHARED ${DMTX_SRCS} ${DMTX_HDRS})
set_target_properties(dmtx PROPERTIES DEFINE_SYMBOL DMTX_BUILD_DLL)

install(TARGETS dmtx
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(FILES ${DMTX_HDRS} DESTINATION include)
