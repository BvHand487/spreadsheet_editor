#include "Table.h"
#include "TableFormatting.h"


void TableLayout::getLayoutWidthAndHeight(const Cell &cell, size_t &width, size_t &height)
{
    const std::string representation = cell.asString();

    if (representation.empty()) {
        width = 1;
        height = 1;
        return;
    }

    size_t currentWidth = 0, maxWidth = 0, lineCount = 1;
    for (const char c : representation)
    {
        if (c == '\n')
        {
            ++lineCount;
            maxWidth = std::max(maxWidth, currentWidth);
            currentWidth = 0;
        }
        else
        {
            ++currentWidth;
        }
    }

    height = lineCount;
    width = std::max(maxWidth, currentWidth);
}

void TableLayout::clear()
{
    rowHeights.clear();
    colWidths.clear();
}

void TableLayout::updateFromCell(const size_t row, const size_t col, const Cell *cell)
{
    size_t width = 1, height = 1;

    if (cell != nullptr)
        getLayoutWidthAndHeight(*cell, width, height);

    if (row >= rowHeights.size()) rowHeights.resize(row + 1, 1);
    if (col >= colWidths.size()) colWidths.resize(col + 1, 1);

    rowHeights[row] = std::max(rowHeights[row], height);
    colWidths[col] = std::max(colWidths[col], width);
}


void TableLayout::updateFromTable(const Table *table)
{
    this->clear();

    if (table == nullptr)
        return;

    for (size_t r = 0; r < table->getRows(); ++r)
    {
        for (size_t c = 0; c < table->getCols(); ++c)
        {
            const Cell* cell = table->getCell(r, c);
            if (cell != nullptr)
                updateFromCell(r, c, cell);
        }
    }
}

void TableLayout::onCellChanged(const size_t row, const size_t col, const Cell *cell)
{
    updateFromCell(row, col, cell);
}

void TableLayout::onTableCleared()
{
    this->clear();
}
