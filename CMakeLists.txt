cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)  # Use C++14

project(Clevy)

add_executable(Clevy WIN32
  src/App.cpp
  src/PreferencesDialog.cpp
  src/TrayIcon.cpp
)

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
