#include "Application.h"
#include "Cell.h"

int main()
{
    const Application& app = Application::getInstance();

    app.apply("edit 1 1 =r1c3");
    app.apply("edit 1 3 =r1c2");
    app.apply("edit 1 2 =r1c1");

    app.run();

    return 0;
}
