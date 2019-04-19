cmake_minimum_required(VERSION 3.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE_DIRECTORIES( ${TEST_PROJECT_NAME} ${METABUF_INCLUDE_PATH} )

ADD_LIBRARY( test_common STATIC Metacode.cpp Metacode.h Metatype.h Metatype.cpp )

set_target_properties(test_common PROPERTIES
    FOLDER tests
)

macro(ADD_MOVIE_TEST testname)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    
    set (test_extra_library ${ARGN})
    
    SET(TEST_PROJECT_NAME test_${testname})
    
    INCLUDE_DIRECTORIES( ${TEST_PROJECT_NAME} ${METABUF_INCLUDE_PATH} )
    INCLUDE_DIRECTORIES( ${TEST_PROJECT_NAME} ${METABUF_PUGIXML_INCLUDE_DIR} )
    
    ADD_EXECUTABLE(${TEST_PROJECT_NAME} test_${testname}.cpp)
    
    TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME} metabuf)
    TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME} xml2metabuf)
    TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME} pugixml)
    
    FOREACH(library ${test_extra_library})
        TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME} ${library})
    ENDFOREACH()
    
    if(NOT METABUF_PUGIXML_EXTERNAL)
        add_dependencies(${PROJECT_NAME} pugixml_download)
    endif()

    set_target_properties(${TEST_PROJECT_NAME} PROPERTIES
        FOLDER tests
    )

    ADD_TEST(NAME ${testname} COMMAND ./test_${testname} ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

ADD_MOVIE_TEST(open_protocol)
ADD_MOVIE_TEST(make_metacode)
ADD_MOVIE_TEST(generate_bin test_common metabuf)
ADD_MOVIE_TEST(parse_bin test_common)
ADD_MOVIE_TEST(memory_leak test_common)