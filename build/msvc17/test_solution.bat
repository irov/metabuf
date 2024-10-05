@echo off

set "PROJECT=metabuf"
set "CONFIGURATION=%1"
set "SOLUTION_DIR=..\..\solutions\test_%PROJECT%_msvc17\%CONFIGURATION%"

@mkdir %SOLUTION_DIR%
@pushd %SOLUTION_DIR%
CMake -G "Visual Studio 17 2022" "%CD%\..\..\.." -DCMAKE_CONFIGURATION_TYPES:STRING='%CONFIGURATION%' -DCMAKE_BUILD_TYPE:STRING='%CONFIGURATION%'
CMake --build . --config %CONFIGURATION%
CTest -C %CONFIGURATION%
@popd

@echo on
@pause
