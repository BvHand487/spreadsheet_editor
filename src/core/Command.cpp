#include "Command.h"

#include <iostream>

#include "Application.h"
#include "CommandParser.h"
#include "Table.h"
#include "TableSerializer.h"
#include "Utility.h"


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
    ctx.setActiveSavePath(path);
    ctx.setActiveTable(table);
}

void HelpCommand::execute() {
    std::cout << "Available commands:\n";

    for (const auto& command: this->ctx.getCommandParser().getRegisteredCommands())
        std::cout << "- " << command << '\n';
}

void QuitCommand::execute() { app.quit(); }

void PrintCommand::execute()
{
    std::ios_base::fmtflags original_flags = std::cout.flags();
    char original_fill = std::cout.fill();

    const Table* table = app.getActiveTable();

    if (table == nullptr)
        throw std::logic_error("There is no active table to print.");

    const size_t rows = table->getRows();
    const size_t cols = table->getCols();
    const TableLayout &layout = app.getLayout();

    for (size_t row = 0; row < rows; row++)
    {
        const size_t height = layout.getRowHeight(row);

        for (size_t line = 0; line < height; line++)
        {
            for (size_t col = 0; col < cols; col++)
            {
                const size_t width = layout.getColWidth(col);

                const Cell* cell = table->getCell(row, col);

                std::string lineInRow;
                if (cell != nullptr)
                {
                    lineInRow = getLine(cell->asString(), line);
                }

                std::cout << std::setw(TableLayout::PADDING_X) << "";
                std::cout << std::left << std::setw(width) << lineInRow;
                std::cout << std::setw(TableLayout::PADDING_X) << "";

                if (col < cols - 1) std::cout << "|";
            }

            std::cout << "\n";
        }
    }

    std::cout.flags(original_flags);
    std::cout.fill(original_fill);
    std::cout << std::flush;
}

void EditCellCommand::execute()
{
    Cell *temp = CellFactory::create_cell_auto(value);
    this->table->setCell(row, column, temp);
}
