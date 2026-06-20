#ifndef TABLESERIALIZER_H
#define TABLESERIALIZER_H

#include "Table.h"

class TableSerializer
{
public:
    static Table* loadFromFile(const std::string& filename, char delimiter = ',');
    static void saveToFile(const Table* table, const std::string& filename, char delimiter = ',');
};

#endif //TABLESERIALIZER_H
