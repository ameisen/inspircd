cmake_minimum_required(VERSION 2.8)
set(CMAKE_CONFIGURATION_TYPES Debug Development RelWithDebInfo MinSizeRel Release)

project(InspIRCd CXX)

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "15.3.2")
    message(FATAL_ERROR "This project requires at least MSVC 15.3.2 (2017, updated to 3.2) to compile.")
  endif()
else()
  message(FATAL_ERROR "This project requires at least MSVC 15.3.2 (2017, updated to 3.2) to compile.")
endif()

#set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" Development)

# /arch:AVX

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  message(FATAL_ERROR "1")
elif(CMAKE_BUILD_TYPE MATCHES Development)
  message(FATAL_ERROR "2")
elif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  message(FATAL_ERROR "3")
elif(CMAKE_BUILD_TYPE MATCHES RelWithDebugInfo)
  message(FATAL_ERROR "4")
elif(CMAKE_BUILD_TYPE MATCHES Release)
  message(FATAL_ERROR "5")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:rvalueCast /std:c++latest /permissive- /EHsc /GR- /W4")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /sdl /Oy- /RTC1 /guard:cf /fp:strict /fp:except /GF")
set(CMAKE_CXX_FLAGS_DEVELOPMENT "${CMAKE_CXX_FLAGS_RELWITHDEBUGINFO} /sdl /Gy /Oy- /guard:cf /fp:fast /fp:except- /Oi /Ot /O2 /Ob1 /GT /GF")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /Gy /fp:fast /fp:except- /Ob2 /Os /Oy /GT /GL /GF /GS-")
set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "${CMAKE_CXX_FLAGS_RELWITHDEBUGINFO} /sdl /Oy- /guard:cf /Gy /fp:fast /fp:except- /Oi /Ot /GT /GL /GF")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /sdl /guard:cf /Gy /fp:fast /fp:except- /Oi /Ot /Oy /GT /GL /GF")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
#set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")

set(CMAKE_EXE_LINKER_FLAGS_DEVELOPMENT "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_DEVELOPMENT "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
set(CMAKE_STATIC_LINKER_FLAGS_DEVELOPMENT "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}")
set(CMAKE_MODULE_LINKER_FLAGS_DEVELOPMENT "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /LARGEADDRESSAWARE")
#set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL "${CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL "${CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL} /LTCG:incremental")

set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBUGINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBUGINFO} /LTCG:incremental")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBUGINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBUGINFO} /LTCG:incremental")
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBUGINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBUGINFO} /LTCG:incremental")
set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBUGINFO "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBUGINFO} /LTCG:incremental")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG:incremental /OPT:REF /OPT:ICF")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /LTCG:incremental")

set(CONFIG_DIR "conf" CACHE PATH "Configuration file path")
set(MODULE_DIR "modules" CACHE PATH "Module path")
set(DATA_DIR "data" CACHE PATH "Data path")
set(LOG_DIR "logs" CACHE PATH "Log file path")

set(EXTRA_INCLUDES "" CACHE PATH "Extra include paths")
set(EXTRA_LIBS "" CACHE PATH "Extra library paths")

set(INSPIRCD_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../")

set(SYSTEM_NAME_VERSION ${CMAKE_SYSTEM})
set(SOCKETENGINE "select")

# Build with multiple processes
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

# Use our own NSIS template
set(CMAKE_MODULE_PATH "${INSPIRCD_BASE}/win")

# Grab version info from version.sh
file(STRINGS "${INSPIRCD_BASE}/src/version.sh" VERSIONSH)
string(REGEX REPLACE ".*InspIRCd-([0-9]*).*" "\\1" VERSION_MAJOR "${VERSIONSH}")
string(REGEX REPLACE ".*InspIRCd-[0-9]*\\.([0-9]*).*" "\\1" VERSION_MINOR "${VERSIONSH}")
string(REGEX REPLACE ".*InspIRCd-[0-9]*\\.[0-9]*\\.([0-9]*).*" "\\1" VERSION_PATCH "${VERSIONSH}")

file(GLOB INSPIRCD_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
"${INSPIRCD_BASE}/win/inspircd_win32wrapper.cpp"
"${INSPIRCD_BASE}/win/win32service.cpp"
"${INSPIRCD_BASE}/src/*.cpp"
"${INSPIRCD_BASE}/src/modes/*.cpp"
"${INSPIRCD_BASE}/src/socketengines/socketengine_select.cpp"
"${INSPIRCD_BASE}/src/threadengines/threadengine_win32.cpp"
"${INSPIRCD_BASE}/win/inspircd_win32wrapper.h"
"${INSPIRCD_BASE}/win/win32service.h"
"${INSPIRCD_BASE}/include/*.h"
"${INSPIRCD_BASE}/include/threadengines/threadengine_win32.h"
)
#file(GLOB_RECURSE INSPIRCD_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
#"${INSPIRCD_BASE}/include/*.h")
list(SORT INSPIRCD_SOURCES)

include_directories("${INSPIRCD_BASE}/win" "${INSPIRCD_BASE}/include")

include_directories(${EXTRA_INCLUDES})
link_directories(${EXTRA_LIBS})

if(MSVC)
	add_library(win32_memory STATIC "${INSPIRCD_BASE}/win/inspircd_memory_functions.cpp")
endif(MSVC)

configure_file("${INSPIRCD_BASE}/win/inspircd.rc.cmake" "${INSPIRCD_BASE}/win/inspircd.rc")
configure_file("${INSPIRCD_BASE}/make/template/config.h" "${INSPIRCD_BASE}/include/config.h")

add_executable(inspircd ${INSPIRCD_SOURCES} "${INSPIRCD_BASE}/win/inspircd.rc")
target_link_libraries(inspircd win32_memory)
set_target_properties(inspircd PROPERTIES ENABLE_EXPORTS ON)
install(TARGETS inspircd DESTINATION .)

add_subdirectory(modules)

# Package any DLLs in win/
file(GLOB EXTRA_DLLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INSPIRCD_BASE}/win/*.dll")
install(FILES ${EXTRA_DLLS} DESTINATION .)

# Install example configs
file(GLOB_RECURSE EXAMPLE_CONFIGS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INSPIRCD_BASE}/docs/conf/*.example")
install(FILES ${EXAMPLE_CONFIGS} DESTINATION conf)

# Install nationalchars files
file(GLOB_RECURSE EXAMPLE_LOCALES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INSPIRCD_BASE}/locales/*")
install(FILES ${EXAMPLE_LOCALES} DESTINATION locales)

# Create an empty data and logs directory and install them
file(MAKE_DIRECTORY ${DATA_DIR})
install(DIRECTORY ${DATA_DIR} DESTINATION .)
file(MAKE_DIRECTORY ${LOG_DIR})
install(DIRECTORY ${LOG_DIR} DESTINATION .)

if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
	set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".") # place runtime libraries next to InspIRCd binary
	include(InstallRequiredSystemLibraries)

	set(CPACK_PACKAGE_NAME "InspIRCd IRC Daemon")
	set(CPACK_PACKAGE_VENDOR "InspIRCd Development Team")
	set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
	set(CPACK_PACKAGE_FILE_NAME "InspIRCd-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../docs/COPYING")

	set(CPACK_GENERATOR "NSIS")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "InspIRCd")
	# NSIS has a bug with full nix paths, so this must contain at least one backslash
	set(CPACK_PACKAGE_ICON "${INSPIRCD_BASE}/win\\\\inspircd.ico")
	set(CPACK_NSIS_MUI_ICON "${INSPIRCD_BASE}/win\\\\inspircd.ico")
	set(CPACK_NSIS_MUI_UNIICON "${INSPIRCD_BASE}/win\\\\inspircd.ico")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "inspircd.exe")
	set(CPACK_NSIS_URL_INFO_ABOUT "http://www.inspircd.org/")
	set(CPACK_NSIS_COMPRESSOR "/SOLID zlib")

	include(CPack)
endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
