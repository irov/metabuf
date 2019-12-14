@echo off

set "PROJECT=metabuf"
set "CONFIGURATION=Debug"
set "SOLUTION_DIR=..\solutions\%PROJECT%_msvc16\%CONFIGURATION%"

@pushd ..
@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
CMake -G "Visual Studio 16 2019" "%CD%\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='%CONFIGURATION%' -DCMAKE_BUILD_TYPE:STRING='%CONFIGURATION%'
@popd
@popd

@echo on
@pause
