#ifndef TABLEFORMATTING_H
#define TABLEFORMATTING_H

#include <vector>

#include "Cell.h"


class Table;

class TableObserver
{
public:
    virtual void onCellChanged(const Table *table, size_t row, size_t col) = 0;
    virtual void onTableCleared() = 0;

    virtual ~TableObserver() = default;
};


// class for the formatting of the table
class TableLayout final : public TableObserver
{
    std::vector<size_t> rowHeights{};
    std::vector<size_t> colWidths{};

public:

    static constexpr size_t PADDING_X = 1;
    static constexpr size_t PADDING_Y = 0;

    static void getLayoutWidthAndHeight(const Cell& cell, size_t &width, size_t &height);

    [[nodiscard]] const std::vector<size_t>& getRowHeights() const { return rowHeights; }
    [[nodiscard]] const std::vector<size_t>& getColWidths() const { return colWidths; }

    [[nodiscard]] size_t getRowHeight(const size_t row) const { return rowHeights[row]; }
    [[nodiscard]] size_t getColWidth(const size_t col) const { return colWidths[col]; }

    void clear();
    void updateFromCell(const Table *table, size_t row, size_t col);
    void updateFromTable(const Table* table);

    void onCellChanged(const Table *table, size_t row, size_t col) override;
    void onTableCleared() override;
};

#endif //TABLEFORMATTING_H
