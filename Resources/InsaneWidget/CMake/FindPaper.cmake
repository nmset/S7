# - Try to find Paper
# Once done this will define
#  PAPER_FOUND - System has Paper
#  PAPER_INCLUDE_DIRS - The Paper include directories
#  PAPER_LIBRARIES - The libraries needed to use Paper
#  PAPER_DEFINITIONS - Compiler switches required for using Paper

find_path(PAPER_INCLUDE_DIR paper.h
          HINTS ${PC_PAPER_INCLUDEDIR} ${PC_PAPER_INCLUDE_DIRS}
          PATH_SUFFIXES paper )

find_library(PAPER_LIBRARY NAMES libpaper.so
             HINTS ${PC_PAPER_LIBDIR} ${PC_PAPER_LIBRARY_DIRS} )

set(PAPER_LIBRARIES ${PAPER_LIBRARY} )
set(PAPER_INCLUDE_DIRS ${PAPER_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PAPER_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Paper  DEFAULT_MSG
                                  PAPER_LIBRARY PAPER_INCLUDE_DIR)
mark_as_advanced(PAPER_INCLUDE_DIR PAPER_LIBRARY )
