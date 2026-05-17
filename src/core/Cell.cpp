#include "Cell.h"


IntegerCell* IntegerCell::clone() const
{
    return new IntegerCell(*this);
}

std::string IntegerCell::serialize() const
{
    return std::to_string(this->value);
};

float IntegerCell::asValue() const
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

float StringCell::asValue() const
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

float DateCell::asValue() const
{
    // TODO
    return 0;
}

Cell* CellFactory::create_cell(const Type type, const std::string &text)
{
    switch (type)
    {
        case None: return nullptr;
        case Int: return new IntegerCell(std::stoi(text));
        case String: return new StringCell(text);
        case Date: return new DateCell(1, 1, 1900);
    }

    return nullptr;
}

Cell* CellFactory::create_cell_auto(const std::string &text)
{
    // TODO
};
