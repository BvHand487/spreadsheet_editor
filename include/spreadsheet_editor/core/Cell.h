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

    [[nodiscard]] int32_t getValue() const { return value; }

    [[nodiscard]] IntegerCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class DecimalCell final : public Cell {
    const double value = 0;

public:
    explicit DecimalCell(const double value) : value(value) {}

    [[nodiscard]] double getValue() const { return value; }

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

    [[nodiscard]] uint8_t getDay() const { return static_cast<unsigned>(date.day()); };
    [[nodiscard]] uint8_t getMonth() const { return static_cast<unsigned>(date.month()); };
    [[nodiscard]] uint16_t getYear() const { return static_cast<signed>(date.year()); };

    [[nodiscard]] DateCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;
};


class FormulaCell final : public Cell {
    std::string representation{};
    const FormulaASTNode* formula = nullptr;

    mutable size_t lastVersion = -1;
    mutable double cachedValue = 0;
    mutable bool isError_ = false;
    mutable bool isEvaluating_ = false;

public:
    explicit FormulaCell(const std::string& text) :
        representation(text),
        formula(FormulaParser::parseFormula(text)) {}

    FormulaCell(const FormulaCell &obj) :
        representation(obj.representation),
        formula(obj.formula->clone()),
        lastVersion(obj.lastVersion),
        cachedValue(obj.cachedValue),
        isError_(obj.isError_) {}

    FormulaCell& operator=(const FormulaCell& obj);

    bool isError() const { return isError_; }
    bool isEvaluating() const { return isEvaluating_; }

    [[nodiscard]] FormulaCell* clone() const override;
    [[nodiscard]] double asValue() const override;
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] std::string serialize() const override;

    ~FormulaCell() override { delete formula; }
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
