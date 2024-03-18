find_package(GTest REQUIRED)

set(TEST_LIBS libublox GTest::GTest GTest::Main)

macro(declare_test NAME FILE)
    add_executable(${NAME} ${FILE})
    target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${NAME} ${LIBS} ${TEST_LIBS})
    add_test(NAME ${NAME} COMMAND ${NAME} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)
endmacro()

add_subdirectory(util)

declare_test(packetParsingTests packetParsing.cpp)
