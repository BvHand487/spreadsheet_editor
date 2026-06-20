#include <fstream>

#include "TableSerializer.h"
#include "Utility.h"


Table* TableSerializer::loadFromFile(const std::string &filename, char delimiter)
{
    // ...
    return nullptr;
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
                file << *cell;

            if (col < table->getCols() - 1)
                file << delimiter;
        }

        file << '\n';
    }

    file << std::endl;
    file.close();
}
