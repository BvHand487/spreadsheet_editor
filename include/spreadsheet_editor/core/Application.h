#ifndef APPLICATION_H
#define APPLICATION_H

#include "CommandParser.h"
#include "Table.h"

class Application
{
    bool running = true;
    Table *activeTable = nullptr;

    // static constexpr size_t COMMAND_HISTORY_LENGTH = 128;
    // std::vector<Command *> cmdHistory;
    CommandParser cmdParser;

public:
    static Application& getInstance() {
        static Application instance;
        return instance;
    }

    Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run() const;

    [[nodiscard]] bool isRunning() const { return running; };
    void quit() { running = false; };

    [[nodiscard]] Table* getActiveTable() const { return activeTable; };
    void setActiveTable(Table* activeTable) { this->activeTable = activeTable; };

    [[nodiscard]] CommandParser& getCommandParser() { return cmdParser; };

    // std::vector<Command *>& getCommandHistory() { return cmdHistory; };
    // void addCommandToHistory(Command *command);

    ~Application();
};



#endif //APPLICATION_H
