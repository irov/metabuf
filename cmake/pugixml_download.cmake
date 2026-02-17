cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

ExternalProject_Add(pugixml_download PREFIX pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
		GIT_TAG "v1.15"
		GIT_PROGRESS TRUE
            
        UPDATE_COMMAND ""
      
        CMAKE_ARGS 
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DPUGIXML_BUILD_SHARED_AND_STATIC_LIBS=OFF
            -DPUGIXML_BUILD_TESTS=OFF
            -DPUGIXML_STATIC_CRT=ON
            -DPUGIXML_WCHAR_MODE=OFF
            -DPUGIXML_COMPACT=ON
            -DPUGIXML_NO_XPATH=ON
            -DPUGIXML_NO_STL=ON
            -DPUGIXML_NO_EXCEPTIONS=ON
    )

ExternalProject_Get_Property(pugixml_download INSTALL_DIR)
set(pugixml_INCLUDE_DIR ${INSTALL_DIR}/include)
set(pugixml_LIBRARY_DIR ${INSTALL_DIR}/lib)
set(pugixml_LIBRARY_NAME ${pugixml_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}pugixml${CMAKE_STATIC_LIBRARY_SUFFIX})

add_library(pugixml STATIC IMPORTED)

set_target_properties(pugixml PROPERTIES IMPORTED_LOCATION ${pugixml_LIBRARY_NAME})