#include "doctest.h"
#include "Cell.h"
#include "Utility.h"


TEST_CASE("IntegerCell tests.")
{
    SUBCASE("Clones correctly")
    {
        const IntegerCell cell(5);
        IntegerCell* clone = cell.clone();

        CHECK(clone != nullptr);
        CHECK(cell.getValue() == clone->getValue());

        delete clone;
    }
}

TEST_CASE("DecimalCell tests.")
{
    SUBCASE("Clones correctly")
    {
        const DecimalCell cell(5.0);
        DecimalCell* clone = cell.clone();

        CHECK(clone != nullptr);
        CHECK(cell.getValue() == clone->getValue());

        delete clone;
    }
}

TEST_CASE("StringCell tests.")
{
    SUBCASE("Clones correctly")
    {
        const StringCell cell("Hello, World!");
        StringCell* clone = cell.clone();

        CHECK(clone != nullptr);
        CHECK(cell.asString() == clone->asString());

        delete clone;
    }

    SUBCASE ("Converts to value correctly")
    {
        const StringCell cell1("123");
        const StringCell cell2("123.456");
        const StringCell cell3("-123");
        const StringCell cell4("-123.456");

        CHECK(approx_equals(cell1.asValue(), 123));
        CHECK(approx_equals(cell2.asValue(), 123.456));
        CHECK(approx_equals(cell3.asValue(), -123));
        CHECK(approx_equals(cell4.asValue(), -123.456));
    }
}

TEST_CASE("DateCell tests.")
{
    SUBCASE("Throws on wrong dates.")
    {
        CHECK_THROWS(DateCell(0, 1, 1970));
        CHECK_THROWS(DateCell(1, 0, 1970));

        CHECK_NOTHROW(DateCell(29, 2, 1972)); // leap
        CHECK_THROWS(DateCell(29, 2, 1973)); // not leap
    }

    SUBCASE("Clones correctly")
    {
        const DateCell cell(24, 6, 2026);
        DateCell* clone = cell.clone();

        CHECK(clone != nullptr);
        CHECK(cell.getDay() == clone->getDay());
        CHECK(cell.getMonth() == clone->getMonth());
        CHECK(cell.getYear() == clone->getYear());

        delete clone;
    }

    SUBCASE ("Converts to value correctly")
    {
        const DateCell cell1(1, 1, 1970);
        const DateCell cell2(31, 12, 1969);
        const DateCell cell3(2, 1, 1970);

        CHECK(approx_equals(cell1.asValue(), 0.0));
        CHECK(approx_equals(cell2.asValue(), -1.0));
        CHECK(approx_equals(cell3.asValue(), 1.0));
    }
}

TEST_CASE("FormulaCell tests.")
{

    SUBCASE("Throws on wrong formula")
    {
        CHECK_THROWS(FormulaCell("= R0C1"));  // rows start with 1
        CHECK_THROWS(FormulaCell("= R1C0"));  // cols start with 1
        CHECK_THROWS(FormulaCell("= hello"));
    }

    SUBCASE("Detects division by 0 error.")
    {
        auto cell = FormulaCell("= 1 / 0");

        CHECK(cell.asString() == "ERROR");
        CHECK(cell.isError());
    }

    SUBCASE("Clones correctly")
    {
        const FormulaCell cell("= 1");
        FormulaCell* clone = cell.clone();

        CHECK(clone != nullptr);
        CHECK(cell.asValue() == clone->asValue());
        CHECK(cell.serialize() == clone->serialize());

        delete clone;
    }
}

TEST_CASE("Cell factory tests.")
{
    SUBCASE("Detects correct type.")
    {
        Cell* cell;

        cell = CellFactory::create_cell_auto("123");
        REQUIRE(cell != nullptr);
        CHECK_EQ(typeid (*cell), typeid (IntegerCell));
        delete cell;

        cell = CellFactory::create_cell_auto("\"hello\"");
        REQUIRE(cell != nullptr);
        CHECK_EQ(typeid (*cell), typeid (StringCell));
        delete cell;

        cell = CellFactory::create_cell_auto("123.456");
        REQUIRE(cell != nullptr);
        CHECK_EQ(typeid (*cell), typeid (DecimalCell));
        delete cell;

        cell = CellFactory::create_cell_auto("= 1 + 2 + 3");
        REQUIRE(cell != nullptr);
        CHECK_EQ(typeid (*cell), typeid (FormulaCell));
        delete cell;

        cell = CellFactory::create_cell_auto("24/6/2026");
        REQUIRE(cell != nullptr);
        CHECK_EQ(typeid (*cell), typeid (DateCell));
        delete cell;
    }
}