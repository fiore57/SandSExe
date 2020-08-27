cmake_minimum_required (VERSION 3.16)

# プロジェクト名
project(SandSDll64)

#
# info
#
message("# SandSDll64")
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
file(GLOB sands_dll_64
	"*.cpp"
	"*/*.cpp"
	"*.rc"
)
add_library(SandSDll64 STATIC SHARED "${sands_dll_64}")

#
# others
#

# WinMain
set_target_properties(SandSDll64 PROPERTIES COMPILE_FLAGS "")

# プリコンパイル済みヘッダ
target_precompile_headers(SandSDll64 PRIVATE pch.h)

#
# link
#
target_link_libraries(SandSDll64 imm32.lib)
