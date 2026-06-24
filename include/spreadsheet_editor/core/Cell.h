#ifndef CELL_H
#define CELL_H

#include <chrono>
#include <string>

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


class DecimalCell final : public Cell {
    const double value = 0;

public:
    explicit DecimalCell(const double value) : value(value) {}

    [[nodiscard]] DecimalCell* clone() const override;
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
    const std::chrono::year_month_day date;

public:

    static constexpr auto REFERENCE_DATE = std::chrono::year(1970) / std::chrono::month(1) / std::chrono::day(1);

    DateCell(
        uint8_t day,
        uint8_t month,
        uint16_t year);

    [[nodiscard]] DateCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class FormulaCell final : public Cell {
    std::string representation{};
    const FormulaASTNode* formula = nullptr;

    mutable double cachedValue = 0;
    mutable bool isDirty_ = true;
    mutable bool isError_ = false;

    void refresh() const
    {
        isDirty_ = false;
        isError_ = false;

        try {
            cachedValue = formula->evaluate();
        }
        catch (const formula_evaluation_error&) {
            isError_ = true;
        }
    }

public:
    explicit FormulaCell(const std::string& text) :
        representation(text),
        formula(FormulaParser::parseFormula(text)) {}
    bool isDirty() const { return isDirty_; }
    bool isError() const { return isError_; }

    [[nodiscard]] FormulaCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class CellFactory
{
public:

    template<class CellType, class... Args>
    static Cell* create_cell(Args&&... fields)
    {
        return new CellType(std::forward<Args>(fields)...);
    }

    static Cell* create_cell_auto(const std::string& text);
};


#endif //CELL_H
