#include <iostream>

#include "Application.h"
#include "Cell.h"


int main()
{
    // These 2 lines are enough to run CLI
    // const Application& app = Application::getInstance();
    // app.run();

    const Application& app = Application::getInstance();

    std::cout << "All types in 1 table:\n";
    app.apply("edit 1 1 10");
    app.apply("edit 1 2 \"Hello, World!\"");
    app.apply("edit 1 3 123.56");
    app.apply("edit 2 1 \"123\"");
    app.apply("edit 2 2 24/06/2026");
    app.apply("edit 2 3 =R1C1 + R1C2 + R1C3");
    app.apply("print");

    std::cout << "Saving and then opening file:\n";

    app.apply("save as table.txt");

    std::cout << "After changes:\n";
    app.apply("edit 1 1 1");
    app.apply("edit 1 2 2");
    app.apply("edit 1 3 3");
    app.apply("print");

    std::cout << "After opening table.txt:\n";
    app.apply("open table.txt");
    app.apply("print");

    std::cout << "Formulas:\n";
    app.apply("clear");

    app.apply("edit 1 1 =10+10");
    app.apply("edit 2 1 =R1C1 + R1C1");
    app.apply("edit 3 1 =R1C1 * R2C1");
    app.apply("edit 4 1 =R1C1 == R2C1");
    app.apply("edit 5 1 =R1C1 <= R2C1");
    app.apply("edit 6 1 =R3C1 >= R2C1");
    app.apply("edit 7 1 =10 / 0");
    app.apply("edit 7 7 =50");
    app.apply("print");

    app.apply("save");  //saves to table.txt

    return 0;
}
