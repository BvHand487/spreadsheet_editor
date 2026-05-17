#include <iostream>

#include "Table.h"
#include "Cell.h"
#include "Command.h"
#include "Parser.h"


int main()
{
    auto table = Table();

    table.setCell(0, 0, CellFactory::create_cell(CellFactory::Int, "1"));
    table.setCell(4, 0, CellFactory::create_cell(CellFactory::Int, "2"));
    table.setCell(0, 4, CellFactory::create_cell(CellFactory::Int, "3"));
    table.setCell(4, 4, CellFactory::create_cell(CellFactory::Int, "4"));

    CommandParser cmdParser;
    cmdParser.registerCommand("print", [&](const std::vector<std::string>& args) {
        return new PrintCommand(table);
    });

    cmdParser.registerCommand("edit <var> <var> <var>", [&](const std::vector<std::string>& args) {
        const auto row = static_cast<size_t>(CommandParser::parse_uint(args[0]));
        const auto column = static_cast<size_t>(CommandParser::parse_uint(args[1]));

        return new EditCellCommand(table, row, column, args[2]);
    });

    // set up command for quitting
    bool running = true;
    cmdParser.registerCommand("quit", [&](const std::vector<std::string>& args) {
        return new QuitCommand(running);
    });

    cmdParser.registerCommand("help", [&](const std::vector<std::string>& args)
    {
        return new HelpCommand(cmdParser);
    });

    std::string buffer;
    while (running)
    {
        std::cout << "> " << std::flush;
        std::getline(std::cin, buffer);

        Command* command = cmdParser.parse(buffer);
        if (command == nullptr)
        {
            std::cout << "Unknown command. Use \"help\" to get a full list of commands." << std::endl;
            continue;
        }

        try
        {
            command->execute();

        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
        }

        delete command;
    }

    return 0;
}
