#include <iostream>

#include "doctest.h"
#include "Table.h"
#include "Cell.h"


TEST_CASE("Table tests.")
{
    SUBCASE("Allocation grows correctly.")
    {
        Table table;

        size_t previousRowsCapacity = table.getRowsCapacity();
        size_t previousColsCapacity = table.getColsCapacity();

        table.setCell(Table::MAX_ROWS, Table::MAX_COLS, nullptr);

        CHECK(table.getRowsCapacity() >= previousRowsCapacity);
        CHECK(table.getColsCapacity() >= previousColsCapacity);
    }

    SUBCASE("Cells get updated.")
    {
        Table table;
        Cell *cell = CellFactory::create_cell_auto("123");

        table.setCell(0, 0, cell);
        Cell* tableCell = table.getCell(0, 0);

        CHECK(tableCell == cell);  // same address
        CHECK_EQ(typeid(*tableCell), typeid(*cell));
        CHECK_EQ(tableCell->asValue(), cell->asValue());

        delete cell;
    }

    SUBCASE("Layout gets updated correctly.")
    {
        Table table;
        TableLayout layout;

        table.subscribe(&layout);

        table.setCell(0, 0, nullptr);

        CHECK(layout.getRowHeights().size() == 1);
        CHECK(layout.getColWidths().size() == 1);
        CHECK(layout.getRowHeight(0) == 1);
        CHECK(layout.getColWidth(0) == 1);

        table.unsubscribe(&layout);
    }
}