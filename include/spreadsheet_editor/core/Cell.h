#ifndef CELL_H
#define CELL_H

#include <cstdint>
#include <string>
#include <utility>

#include "Formula.h"


class Cell {
public:
    [[nodiscard]] virtual Cell* clone() const = 0;
    [[nodiscard]] virtual std::string serialize() const = 0;
    [[nodiscard]] virtual double asValue() const = 0;

    friend std::ostream& operator<< (std::ostream& os, const Cell& cell)
    {
        return os << cell.serialize();
    }

    virtual ~Cell() = default;
};


class IntegerCell final : public Cell {
private:
    const int32_t value = 0;

public:
    IntegerCell(const int32_t value) : value(value) {}
    IntegerCell(const IntegerCell& cell) : value(cell.value) {}

    [[nodiscard]] IntegerCell* clone() const override;
    [[nodiscard]] std::string serialize() const override;
    [[nodiscard]] double asValue() const override;
};


class StringCell final : public Cell {
private:
    const std::string text;

public:
    StringCell(std::string text) : text(std::move(text)) {}
    StringCell(const StringCell& cell) : text(cell.text) {}

    [[nodiscard]] StringCell* clone() const override;
    [[nodiscard]] std::string serialize() const override;
    [[nodiscard]] double asValue() const override;
};


class DateCell final : public Cell {
private:
    const uint8_t day = 0;
    const uint8_t month = 0;
    const uint16_t year = 0;

public:
    DateCell(
        const uint8_t day,
        const uint8_t month,
        const uint16_t year) : day(day), month(month), year(year) {}

    DateCell(const DateCell& cell) : day(cell.day), month(cell.month), year(cell.year) {}

    [[nodiscard]] DateCell* clone() const override;
    [[nodiscard]] std::string serialize() const override;
    [[nodiscard]] double asValue() const override;
};


class FormulaCell final : public Cell {
    const FormulaASTNode* formula = nullptr;
    mutable double cachedValue = 0;
    mutable bool dirty = true;

    void refresh() const
    {
        cachedValue = formula->evaluate();
        dirty = false;
    }

public:
    explicit FormulaCell(const FormulaASTNode* formula) : formula(formula) {}
    FormulaCell(const FormulaCell& cell) : formula(cell.formula) {}

    bool isDirty() const { return dirty == true; }

    [[nodiscard]] FormulaCell* clone() const override;
    [[nodiscard]] std::string serialize() const override;
    [[nodiscard]] double asValue() const override;
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
