#include <iostream>

#include "Table.h"
#include "Cell.h"
#include "Command.h"
#include "Parser.h"

using Node = CommandParser::Node;

int main()
{
    auto table = Table();

    table.setCell(0, 0, CellFactory::create_cell(CellFactory::Int, "1"));
    table.setCell(4, 0, CellFactory::create_cell(CellFactory::Int, "2"));
    table.setCell(0, 4, CellFactory::create_cell(CellFactory::Int, "3"));
    table.setCell(4, 4, CellFactory::create_cell(CellFactory::Int, "4"));

    CommandParser cmdParser;
    cmdParser.registerCommand("print", [&](const std::vector<std::string> &args) {
        return new PrintCommand(table);
    });

    // set up command for quitting
    bool running = true;
    cmdParser.registerCommand("quit", [&](const std::vector<std::string> &args) {
        return new QuitCommand(running);
    });

    std::string buffer;
    while (running)
    {
        std::cout << "> " << std::flush;
        std::getline(std::cin, buffer);

        Command* command = cmdParser.parse(buffer);
        if (command != nullptr)
        {
            command->execute();
            delete command;
        }
    }

    return 0;
}
