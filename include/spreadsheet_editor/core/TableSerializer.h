#ifndef TABLESERIALIZER_H
#define TABLESERIALIZER_H

#include "Table.h"

class TableSerializer
{
public:
    static Table* loadFromFile(const std::string& file_name, char delimiter = ',');
    static void saveToFile(Table* table, const std::string& file_name, char delimiter = ',');
};

#endif //TABLESERIALIZER_H
