#include <iostream>

#include "Application.h"
#include "Command.h"


Application::Application()
{
    this->activeTable = nullptr;

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
            // addCommandToHistory(command);
        }
        catch (const std::exception& err)
        {
            std::cout << err.what() << std::endl;
        }

        delete command;
    }
}

// void Application::addCommandToHistory(Command *command)
// {
//     cmdHistory.push_back(command);
//
//     if (cmdHistory.size() > COMMAND_HISTORY_LENGTH)
//         cmdHistory.erase(cmdHistory.begin());
// }

Application::~Application()
{
    // for (const Command* command : cmdHistory)
    //     delete command;

    delete activeTable;
}
