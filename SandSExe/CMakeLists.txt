cmake_minimum_required (VERSION 3.16)

# プロジェクト名
project(SandSExe)

#
# info
#
message("# SandSExe")
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
file(GLOB sands_exe
	"*.cpp"
	"*/*.cpp"
	"*.rc"
)
message("${sands_exe}")
add_executable (SandSExe WIN32 "${sands_exe}")

#
# others
#

# WinMain
set_target_properties(SandSExe PROPERTIES COMPILE_FLAGS "/subsystem:windows")

# プリコンパイル済みヘッダ
# set_target_properties(SandSExe PROPERTIES COMPILE_FLAGS "/Yu${PROJECT_SOURCE_DIR}/pch.h")
# set_target_properties(SandSExe PROPERTIES COMPILE_FLAGS "/FI${PROJECT_SOURCE_DIR}/pch.h")
# set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Yc${PROJECT_SOURCE_DIR}/pch.h")
target_precompile_headers(SandSExe PRIVATE pch.h)

#
# link
#
target_link_libraries(SandSExe shell32.lib)
target_link_libraries(SandSExe comctl32.lib)
target_link_libraries(SandSExe SandSDll)
