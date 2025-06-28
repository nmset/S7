# 

# - Try to find LibInsane
# Once done this will define
#  LIBINSANE_FOUND - System has LibInsane
#  LIBINSANE_INCLUDE_DIRS - The LibInsane include directories
#  LIBINSANE_LIBRARIES - The libraries needed to use LibInsane
#  LIBINSANE_DEFINITIONS - Compiler switches required for using LibInsane

find_package(PkgConfig)
pkg_check_modules(PC_LIBINSANE QUIET libinsane.pc)
set(LIBINSANE_DEFINITIONS ${PC_LIBINSANE_CFLAGS_OTHER})

find_path(LIBINSANE_INCLUDE_DIR libinsane/sane.h
          HINTS ${PC_LIBINSANE_INCLUDEDIR} ${PC_LIBINSANE_INCLUDE_DIRS}
          PATH_SUFFIXES libinsane )

find_library(LIBINSANE_LIBRARY NAMES libinsane.so 
             HINTS ${PC_LIBINSANE_LIBDIR} ${PC_LIBINSANE_LIBRARY_DIRS} )

set(LIBINSANE_LIBRARIES ${LIBINSANE_LIBRARY})
set(LIBINSANE_INCLUDE_DIRS ${LIBINSANE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBINSANE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibInsane  DEFAULT_MSG
                                  LIBINSANE_LIBRARY LIBINSANE_INCLUDE_DIR)
mark_as_advanced(LIBINSANE_INCLUDE_DIR LIBINSANE_LIBRARY )
