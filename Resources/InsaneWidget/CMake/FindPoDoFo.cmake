# - Try to find PoDoFo
# Once done this will define
#  PODOFO_FOUND - System has PoDoFo
#  PODOFO_INCLUDE_DIRS - The PoDoFo include directories
#  PODOFO_LIBRARIES - The libraries needed to use PoDoFo
#  PODOFO_DEFINITIONS - Compiler switches required for using PoDoFo

find_package(PkgConfig)
pkg_check_modules(PC_PODOFO QUIET libpodofo.pc)
set(PODOFO_DEFINITIONS ${PC_PODOFO_CFLAGS_OTHER})

find_path(PODOFO_INCLUDE_DIR podofo/podofo.h
          HINTS ${PC_PODOFO_INCLUDEDIR} ${PC_PODOFO_INCLUDE_DIRS}
          PATH_SUFFIXES podofo )

find_library(PODOFO_LIBRARY NAMES libpodofo.so
             HINTS ${PC_PODOFO_LIBDIR} ${PC_PODOFO_LIBRARY_DIRS} )

set(PODOFO_LIBRARIES ${PODOFO_LIBRARY} )
set(PODOFO_INCLUDE_DIRS ${PODOFO_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PODOFO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(PoDoFo  DEFAULT_MSG
                                  PODOFO_LIBRARY PODOFO_INCLUDE_DIR)
mark_as_advanced(PODOFO_INCLUDE_DIR PODOFO_LIBRARY )
