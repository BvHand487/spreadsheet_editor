add_library(spreadsheet_editor_core
    core/Cell.cpp
    core/Table.cpp
    core/TableFormatting.cpp
    core/TableSerializer.cpp
    core/Formula.cpp
    core/FormulaParser.cpp
    core/Command.cpp
    core/CommandParser.cpp
    core/Utility.cpp
)

target_include_directories(spreadsheet_editor_core
        PUBLIC ${CMAKE_SOURCE_DIR}/include/spreadsheet_editor/core
)

add_executable(spreadsheet_app core/Application.cpp main.cpp)

target_link_libraries(spreadsheet_app PRIVATE spreadsheet_editor_core)
target_link_options(spreadsheet_app PRIVATE "-static-libgcc" "-static-libstdc++" "-static")