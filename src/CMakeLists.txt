add_library(spreadsheet_editor_core
    core/Cell.cpp
    core/Command.cpp
    core/Table.cpp
        core/Parser.cpp
        ../include/spreadsheet_editor/core/Parser.h
        ../include/spreadsheet_editor/core/Utility.h
        core/Utility.cpp
)

target_include_directories(spreadsheet_editor_core
        PUBLIC ${CMAKE_SOURCE_DIR}/include/spreadsheet_editor/core
)

add_executable(spreadsheet_app main.cpp)
target_link_libraries(spreadsheet_app PRIVATE spreadsheet_editor_core)