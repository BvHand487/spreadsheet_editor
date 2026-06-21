#ifndef APPLICATION_H
#define APPLICATION_H

#include "CommandParser.h"
#include "Table.h"

class Application
{
    bool running = true;

    Table *activeTable = nullptr;
    TableLayout layout {};

    std::string activeSavePath {};
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
    void setActiveTable(Table* newTable);

    [[nodiscard]] const TableLayout& getLayout() const { return layout; };

    [[nodiscard]] const std::string& getActiveSavePath() const { return activeSavePath; };
    void setActiveSavePath(std::string savePath) { this->activeSavePath = std::move(savePath); };

    [[nodiscard]] CommandParser& getCommandParser() { return cmdParser; };

    ~Application();
};



#endif //APPLICATION_H
