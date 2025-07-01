cmake_minimum_required(VERSION 3.5)

project(StampWidget)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(wxWidgets COMPONENTS  base core CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}
                    ${CMAKE_CURRENT_LIST_DIR}/UI)
                    
add_library(stampwidget STATIC

            DefsStampWidget.h
            UI/StampWidget.cpp
            UI/StampWidgets.cpp
            XStampWidget.cpp
            XStampWidgets.cpp
            StampWorker.cpp)

target_link_libraries(stampwidget
                    ${wxWidgets_LIBRARIES})
