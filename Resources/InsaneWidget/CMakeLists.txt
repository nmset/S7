cmake_minimum_required(VERSION 3.5)

project(InsaneWidget)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(wxWidgets COMPONENTS  base core CONFIG REQUIRED)
find_package(LibInsane REQUIRED)
find_package(PoDoFo REQUIRED)
find_package(Paper REQUIRED)


include_directories(${CMAKE_CURRENT_LIST_DIR}
                    ${CMAKE_CURRENT_LIST_DIR}/UI
                    ${CMAKE_CURRENT_LIST_DIR}/../Utilities)

add_library(insanewidget STATIC
            UI/InsaneWidget.cpp
            UI/ScannerWidget.cpp

            Common.h
            XInsaneWidget.cpp
            XScannerWidget.cpp
            InsaneWorker.cpp
            PixelToImageWriter.cpp
            PixelToPdfWriter.cpp)

target_link_libraries(insanewidget minutils
                    ${wxWidgets_LIBRARIES}
                    ${LIBINSANE_LIBRARIES}
                    ${PODOFO_LIBRARIES}
                    ${NETPBM_LIBRARIES}
                    ${PAPER_LIBRARIES})
