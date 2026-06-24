#ifndef TABLE_H
#define TABLE_H

#include <vector>

#include "Cell.h"
#include "TableFormatting.h"


class Table
{
    size_t rows = 0, cols = 0;
    size_t rowsCapacity = 0, colsCapacity = 0;
    std::vector<Cell *> cells{};

    size_t globalVersion = 0;

    std::vector<TableObserver *> listeners{};

    // reserves memory for the cells with new row/col capacity
    void reserve(size_t newRowCapacity, size_t newColCapacity);
    void ensureCapacity(size_t row, size_t col);

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
    [[nodiscard]] size_t getRowsCapacity() const { return rowsCapacity; };
    [[nodiscard]] size_t getCols() const { return cols; };
    [[nodiscard]] size_t getColsCapacity() const { return colsCapacity; };

    void clear();
    void setCell(size_t row, size_t col, Cell *cell);
    [[nodiscard]] Cell* getCell(size_t row, size_t col) const;

    [[nodiscard]] std::vector<Cell*> getRow(size_t row) const;
    [[nodiscard]] std::vector<Cell*> getColumn(size_t col) const;

    [[nodiscard]] size_t getVersion() const { return globalVersion; };
    void incrementVersion() { globalVersion++; };
    void decrementVersion() { globalVersion--; };

    void subscribe(TableObserver *listener);
    void unsubscribe(const TableObserver *listener);

    void notifyOnCellChanged(size_t row, size_t col) const;
    void notifyOnTableCleared() const;

    ~Table();
};


#endif //TABLE_H
