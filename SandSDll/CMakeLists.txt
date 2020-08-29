cmake_minimum_required (VERSION 3.16)

# プロジェクト名
project(SandSDll)

#
# info
#
message("# SandSDll")
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
# compile
#
file(GLOB sands_dll
	"*.cpp"
	"*/*.cpp"
	"*.rc"
	"*.def"
)
add_library(SandSDll STATIC SHARED "${sands_dll}")

#
# others
#

# WinMain
set_target_properties(SandSDll PROPERTIES COMPILE_FLAGS "")

# プリコンパイル済みヘッダ
target_precompile_headers(SandSDll PRIVATE pch.hpp)

# dllファイル名を変更
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(SandSDll PROPERTIES OUTPUT_NAME "SandSDll64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set_target_properties(SandSDll PROPERTIES OUTPUT_NAME "SandSDll32")
endif()

#
# link
#
target_link_libraries(SandSDll imm32.lib)
