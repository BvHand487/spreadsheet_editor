add_executable(unit_tests
    test_table.cpp
)

target_link_libraries(unit_tests PRIVATE spreadsheet_editor_core)

target_include_directories(unit_tests
        SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party/doctest
)

add_test(NAME tests COMMAND unit_tests)