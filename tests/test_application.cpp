#include "doctest.h"
#include "Application.h"


TEST_CASE("Commands apply correctly.")
{
    Application& app = Application::getInstance();
    Table *table = app.getActiveTable();

    REQUIRE((table->getRowsCapacity() > 0 && table->getColsCapacity() > 0));

    SUBCASE("Edit command modifies a cell.")
    {
        const std::string text = "test";

        app.apply(std::format("edit 1 1 \"{}\"", text));

        Cell* cell = table->getCell(0, 0);
        CHECK(cell != nullptr);
        CHECK(cell->asString() == text);
    }

    {
        const std::string text = "test.txt";

        SUBCASE("Save as command changes save location.")
        {


            app.apply(std::format("save as {}", text));

            CHECK(app.getActiveSavePath() == text);
        }

        SUBCASE("Open command changes save location.")
        {
            app.setActiveSavePath("");

            app.apply(std::format("open {}", text));

            CHECK(app.getActiveSavePath() == text);
        }
    }

    SUBCASE("Quit command stops the app from running.")
    {
        app.apply("quit");
        CHECK(app.isRunning() == false);
    }
}