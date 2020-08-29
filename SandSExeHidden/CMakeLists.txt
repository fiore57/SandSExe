cmake_minimum_required (VERSION 3.16)

# プロジェクト名
project(SandSExeHidden)

#
# info
#
message("# SandSExeHidden")
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
file(GLOB sands_exe_hidden
	"*.cpp"
	"*/*.cpp"
	"*.rc"
)
message("${sands_exe_hidden}")
add_executable (SandSExeHidden WIN32 "${sands_exe_hidden}")

#
# others
#

# WinMain
set_target_properties(SandSExeHidden PROPERTIES COMPILE_FLAGS "/subsystem:windows")

# プリコンパイル済みヘッダ
target_precompile_headers(SandSExeHidden PRIVATE pch.hpp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(SandSExeHidden PROPERTIES OUTPUT_NAME "SandSExeHidden64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set_target_properties(SandSExeHidden PROPERTIES OUTPUT_NAME "SandSExeHidden32")
endif()

#
# link
#
target_link_libraries(SandSExeHidden shell32.lib)
target_link_libraries(SandSExeHidden comctl32.lib)
target_link_libraries(SandSExeHidden shlwapi.lib)
# target_link_libraries(SandSExeHidden "${CMAKE_BINARY_DIR}/SandSDll/SandSDll64.lib")
