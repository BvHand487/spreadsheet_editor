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

void TableLayout::updateFromCell(const Table *table, const size_t row, const size_t col)
{
    if (row >= rowHeights.size()) rowHeights.resize(row + 1, 1);
    if (col >= colWidths.size()) colWidths.resize(col + 1, 1);

    colWidths[col] = 1;
    rowHeights[row] = 1;

    size_t width = 1, height = 1;

    for (size_t r = 0; r < table->getRows(); ++r)
    {
        if (const Cell* cell = table->getCell(r, col))
        {
            getLayoutWidthAndHeight(*cell, width, height);
            colWidths[col] = std::max(colWidths[col], width);
        }
    }

    for (size_t c = 0; c < table->getCols(); ++c)
    {
        if (const Cell* cell = table->getCell(row, c))
        {
            getLayoutWidthAndHeight(*cell, width, height);
            rowHeights[row] = std::max(rowHeights[row], height);
        }
    }
}


void TableLayout::updateFromTable(const Table *table)
{
    if (table == nullptr) return;

    this->clear();

    rowHeights.resize(table->getRows(), 1);
    colWidths.resize(table->getCols(), 1);

    for (size_t r = 0; r < table->getRows(); ++r)
    {
        for (size_t c = 0; c < table->getCols(); ++c)
        {
            if (const Cell* cell = table->getCell(r, c))
            {
                size_t width = 1, height = 1;
                getLayoutWidthAndHeight(*cell, width, height);

                colWidths[c] = std::max(colWidths[c], width);
                rowHeights[r] = std::max(rowHeights[r], height);
            }
        }
    }
}

void TableLayout::onCellChanged(const Table *table, const size_t row, const size_t col)
{
    updateFromCell(table, row, col);
}

void TableLayout::onTableCleared()
{
    this->clear();
}
