#include "Cell.h"
#include "Utility.h"


IntegerCell* IntegerCell::clone() const
{
    return new IntegerCell(*this);
}

double IntegerCell::asValue() const
{
    return value;
};

std::string IntegerCell::asString() const
{
    return std::to_string(this->value);
}

std::string IntegerCell::serialize() const
{
    return std::to_string(this->value);
};


DecimalCell* DecimalCell::clone() const
{
    return new DecimalCell(*this);
}

double DecimalCell::asValue() const
{
    return value;
};

std::string DecimalCell::asString() const
{
    return std::format("{:.2f}", value);
}

std::string DecimalCell::serialize() const
{
    return std::to_string(value);
};


StringCell* StringCell::clone() const
{
    return new StringCell(*this);
}

double StringCell::asValue() const
{
    if (text.empty()) return 0.0;

    try
    {
        size_t idx = 0;
        const double result = std::stod(text, &idx);

        if (idx == text.length())
            return result;
    }
    catch (const std::exception&) {}

    return 0.0;
};

std::string StringCell::asString() const
{
    return this->text;
};

std::string StringCell::serialize() const
{
    return escape(this->text);
};


DateCell::DateCell(
    const uint8_t day,
    const uint8_t month,
    const uint16_t year) : date(std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day))
{
    if (!date.ok())
        throw std::invalid_argument("Invalid date.");
}

DateCell* DateCell::clone() const
{
    return new DateCell(*this);
}

double DateCell::asValue() const
{
    const auto days_diff = std::chrono::sys_days(date) - std::chrono::sys_days(REFERENCE_DATE);

    return static_cast<double>(days_diff.count());
}

std::string DateCell::asString() const
{
    return std::format("{:%d/%m/%Y}", date);
}

std::string DateCell::serialize() const
{
    return std::format("{:%d/%m/%Y}", date);
};


FormulaCell* FormulaCell::clone() const
{
    return new FormulaCell(*this);
}

double FormulaCell::asValue() const
{
    if (isDirty())
        refresh();

    return cachedValue;
}

std::string FormulaCell::asString() const
{
    if (isDirty())
        refresh();

    if (isError())
        return "ERROR";

    return std::format("{:.2f}", cachedValue);
}

std::string FormulaCell::serialize() const
{
    return this->representation;
}


Cell* CellFactory::create_cell_auto(const std::string &text)
{
    std::string trimmed = trim(text);

    if (trimmed.empty())
        return nullptr;

    // try formula
    if (trimmed.front() == '=') return create_cell<FormulaCell>(trimmed);

    // try string
    if (trimmed.front() == '\"' && trimmed.back() == '\"')
    {
        if (trimmed.length() > 2)
            return create_cell<StringCell>(unescape(trimmed));
        else
            return nullptr;  // this allows the cell to be cleared by - edit <row> <col> ""
    }

    // try date
    if (trimmed.find('/') != std::string::npos)
    {
        try
        {
            auto [day, month, year] = parse_date_numbers(trimmed, '/');
            return create_cell<DateCell>(day, month, year);
        }
        catch (const std::invalid_argument& _)
        {
            throw;
        }
    }

    // try number
    if ((trimmed.front() == '+' || trimmed.front() == '-') || std::isdigit(trimmed.front()))
    {
        try
        {
            size_t index = 0;
            const double value = std::stod(trimmed, &index);

            // valid number
            if (trimmed.length() == index)
            {
                if (trimmed.find('.') != std::string::npos)
                    return create_cell<DecimalCell>(value);

                return create_cell<IntegerCell>(std::stoi(trimmed));
            }
        }
        catch (const std::out_of_range &) { throw std::invalid_argument("Number is too large for it's data type."); }
        catch (const std::invalid_argument&) { throw std::invalid_argument("Couldn't parse number."); }
    }

    throw std::invalid_argument(std::format("Couldn't deduce the type of \'{}\'", trimmed));
};
