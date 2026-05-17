#include "Table.h"

Table::Table()
{
    this->rows = DEFAULT_ROWS;
    this->cols = DEFAULT_COLS;

    cells = std::vector<Cell*>();
    cells.resize(rows * cols);
}

Table::Table(const size_t rows, const size_t cols)
{
    this->rows = rows;
    this->cols = cols;

    cells = std::vector<Cell*>();
    cells.resize(rows * cols);
}

void Table::setCell(const size_t row, const size_t col, const Cell *cell)
{
    if (row >= rows) throw std::out_of_range("Row index is out of range.");
    if (col >= cols) throw std::out_of_range("Column index is out of range.");

    const size_t cell_index = rows * col + row;

    delete cells[cell_index];
    cells[cell_index] = cell->clone();
}

Cell* Table::getCell(const size_t row, const size_t col) const
{
    return cells[rows * col + row];
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

Table::~Table()
{
    for (size_t col = 0; col < cols; col++)
        for (size_t row = 0; row < rows; row++)
            delete cells[row * cols + col];
}
