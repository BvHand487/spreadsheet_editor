#include <format>

#include "Cell.h"
#include "FormulaParser.h"
#include "Utility.h"


IntegerCell* IntegerCell::clone() const
{
    return new IntegerCell(*this);
}

std::string IntegerCell::serialize() const
{
    return std::to_string(this->value);
};

double IntegerCell::asValue() const
{
    return static_cast<float>(value);
};


StringCell * StringCell::clone() const
{
    return new StringCell(*this);
}

std::string StringCell::serialize() const
{
    return this->text;
};

double StringCell::asValue() const
{
    // TODO
    return 0;
};


DateCell* DateCell::clone() const
{
    return new DateCell(*this);
}

std::string DateCell::serialize() const
{
    return std::format("{}/{}/{}", this->day, this->month, this->year);
};

double DateCell::asValue() const
{
    // TODO
    return 0;
}

FormulaCell* FormulaCell::clone() const
{
    return new FormulaCell(*this);
}

std::string FormulaCell::serialize() const
{
    if (isDirty()) refresh();
    return std::format("{:.2f}", cachedValue);
}

double FormulaCell::asValue() const
{
    if (isDirty()) refresh();
    return cachedValue;
}

Cell* CellFactory::create_cell(const Type type, const std::string &text)
{
    switch (type)
    {
        case None: return nullptr;
        case Int: return new IntegerCell(std::stoi(text));
        case String: return new StringCell(text);
        case Date: return new DateCell(1, 1, 1900);  // TODO
        case Formula: return new FormulaCell(FormulaParser::parseFormula(text));
    }

    return nullptr;
}

Cell* CellFactory::create_cell_auto(const std::string &text)
{
    if (text.empty()) return nullptr;

    try
    {
        size_t index = 0;
        std::stoi(text, &index);

        if (text.size() - 1 == index)
            return create_cell(Type::Int, text);
    }
    catch (...) { };

    if (text.front() == '\"') return create_cell(Type::String, unescape(text));
    // TODO: date...
    if (text.front() == '=') return create_cell(Type::Formula, text);

    return nullptr;
};
