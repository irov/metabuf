@echo off

set "PROJECT=metabuf"
set "CONFIGURATION=%1"
set "SOLUTION_DIR=..\..\solutions\%PROJECT%_msvc17\%CONFIGURATION%"

@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
call CMake -G "Visual Studio 17 2022" -S "%CD%\..\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='%CONFIGURATION%'
@popd

@echo on
@pause
