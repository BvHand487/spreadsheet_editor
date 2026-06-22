#include <fstream>

#include "TableSerializer.h"
#include "Utility.h"


Table* TableSerializer::loadFromFile(const std::string &filename, char delimiter)
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
                table->setCell(row, col, CellFactory::create_cell_auto(token));
                token.clear();

                col++;
            }
            else
                token += c;
        }

        if (!token.empty())
            table->setCell(row, col, CellFactory::create_cell_auto(token));

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

    file << std::endl;
    file.close();
}
