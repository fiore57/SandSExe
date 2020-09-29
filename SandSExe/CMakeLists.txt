# プロジェクト名
project(SandSExe)

#
# info
#
message("# SandSExe")

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

# プリコンパイル済みヘッダ
# set_target_properties(SandSExe PROPERTIES COMPILE_FLAGS "/Yu${PROJECT_SOURCE_DIR}/pch.h")
# set_target_properties(SandSExe PROPERTIES COMPILE_FLAGS "/FI${PROJECT_SOURCE_DIR}/pch.h")
# set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Yc${PROJECT_SOURCE_DIR}/pch.h")
target_precompile_headers(SandSExe PRIVATE pch.hpp)

#
# link
#
target_link_libraries(SandSExe shell32.lib)
target_link_libraries(SandSExe comctl32.lib)
target_link_libraries(SandSExe shlwapi.lib)

# 管理者権限
set_target_properties(SandSExe PROPERTIES LINK_FLAGS /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'")
