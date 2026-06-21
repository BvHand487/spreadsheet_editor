#ifndef CELL_H
#define CELL_H

#include <cstdint>
#include <string>
#include <utility>

#include "Formula.h"
#include "FormulaParser.h"


class Cell {
public:
    [[nodiscard]] virtual Cell* clone() const = 0;
    [[nodiscard]] virtual double asValue() const = 0;
    [[nodiscard]] virtual std::string asString() const = 0;
    [[nodiscard]] virtual std::string serialize() const = 0;

    virtual ~Cell() = default;
};


class IntegerCell final : public Cell {
    const int32_t value = 0;

public:
    explicit IntegerCell(const int32_t value) : value(value) {}

    [[nodiscard]] IntegerCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class StringCell final : public Cell {
    const std::string text;

public:
    explicit StringCell(std::string text) : text(std::move(text)) {}

    [[nodiscard]] StringCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class DateCell final : public Cell {
    const uint8_t day = 0;
    const uint8_t month = 0;
    const uint16_t year = 0;

public:
    DateCell(
        const uint8_t day,
        const uint8_t month,
        const uint16_t year) : day(day), month(month), year(year) {}

    [[nodiscard]] DateCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class FormulaCell final : public Cell {
    std::string representation{};
    const FormulaASTNode* formula = nullptr;

    mutable double cachedValue = 0;
    mutable bool dirty = true;

    void refresh() const
    {
        cachedValue = formula->evaluate();
        dirty = false;
    }

public:
    explicit FormulaCell(const std::string& text) :
        representation(text),
        formula(FormulaParser::parseFormula(text)) {}
    bool isDirty() const { return dirty == true; }

    [[nodiscard]] FormulaCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class CellFactory
{
public:
    enum Type
    {
        None,
        Int,
        String,
        Date,
        Formula,
    };

    static Cell* create_cell(Type type, const std::string& text);
    static Cell* create_cell_auto(const std::string& text);
};


#endif //CELL_H
