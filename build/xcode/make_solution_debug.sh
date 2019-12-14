#! /bin/bash

CONFIGURATION=Debug

mkdir -p ../solutions/solution_xcode/$CONFIGURATION
pushd ../solutions/solution_xcode/$CONFIGURATION
/Applications/CMake.app/Contents/bin/cmake -G"Xcode" "$PWD/../.." -DCMAKE_BUILD_TYPE:STRING='$CONFIGURATION' -DCMAKE_CONFIGURATION_TYPES:STRING='$CONFIGURATION' -DCMAKE_OSX_SYSROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9
popd
