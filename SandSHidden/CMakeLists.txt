# プロジェクト名
project(SandSHidden)

#
# info
#
message("# SandSHidden")

#
# compile
#
file(GLOB sands_hidden
	"*.cpp"
	"*/*.cpp"
	"*.rc"
)
message("${sands_hidden}")
add_executable (SandSHidden WIN32 "${sands_hidden}")

#
# others
#

# プリコンパイル済みヘッダ
target_precompile_headers(SandSHidden PRIVATE pch.hpp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(SandSHidden PROPERTIES OUTPUT_NAME "SandSHidden64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set_target_properties(SandSHidden PROPERTIES OUTPUT_NAME "SandSHidden32")
endif()

#
# link
#
target_link_libraries(SandSHidden shell32.lib)
target_link_libraries(SandSHidden comctl32.lib)
target_link_libraries(SandSHidden shlwapi.lib)
# target_link_libraries(SandSHidden "${CMAKE_BINARY_DIR}/SandSDll/SandSDll64.lib")
