#ifndef TABLE_H
#define TABLE_H

#include <cmath>
#include <string>
#include <vector>

#include "Cell.h"
#include "TableFormatting.h"


class Table
{
    size_t rows = 0, cols = 0;
    size_t rowsCapacity = 0, colsCapacity = 0;
    std::vector<Cell *> cells{};

    std::vector<TableObserver *> listeners{};

    // reserves memory for the cells with new row/col capacity
    void reserve(size_t newRowCapacity, size_t newColCapacity);

public:
    static constexpr size_t MAX_ROWS = 50;
    static constexpr size_t MAX_COLS = 50;
    static constexpr void validate_coords(size_t row, size_t col);

    Table();
    Table(size_t rows, size_t cols);
    Table(Table &&table) = delete;
    Table(const Table &table) = delete;
    Table& operator= (Table &&table) noexcept = delete;
    Table& operator= (const Table &table) = delete;

    [[nodiscard]] size_t getRows() const { return rows; };
    [[nodiscard]] size_t getCols() const { return cols; };

    void setCell(size_t row, size_t col, const Cell *cell);
    [[nodiscard]] Cell* getCell(size_t row, size_t col) const;

    [[nodiscard]] std::vector<Cell*> getRow(size_t row) const;
    [[nodiscard]] std::vector<Cell*> getColumn(size_t col) const;

    void subscribe(TableObserver *listener);
    void unsubscribe(const TableObserver *listener);

    ~Table();
};


#endif //TABLE_H
