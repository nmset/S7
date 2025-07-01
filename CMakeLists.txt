cmake_minimum_required(VERSION 3.5)

project(s7)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(wxWidgets COMPONENTS  base core CONFIG REQUIRED)

# xpm include.
add_definitions(-Wno-write-strings)

include_directories(${CMAKE_CURRENT_LIST_DIR}
                    Resources/InsaneWidget
                    Resources/Utilities
                    Resources/InsaneWidget/UI
                    Resources/StampWidget
                    Resources/StampWidget/UI
                    
                    Resources/UI/S7)

add_subdirectory(Resources/Utilities)
add_subdirectory(Resources/StampWidget)
add_subdirectory(Resources/InsaneWidget)

add_executable(s7 
              Resources/UI/S7/s7app.cpp 
              Resources/UI/S7/s7.cpp
              XS7.cpp)

install(TARGETS s7 RUNTIME DESTINATION bin)

target_link_libraries(s7 insanewidget
                    ${wxWidgets_LIBRARIES}
                    )
