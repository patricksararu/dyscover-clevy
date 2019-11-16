cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)  # Use C++14

project(Clevy)

add_executable(Clevy WIN32
  src/App.cpp
  src/App.h
  src/PreferencesDialog.cpp
  src/PreferencesDialog.h
  src/TrayIcon.cpp
  src/TrayIcon.h
)

# Warnings
if(MSVC)
  target_compile_options(Clevy PRIVATE /W4 /WX)
else()
  target_compile_options(Clevy PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# Use wxWidgets
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(Clevy ${wxWidgets_LIBRARIES})

# Resources
if (APPLE)
  file(COPY res/ClevyIcon.icns DESTINATION ${CMAKE_BINARY_DIR})
else()
  file(COPY res/ClevyIcon.ico DESTINATION ${CMAKE_BINARY_DIR})
endif()
