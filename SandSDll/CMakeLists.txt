# プロジェクト名
project(SandSDll)

#
# info
#
message("# SandSDll")

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

# プリコンパイル済みヘッダ
target_precompile_headers(SandSDll PRIVATE pch.hpp)

# dllファイル名を変更
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(SandSDll PROPERTIES OUTPUT_NAME "SandS64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set_target_properties(SandSDll PROPERTIES OUTPUT_NAME "SandS32")
endif()

#
# link
#
target_link_libraries(SandSDll imm32.lib)
