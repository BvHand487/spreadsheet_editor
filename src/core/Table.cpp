#include "Table.h"

#include <stdexcept>


constexpr void Table::validate_coords(const size_t row, const size_t col)
{
    if (row > MAX_ROWS) throw std::logic_error("Row exceeds the maximum number of rows.");
    if (col > MAX_COLS) throw std::logic_error("Column exceeds the maximum number of cols.");
}

void Table::reserve(const size_t newRowCapacity, const size_t newColCapacity)
{
    const size_t actualRows = std::max(rowsCapacity, newRowCapacity);
    const size_t actualCols = std::max(colsCapacity, newColCapacity);

    // no need to continue
    if (actualRows <= rowsCapacity && actualCols <= colsCapacity)
        return;

    std::vector<Cell *> newCells(actualRows * actualCols, nullptr);

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            const size_t oldIndex = col * rowsCapacity + row;
            const size_t newIndex = col * actualRows + row;

            newCells[newIndex] = cells[oldIndex];
        }
    }

    cells = std::move(newCells);
    rowsCapacity = actualRows;
    colsCapacity = actualCols;
}

Table::Table()
{
    // reserve space for a 5x5 table by default
    reserve(5, 5);
}

Table::Table(const size_t rows, const size_t cols)
{
    validate_coords(rows, cols);
    reserve(rows, cols);
}

// takes ownership of cell
void Table::setCell(const size_t row, const size_t col, Cell *cell)
{
    validate_coords(row, col);

    // if we need to grow
    if (row >= rowsCapacity || col >= colsCapacity)
    {
        const size_t targetRows = std::min(
            std::max(rowsCapacity * 2, row + 1),
            MAX_ROWS
        );

        const size_t targetCols = std::min(
            std::max(colsCapacity * 2, col + 1),
            MAX_COLS
        );

        reserve(targetRows, targetCols);
    }

    this->rows = std::max(this->rows, row + 1);
    this->cols = std::max(this->cols, col + 1);

    const size_t cell_index = rowsCapacity * col + row;

    delete cells[cell_index];
    cells[cell_index] = cell;

    // notify onCellChange
    for (const auto& listener : listeners)
        if (listener)
            listener->onCellChange(row, col, cell);
}

Cell* Table::getCell(const size_t row, const size_t col) const
{
    if (row >= rows || col >= cols)
        return nullptr;

    return cells[rowsCapacity * col + row];
}

std::vector<Cell*> Table::getRow(const size_t row) const
{
    std::vector<Cell*> rowCells;

    for (size_t col = 0; col < cols; col++)
    {
        auto cellPtr = getCell(row, col);
        rowCells.push_back(cellPtr);
    }

    return rowCells;
}

std::vector<Cell*> Table::getColumn(const size_t col) const
{
    std::vector<Cell*> colCells;

    for (size_t row = 0; row < rows; row++)
        colCells.push_back(getCell(row, col));

    return colCells;
}


void Table::subscribe(TableObserver *listener)
{
    if (listener)
        listeners.push_back(listener);
}

void Table::unsubscribe(const TableObserver *listener)
{
    if (listener)
    {
        for (auto iter = listeners.begin(); iter != listeners.end();)
        {
            if (*iter == listener)
                listeners.erase(iter);
            else
                ++iter;
        }
    }
}


Table::~Table()
{
    for (size_t col = 0; col < cols; col++)
        for (size_t row = 0; row < rows; row++)
            delete cells[rowsCapacity * cols + col];
}
