#include <iostream>

#include "Application.h"
#include "Command.h"


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

    this->running = true;
}

void Application::run() const
{
    std::string buffer;
    while (isRunning())
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
}

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
