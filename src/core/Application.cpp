#include <iostream>

#include "Application.h"
#include "Command.h"
#include "Utility.h"


Application::Application()
{
    this->activeTable = new Table();

    activeTable->subscribe(&layout);

    cmdParser.registerCommand("save as ...",
        [](const std::vector<std::string>& args, Application *app) -> Command* {
            return new SaveAsCommand(*app, args[0]);
        },
        this
    );

    cmdParser.registerCommand("save",
        [](auto&, Application *app) -> Command* {
            return new SaveCommand(*app);
        },
        this
    );

    cmdParser.registerCommand("open ...",
    [](const std::vector<std::string>& args, Application *app) -> Command* {
            return new OpenCommand(*app, args[0]);
        },
        this
    );

    cmdParser.registerCommand("quit",
        [](auto&, Application *app) -> Command* {
            return new QuitCommand(*app);
        },
        this
    );

    cmdParser.registerCommand("help",
        [](auto&, Application *app) -> Command* {
            return new HelpCommand(*app);
        },
        this
    );

    cmdParser.registerCommand("print",
        [](auto&, Application *app) -> Command* {
            return new PrintCommand(*app);
        },
        this
    );

    cmdParser.registerCommand("clear",
        [](auto&, Application *app) -> Command* {
            return new ClearCommand(app->getActiveTable());
        },
        this
    );

    cmdParser.registerCommand("edit <arg> <arg> ...",
        [](const std::vector<std::string>& args, Application *app) -> Command* {
            const auto row = static_cast<size_t>(parse_uint(args[0]));
            const auto col = static_cast<size_t>(parse_uint(args[1]));

            if (row == 0) throw std::runtime_error("Expected a valid row number.");
            if (col == 0) throw std::runtime_error("Expected a valid column number.");

            return new EditCellCommand(app->getActiveTable(), row - 1, col - 1, args[2]);
        },
        this
    );

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
