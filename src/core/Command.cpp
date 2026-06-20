#include "Command.h"

#include <iostream>

#include "Application.h"
#include "CommandParser.h"
#include "Table.h"
#include "TableSerializer.h"


void SaveCommand::execute()
{
    if (ctx.getActiveTable() == nullptr)
        throw std::logic_error("There is no active table to save.");

    const std::string& filepath = ctx.getActiveSavePath();
    if (filepath.empty())
        throw std::logic_error("There is no saved path.");

    TableSerializer::saveToFile(ctx.getActiveTable(), filepath);
}

void SaveAsCommand::execute()
{
    if (ctx.getActiveTable() == nullptr)
        throw std::logic_error("There is no active table to save.");

    TableSerializer::saveToFile(ctx.getActiveTable(), this->path);

    ctx.setActiveSavePath(this->path);
}

void OpenCommand::execute() {
    Table *table = TableSerializer::loadFromFile(path);
    ctx.setActiveTable(table);
}

void HelpCommand::execute() {
    std::cout << "Available commands:\n";

    for (const auto& command: this->ctx.getCommandParser().getRegisteredCommands())
        std::cout << "- " << command << '\n';
}

// void HistoryCommand::execute()
// {
//     std::cout << "Command history:\n";
//
//     for (const auto& command: this->ctx.getCommandHistory())
//         std::cout << command << '\n';
// }

void QuitCommand::execute() { app.quit(); }

// TODO: fix this mess
void PrintCommand::execute()
{
    std::ios_base::fmtflags original_flags = std::cout.flags();
    char original_fill = std::cout.fill();

    size_t width_array[Table::MAX_COLS] = { 0 };

    for (size_t col = 0; col < table->getCols(); ++col)
    {
        auto columnCells = table->getColumn(col);
        size_t max_length = 0;
        for (size_t row = 0; row < table->getRows(); ++row)
        {
            if (!columnCells[row]) continue;

            const auto length = columnCells[row]->serialize().length();
            if (max_length < length) max_length = length;
        }

        width_array[col] = max_length;
    }

    for (size_t row = 0; row < table->getRows(); ++row)
    {
        for (size_t col = 0; col < table->getCols(); ++col)
        {
            const auto cell = table->getCell(row, col);

            size_t current_width = width_array[col] + 2;
            if (width_array[col] == 0) current_width = 3;

            std::cout << std::right << std::setfill(' ') << std::setw(current_width);

            if (cell) std::cout << cell->serialize();
            else std::cout << " ";

            if (col < 4) std::cout << "|";
        }

        std::cout << "\n";
    }

    std::cout.flags(original_flags);
    std::cout.fill(original_fill);
    std::cout << std::flush;
}

void EditCellCommand::execute()
{
    const Cell *temp = CellFactory::create_cell_auto(value);
    this->table->setCell(row, column, temp);
    delete temp;
}
