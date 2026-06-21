#include <format>

#include "Cell.h"
#include "FormulaParser.h"
#include "Utility.h"


IntegerCell* IntegerCell::clone() const
{
    return new IntegerCell(*this);
}

double IntegerCell::asValue() const
{
    return static_cast<float>(value);
};

std::string IntegerCell::asString() const
{
    return std::to_string(this->value);
}

std::string IntegerCell::serialize() const
{
    return std::to_string(this->value);
};


StringCell* StringCell::clone() const
{
    return new StringCell(*this);
}

double StringCell::asValue() const
{
    // TODO
    return 0;
};

std::string StringCell::asString() const
{
    return this->text;
};

std::string StringCell::serialize() const
{
    return escape(this->text);
};


DateCell* DateCell::clone() const
{
    return new DateCell(*this);
}

double DateCell::asValue() const
{
    // TODO
    return 0;
}

std::string DateCell::asString() const
{
    return std::format("{}-{}-{}", day, month, year);
}

std::string DateCell::serialize() const
{
    return std::format("{}-{}-{}", day, month, year);
};


FormulaCell* FormulaCell::clone() const
{
    return new FormulaCell(*this);
}

double FormulaCell::asValue() const
{
    if (isDirty()) refresh();
    return cachedValue;
}

std::string FormulaCell::asString() const
{
    if (isDirty()) refresh();
    return std::format("{:.2f}", cachedValue);
}

std::string FormulaCell::serialize() const
{
    return this->representation;
}

Cell* CellFactory::create_cell(const Type type, const std::string &text)
{
    switch (type)
    {
        case None: return nullptr;
        case Int: return new IntegerCell(std::stoi(text));
        case String: return new StringCell(text);
        case Date: return new DateCell(1, 1, 1900);  // TODO
        case Formula: return new FormulaCell(text);
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

        if (text.size() == index)
            return create_cell(Int, text);
    }
    catch (const std::invalid_argument& _) { };

    if (text.front() == '\"' && text.back() == '\"') return create_cell(String, unescape(text));
    // TODO: date...
    if (text.front() == '=') return create_cell(Formula, text);

    return nullptr;
};
