#include <iostream>

#include "Application.h"
#include "Command.h"
#include "Utility.h"


Application::Application()
{
    this->activeTable = new Table();

    activeTable->subscribe(&layout);

    cmdParser.registerCommand("quit", [this](const auto&) {
        return new QuitCommand(*this);
    });

    cmdParser.registerCommand("help", [this](const auto&) {
        return new HelpCommand(*this);
    });

    cmdParser.registerCommand("print", [this](const std::vector<std::string>&) {
        return new PrintCommand(*this);
    });

    cmdParser.registerCommand("edit <arg> <arg> ...", [this](const std::vector<std::string>& args) {
        const auto row = static_cast<size_t>(parse_uint(args[0]));
        const auto col = static_cast<size_t>(parse_uint(args[1]));

        if (row == 0) throw std::runtime_error("Expected a valid row number.");
        if (col == 0) throw std::runtime_error("Expected a valid column number.");

        return new EditCellCommand(this->getActiveTable(), row - 1, col - 1, args[2]);
    });

    cmdParser.registerCommand("save as ...", [this](const std::vector<std::string>& args) {
        return new SaveAsCommand(*this, args[0]);
    });

    cmdParser.registerCommand("save", [this](const std::vector<std::string>&) {
        return new SaveCommand(*this);
    });

    cmdParser.registerCommand("open ...", [this](const std::vector<std::string>& args) {
       return new OpenCommand(*this, args[0]);
   });

    this->running = true;
}

void Application::apply(const std::string& text) const
{
    Command* command = cmdParser.parse(text);
    if (command == nullptr)
    {
        std::cout << "Unknown command. Use \"help\" to get a full list of commands." << std::endl;
        return;
    }

    try {
        command->execute();
    }
    catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    delete command;
}

void Application::run() const
{
    std::string commandText;
    while (isRunning())
    {
        std::cout << "> " << std::flush;
        std::getline(std::cin, commandText);

        if (!commandText.empty())
            this->apply(commandText);
    }
}

// takes ownership of table
void Application::setActiveTable(Table *newTable)
{
    if (activeTable)
    {
        activeTable->unsubscribe(&layout);
        delete activeTable;
    }

    activeTable = newTable;

    if (activeTable != nullptr) activeTable->subscribe(&layout);

    layout.updateFromTable(activeTable);
}


Application::~Application()
{
    activeTable->unsubscribe(&layout);

    delete activeTable;
}
