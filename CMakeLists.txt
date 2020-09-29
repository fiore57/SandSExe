cmake_minimum_required (VERSION 3.16)

# 言語の設定
enable_language(CXX)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# プロジェクト名
project ("SandS")

#
# info
#
message("# SandS")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message("## x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message("## x86")
else()
	message("## x??")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message("## Debug")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	message("## Release")
endif()

#
# macro
#
if(MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DUNICODE /D_UNICODE")
endif()

#
# warning
#
if(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../release")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../release")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# x64
	add_subdirectory ("SandSDll")
	add_subdirectory ("SandSHidden")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# x86
	add_subdirectory ("SandSDll")
	add_subdirectory ("SandSExe")
	add_subdirectory ("SandSHidden")
endif()

