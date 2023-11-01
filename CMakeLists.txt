cmake_minimum_required(VERSION 3.15)

# vcpkg
# TODO: On Windows and macOS, use vcpkg by default
# First try to find VCPKG_ROOT env variable
message(STATUS "vcpkg root: $ENV{VCPKG_ROOT}")
#if (WIN32)
if(DEFINED ENV{VCPKG_ROOT})
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT})
else()
# If not set, find vcpkg executable and take its' parent
find_program(VCPKG_PATH vcpkg)
message(STATUS "Found vcpkg: ${VCPKG_PATH}")
cmake_path(GET VCPKG_PATH PARENT_PATH VCPKG_ROOT)
message(STATUS "vcpkg root: ${VCPKG_ROOT}")
endif()

project(Dyscover
  VERSION 4.0.5.0
  DESCRIPTION "Clevy Dyscover"
  HOMEPAGE_URL "https://clevy.com"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)  # Use C++14

add_executable(Dyscover WIN32
  src/App.cpp
  src/App.h
  src/Audio.cpp
  src/Audio.h
  src/AudioLevel.cpp
  src/AudioLevel.h
  src/Config.cpp
  src/Config.h
  src/Core.cpp
  src/Core.h
  src/Device.cpp
  src/Device.h
  src/Keyboard.cpp
  src/Keyboard.h
  src/Keys.cpp
  src/Keys.h
  src/LicensingDemo.cpp
  src/LicensingDemo.h
  src/PreferencesDialog.cpp
  src/PreferencesDialog.h
  src/Queue.h
  src/ResourceLoader.cpp
  src/ResourceLoader.h
  src/SoundPlayer.cpp
  src/SoundPlayer.h
  src/Speech.cpp
  src/Speech.h
  src/TrayIcon.cpp
  src/TrayIcon.h
)

# Platform-specifics
if(WIN32)
  target_sources(Dyscover PRIVATE src/DeviceWindows.cpp src/DeviceWindows.h)
  target_link_libraries(Dyscover cfgmgr32.lib)
  target_sources(Dyscover PRIVATE src/KeyboardWindows.cpp src/KeyboardWindows.h)
elseif(UNIX)
  target_sources(Dyscover PRIVATE src/DeviceLinux.cpp src/DeviceLinux.h)
  target_sources(Dyscover PRIVATE src/KeyboardLinux.cpp src/KeyboardLinux.h)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

# Prevent adding /W3 to CMAKE_<LANG>_FLAGS by default
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

# Warnings
if(MSVC)
  target_compile_options(Dyscover PRIVATE /W4 /WX)
else()
  target_compile_options(Dyscover PRIVATE -Wall -Wextra -Werror -pedantic -Wno-error=missing-field-initializers)
endif()

# Use wxWidgets
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(Dyscover ${wxWidgets_LIBRARIES})

# Use PortAudio
if(WIN32)
  find_package(PortAudio REQUIRED)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(PORTAUDIO portaudio-2.0)
endif()
target_link_libraries(Dyscover portaudio)

# Use librstts
target_include_directories(Dyscover PRIVATE lib/rstts/include/)

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
    target_sources(Dyscover PRIVATE ${LIBRSTTS_LIB_FILE})
    target_link_libraries(Dyscover ${LIBRSTTS_LIB_FILE})

    # Copy .dll file to the folder where .exe is
    add_custom_command(
        TARGET Dyscover POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRSTTS_DLL_FILE} $<TARGET_FILE_DIR:Dyscover>
    )

    # Copy data files
    add_custom_command(
        TARGET Dyscover POST_BUILD
        COMMAND xcopy \"${CMAKE_SOURCE_DIR}/res/data/tts\" \"$<TARGET_FILE_DIR:Dyscover>/tts\" /s /y /i /d
        COMMENT "Updating TTS data files ..."
    )
else()
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/rstts/platforms/x86_64-pc-linux-gnu)
    target_link_libraries(Dyscover rstts)
endif()

# Languages
set(LANGUAGE "nl" CACHE STRING "Specifies which language to use")
set_property(CACHE LANGUAGE PROPERTY STRINGS nl nl_be)
message(STATUS "Language is set to ${LANGUAGE}.")
if(LANGUAGE STREQUAL "nl")
  set(LANG "NL")
  set(LANG_NAME "Dutch")
  set(LANG_ID "0413")  # 0x0413 = Dutch
  set(CHARSET "04B0")  # 0x04B0 = 1200 = Unicode
  set(TTS_LANG "nl_nl")
  set(TTS_VOICE "Ilse")
  target_compile_definitions(Dyscover PRIVATE __LANGUAGE_NL__)
elseif(LANGUAGE STREQUAL "nl_be")
  set(LANG "BE")
  set(LANG_NAME "Flemish")
  set(LANG_ID "0813")  # 0x0813 = Belgian Dutch
  set(CHARSET "04B0")  # 0x04B0 = 1200 = Unicode
  set(TTS_LANG "nl_be")
  set(TTS_VOICE "Veerle")
  target_compile_definitions(Dyscover PRIVATE __LANGUAGE_NL_BE__)
else()
  message(FATAL_ERROR "Unsupported language.")
endif()

# Translations
find_package(Gettext REQUIRED)
set(PO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/res/lang/${LANGUAGE}.po)
set(MO_FILE ${CMAKE_CURRENT_BINARY_DIR}/Dyscover.mo)
add_custom_command(
  MAIN_DEPENDENCY ${PO_FILE}
  OUTPUT ${MO_FILE}
  COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} ${PO_FILE} -o"${MO_FILE}"
)
target_sources(Dyscover PRIVATE ${MO_FILE})

# Licensing
set(LICENSING "demo" CACHE STRING "Specifies which licensing of the app to use")
set_property(CACHE LICENSING PROPERTY STRINGS demo full none)
message(STATUS "Licensing is set to ${LICENSING}.")
if(LICENSING STREQUAL "demo")
  set(LICENSING_NAME "Demo")
  target_compile_definitions(Dyscover PRIVATE __LICENSING_DEMO__)
elseif(LICENSING STREQUAL "full")
  set(LICENSING_NAME "Full")
  target_compile_definitions(Dyscover PRIVATE __LICENSING_FULL__)
elseif(LICENSING STREQUAL "none")
  set(LICENSING_NAME "Development")
  target_compile_definitions(Dyscover PRIVATE __LICENSING_NONE__)
else()
  message(FATAL_ERROR "Invalid licensing mode! Valid modes are demo, full and none.")
endif()

# Version
configure_file(res/VersionInfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated/VersionInfo.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated/)

# Resources
if(WIN32)
    target_sources(Dyscover PRIVATE res/Dyscover.rc)
elseif(APPLE)
    file(COPY res/icons/Icon.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon0.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon1.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon2.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon3.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon4.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon5.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon6.icns DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/Splash.bmp DESTINATION ${CMAKE_BINARY_DIR})
else()
    file(COPY res/icons/Icon.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon0.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon1.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon2.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon3.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon4.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon5.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/icons/Icon6.ico DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY res/Splash.bmp DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Sounds
file(GLOB SOUND_FILES "res/data/*.wav")
add_custom_command(
    TARGET Dyscover POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Dyscover>/audio
)
add_custom_command(
    TARGET Dyscover POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOUND_FILES} $<TARGET_FILE_DIR:Dyscover>/audio
)

# Installation
install(TARGETS Dyscover RUNTIME DESTINATION .)
install(FILES ${LIBRSTTS_DLL_FILE} DESTINATION .)
install(FILES ${SOUND_FILES} DESTINATION audio)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_LANG}.db DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_LANG}.fsa DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_LANG}.fst DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_VOICE}.db DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_VOICE}.fon DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_VOICE}.opu DESTINATION tts/data/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/tts/data/${TTS_VOICE}.udb DESTINATION tts/data/)

# Decode platform name
set(PLATFORM_NAME ${CMAKE_SYSTEM_NAME})
if(PLATFORM_NAME MATCHES "Windows")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_NAME win64)
  else()
    set(PLATFORM_NAME win32)
  endif()
endif()

# Packaging
set(CPACK_PACKAGE_NAME "Clevy Dyscover 4 ${LANG_NAME} ${LICENSING_NAME}")
set(CPACK_PACKAGE_VENDOR "Alt. Medical B.V.")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Clevy Dyscover 4")
set(CPACK_PACKAGE_EXECUTABLES "Dyscover;Clevy Dyscover 4")
set(CPACK_PACKAGE_FILE_NAME "Dyscover-${LANGUAGE}-${LICENSING}-${CMAKE_PROJECT_VERSION}-${PLATFORM_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/res/License.rtf")
if(WIN32)
    set(CPACK_GENERATOR WIX)
    set(CPACK_WIX_UPGRADE_GUID 12C19B6B-BB03-4255-AD65-0A46D994886B) # NOTE: Never change this and never reuse in any other project.
    set(CPACK_WIX_PRODUCT_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/icons/Icon.ico)
    set(CPACK_WIX_PROGRAM_MENU_FOLDER "Clevy Dyscover 4")
    set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "https://clevy.com/")
    set(CPACK_WIX_PROPERTY_ARPHELPLINK "https://clevy.com/")
    set(CPACK_WIX_EXTENSIONS "WixUIExtension" "WixUtilExtension")
    set(CPACK_WIX_PATCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CPackWixPatch.xml)
endif()
include(CPack)
