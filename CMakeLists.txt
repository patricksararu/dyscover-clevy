cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)  # Use C++14

project(Clevy)

add_executable(Clevy WIN32
  src/App.cpp
  src/App.h
  src/Audio.cpp
  src/Audio.h
  src/Core.cpp
  src/Core.h
  src/Keyboard.cpp
  src/Keyboard.h
  src/Keys.cpp
  src/Keys.h
  src/PreferencesDialog.cpp
  src/PreferencesDialog.h
  src/Queue.h
  src/SoundPlayer.cpp
  src/SoundPlayer.h
  src/Speech.cpp
  src/Speech.h
  src/TrayIcon.cpp
  src/TrayIcon.h
)

# Platform-specifics
if(WIN32)
  target_sources(Clevy PRIVATE src/KeyboardWindows.cpp src/KeyboardWindows.h)
elseif(UNIX)
  target_sources(Clevy PRIVATE src/KeyboardLinux.cpp src/KeyboardLinux.h)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

# Warnings
if(MSVC)
  target_compile_options(Clevy PRIVATE /W4 /WX)
else()
  target_compile_options(Clevy PRIVATE -Wall -Wextra -Werror -pedantic -Wno-error=missing-field-initializers)
endif()

# Use wxWidgets
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(Clevy ${wxWidgets_LIBRARIES})

# Use librstts
target_include_directories(Clevy PRIVATE lib/rstts/include/)

if(WIN32)
    # Determine file paths
    set(LIBRSTTS_LIB_FILE "${CMAKE_BINARY_DIR}/lib/librstts.lib")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(LIBRSTTS_MACHINE "x86")
        set(LIBRSTTS_DEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/lib/rstts/platforms/i686-pc-win32/librstts.dll.def")
        set(LIBRSTTS_DLL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/lib/rstts/platforms/i686-pc-win32/librstts-2.dll")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(LIBRSTTS_MACHINE "x64")
        set(LIBRSTTS_DEF_FILE "${CMAKE_CURRENT_SOURCE_DIR}/lib/rstts/platforms/x86_64-pc-win64/librstts.dll.def")
        set(LIBRSTTS_DLL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/lib/rstts/platforms/x86_64-pc-win64/librstts-2.dll")
    endif()

    # Generate .lib file
    add_custom_command(
        COMMENT "librstts: Generating .lib file from .def file ..."
        MAIN_DEPENDENCY ${LIBRSTTS_DEF_FILE}
        OUTPUT ${LIBRSTTS_LIB_FILE}
        COMMAND lib.exe /nologo /name:librstts-2 /machine:${LIBRSTTS_MACHINE} /def:${LIBRSTTS_DEF_FILE} /out:${LIBRSTTS_LIB_FILE}
    )

    # Link .exe with generated .lib
    target_sources(Clevy PRIVATE ${LIBRSTTS_LIB_FILE})
    target_link_libraries(Clevy ${LIBRSTTS_LIB_FILE})

    # Copy .dll file to the folder where .exe is
    add_custom_command(
        TARGET Clevy POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRSTTS_DLL_FILE} $<TARGET_FILE_DIR:Clevy>
    )
endif()

# Resources
if (APPLE)
  file(COPY res/ClevyIcon.icns DESTINATION ${CMAKE_BINARY_DIR})
else()
  file(COPY res/ClevyIcon.ico DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Sounds
file(GLOB SOUND_FILES "res/data/*.wav")
add_custom_command(
    TARGET Clevy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Clevy>/data
)
add_custom_command(
    TARGET Clevy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOUND_FILES} $<TARGET_FILE_DIR:Clevy>/data
)
