include(ExternalProject)

ExternalProject_Add(pugixml_download PREFIX glad
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
		GIT_TAG "v1.9"
		GIT_PROGRESS TRUE
            
        UPDATE_COMMAND ""
      
        CMAKE_ARGS 
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DBUILD_SHARED_LIBS=OFF
            -DBUILD_TESTS=OFF
            -DSTATIC_CRT=ON
    )

ExternalProject_Get_Property(pugixml_download INSTALL_DIR)
set(pugixml_INCLUDE_DIR ${INSTALL_DIR}/include)
set(pugixml_LIBRARY_DIR ${INSTALL_DIR}/lib)
set(pugixml_LIBRARY_NAME ${pugixml_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}pugixml${CMAKE_STATIC_LIBRARY_SUFFIX})

add_library(pugixml STATIC IMPORTED)

set_target_properties(pugixml PROPERTIES IMPORTED_LOCATION ${pugixml_LIBRARY_NAME})