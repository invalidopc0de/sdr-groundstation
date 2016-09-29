INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_RFM69 rfm69)

FIND_PATH(
    RFM69_INCLUDE_DIRS
    NAMES rfm69/api.h
    HINTS $ENV{RFM69_DIR}/include
        ${PC_RFM69_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    RFM69_LIBRARIES
    NAMES gnuradio-rfm69
    HINTS $ENV{RFM69_DIR}/lib
        ${PC_RFM69_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RFM69 DEFAULT_MSG RFM69_LIBRARIES RFM69_INCLUDE_DIRS)
MARK_AS_ADVANCED(RFM69_LIBRARIES RFM69_INCLUDE_DIRS)

