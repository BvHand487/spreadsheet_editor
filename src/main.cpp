#include <iostream>

#include "Application.h"
#include "Table.h"
#include "Cell.h"
#include "Command.h"
#include "CommandParser.h"
#include "Utility.h"


int main()
{
    Application& app = Application::getInstance();

    app.setActiveTable(new Table());
    Table *table = app.getActiveTable();
    table->setCell(0, 0, CellFactory::create_cell(CellFactory::Int, "1"));
    table->setCell(0, 1, CellFactory::create_cell(CellFactory::Int, "1"));
    table->setCell(0, 2, CellFactory::create_cell(CellFactory::Formula, "= R0C0 + R0C1"));

    CommandParser& parser = app.getCommandParser();

    parser.registerCommand("print", [&](const std::vector<std::string>&) {
        return new PrintCommand(app.getActiveTable());
    });

    parser.registerCommand("edit <arg> <arg> ...", [&](const std::vector<std::string>& args) {
        const auto row = static_cast<size_t>(parse_uint(args[0]));
        const auto column = static_cast<size_t>(parse_uint(args[1]));

        return new EditCellCommand(app.getActiveTable(), row, column, args[2]);
    });
    
    parser.registerCommand("save as ...", [&](const std::vector<std::string>& args) {
        return new SaveAsCommand(app, args[0]);
    });

    parser.registerCommand("save", [&](const std::vector<std::string>&) {
        return new SaveCommand(app);
    });

    app.run();

    return 0;
}
