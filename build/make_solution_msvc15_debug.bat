@echo off

set "build_dir=build"
set "configuration=Debug"
set "CMAKE_EXE=%CD%\..\..\cmake\bin\cmake.exe"
set "SOLUTION_DIR=solutions\metabuf_msvc15_debug"

@pushd ..
@call vcvarsall_msvc15
@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
%CMAKE_EXE% -G "Visual Studio 15 2017" "%CD%\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='Debug' -DCMAKE_BUILD_TYPE:STRING='Debug' -DMETABUF_PUGIXML_INCLUDE:STRING="../pugixml/src" -DMETABUF_PUGIXML_LIB:STRING="../../../pugixml/scripts/vs2017/Win32_DebugStatic/pugixml"
@popd

@echo on
@pause
