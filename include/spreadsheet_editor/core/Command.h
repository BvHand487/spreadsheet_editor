#ifndef COMMAND_H
#define COMMAND_H

#include "Table.h"

// TODO: maybe remove
// forward-declare CommandParser for HelpCommand
class CommandParser;

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};


class HelpCommand final : public Command
{
    const CommandParser& parser;

public:
    explicit HelpCommand(const CommandParser& parser) : parser(parser) {}
    void execute() override;
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


class EditCellCommand final : public Command
{
    // TODO: maybe change type of row/column to separate classes
    Table& table;
    const size_t row;
    const size_t column;
    const std::string& value;

public:
    explicit EditCellCommand(Table& table, const size_t row, const size_t column, const std::string& value) :
        table(table),
        row(row),
        column(column),
        value(value) {}
    void execute() override;
};


#endif //COMMAND_H
