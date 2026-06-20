#ifndef APPLICATION_H
#define APPLICATION_H

#include "CommandParser.h"
#include "Table.h"

class Application
{
    bool running = true;
    Table *activeTable = nullptr;
    std::string activeSavePath {};

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
    Application(Application&&) noexcept = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) noexcept = delete;

    void run() const;

    [[nodiscard]] bool isRunning() const { return running; };
    void quit() { running = false; };

    [[nodiscard]] Table* getActiveTable() const { return activeTable; };
    void setActiveTable(Table* activeTable) { this->activeTable = activeTable; };

    [[nodiscard]] const std::string& getActiveSavePath() const { return activeSavePath; };
    void setActiveSavePath(std::string savePath) { this->activeSavePath = std::move(savePath); };

    [[nodiscard]] CommandParser& getCommandParser() { return cmdParser; };

    // std::vector<Command *>& getCommandHistory() { return cmdHistory; };
    // void addCommandToHistory(Command *command);

    ~Application();
};



#endif //APPLICATION_H
