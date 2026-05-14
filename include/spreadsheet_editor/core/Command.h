#ifndef COMMAND_H
#define COMMAND_H

#include "Table.h"


class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class QuitCommand final : public Command
{
    bool& running;

public:
    explicit QuitCommand(bool& running) : running(running) {}
    void execute() override;
};


class PrintCommand final : public Command
{
    const Table& table;

public:
    explicit PrintCommand(const Table& table) : table(table) {}
    void execute() override;
};

#endif //COMMAND_H
