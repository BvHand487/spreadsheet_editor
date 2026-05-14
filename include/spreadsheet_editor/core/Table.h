#ifndef TABLE_H
#define TABLE_H

#include <chrono>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

#include "Cell.h"

class Table
{
    static constexpr size_t DEFAULT_ROWS = 100;
    static constexpr size_t DEFAULT_COLS = 100;

    std::vector<Cell *> cells;
    size_t rows = 0, cols = 0;

public:
    Table();
    Table(size_t rows, size_t cols);

    [[nodiscard]] size_t getRows() const { return rows; };
    [[nodiscard]] size_t getCols() const { return cols; };

    void setCell(size_t row, size_t col, const Cell *cell);
    [[nodiscard]] Cell* getCell(size_t row, size_t col) const;

    [[nodiscard]] std::vector<Cell*> getRow(size_t row) const;
    [[nodiscard]] std::vector<Cell*> getColumn(size_t col) const;

    ~Table();
};

#endif //TABLE_H
