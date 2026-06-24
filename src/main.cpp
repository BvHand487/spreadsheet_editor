#include "Application.h"
#include "Table.h"
#include "Cell.h"

int main()
{
    const Application& app = Application::getInstance();
    Table* table = app.getActiveTable();

    table->setCell(0, 0, CellFactory::create_cell_auto("1"));
    table->setCell(0, 2, CellFactory::create_cell_auto("= R1C1 + R1C2"));

    app.run();

    return 0;
}
