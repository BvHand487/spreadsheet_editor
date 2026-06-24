#include <fstream>

#include "TableSerializer.h"

#include <iostream>

#include "Utility.h"


Table* TableSerializer::loadFromFile(const std::string &filename, const char delimiter)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open input file " + filename);

    std::string line;
    bool inQuotes = false;

    size_t row = 0, col = 0;
    auto *table = new Table();

    while (std::getline(file, line))
    {
        std::string token;

        for (size_t i = 0; i < line.length(); ++i)
        {
            const char c = line[i];

            if (c == '\"' && (i == 0 || line[i - 1] != '\\'))
                inQuotes = !inQuotes;

            if (c == delimiter && !inQuotes)
            {
                Cell* cell = nullptr;
                try {
                    cell = CellFactory::create_cell_auto(token);
                }
                catch (const std::runtime_error &err) {
                    delete table;
                    throw std::format_error(std::format("Error at R{}C{}: {}", row + 1, col + 1, err.what()));
                }

                table->setCell(row, col, cell);
                token.clear();

                col++;
            }
            else
                token += c;
        }

        if (!token.empty())
        {
            Cell* cell = nullptr;
            try {
                cell = CellFactory::create_cell_auto(token);
            }
            catch (const std::runtime_error &err) {
                delete table;
                throw std::format_error(std::format("Error at R{}C{}: {}", row + 1, col + 1, err.what()));
            }

            table->setCell(row, col, cell);
        }

        row++;
        col = 0;
    }

    file.close();
    return table;
}

void TableSerializer::saveToFile(const Table* table, const std::string &filename, const char delimiter)
{
    std::ofstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Could not open output file " + filename);

    for (size_t row = 0; row < table->getRows(); row++)
    {
        for (size_t col = 0; col < table->getCols(); col++)
        {
            Cell* cell = table->getCell(row, col);
            if (cell != nullptr)
                file << cell->serialize();

            if (col < table->getCols() - 1)
                file << delimiter;
        }

        file << '\n';
    }

    file.close();
}
