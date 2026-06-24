#ifndef COMMAND_H
#define COMMAND_H

#include <utility>

#include "Table.h"

class Application;

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};


class SaveCommand final : public Command
{
    const Application& ctx;

public:
    explicit SaveCommand(const Application& ctx) : ctx(ctx) {}
    void execute() override;
};

class SaveAsCommand final : public Command
{
    Application& ctx;
    std::string path;

public:
    explicit SaveAsCommand(Application& ctx, std::string path) :
        ctx(ctx),
        path(std::move(path)) {}
    void execute() override;
};

class OpenCommand final : public Command
{
    Application& ctx;
    std::string path;

public:
    explicit OpenCommand(Application& ctx, std::string path) :
        ctx(ctx),
        path(std::move(path)) {}
    void execute() override;
};


class HelpCommand final : public Command
{
    Application& ctx;

public:
    explicit HelpCommand(Application& ctx) : ctx(ctx) {}
    void execute() override;
};


class QuitCommand final : public Command
{
    Application& app;

public:
    explicit QuitCommand(Application& app) : app(app) {}
    void execute() override;
};


class PrintCommand final : public Command
{
    const Application& app;

public:
    explicit PrintCommand(const Application& app) : app(app) {}
    void execute() override;
};


class EditCellCommand final : public Command
{
    Table* table;
    const size_t row;
    const size_t column;
    std::string value;

public:
    explicit EditCellCommand(Table* table, const size_t row, const size_t column, std::string value) :
        table(table),
        row(row),
        column(column),
        value(std::move(value)) {}
    void execute() override;
};


#endif //COMMAND_H
