@echo off

set "PROJECT=metabuf"
set "CONFIGURATION=%1"
set "SOLUTION_DIR=..\..\solutions\%PROJECT%_msvc16\%CONFIGURATION%"

@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
call CMake -G "Visual Studio 16 2019" -S "%CD%\..\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='%CONFIGURATION%'
@popd

@echo on
@pause
