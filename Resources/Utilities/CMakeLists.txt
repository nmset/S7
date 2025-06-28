cmake_minimum_required(VERSION 3.5)

project(MinimalUtilities)

find_package(wxWidgets COMPONENTS  base core CONFIG REQUIRED)

add_library(minutils STATIC
            ConfigEditorPopup.cpp
            MiscTools.cpp
            PopupTransientWindow.cpp
            TimeredStatusBar.cpp
            XClientData.hpp
            )

target_link_libraries(minutils
                    ${wxWidgets_LIBRARIES}
                    )
