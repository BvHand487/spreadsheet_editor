#include <stdexcept>

#include "Application.h"
#include "Exceptions.h"
#include "Formula.h"
#include "Utility.h"



int FormulaOperator::getPrecedence(const Type type)
{
    switch (type)
    {
        case Type::MUL:
        case Type::DIV:
            return 3;

        case Type::ADD:
        case Type::SUB:
            return 2;

        case Type::EQ:
        case Type::NEQ:
        case Type::LT:
        case Type::GT:
        case Type::LTE:
        case Type::GTE:
            return 1;

        default:
            return 0;
    }
}

FormulaOperator::FormulaOperator(const FormulaOperator &obj) :
    left(obj.left ? obj.left->clone() : nullptr),
    right(obj.right ? obj.right->clone() : nullptr),
    type(obj.type)
{
}

FormulaOperator& FormulaOperator::operator=(const FormulaOperator &obj)
{
    if (this != &obj)
    {
        FormulaASTNode *newLeft = obj.left ? obj.left->clone() : nullptr;
        FormulaASTNode *newRight = obj.right ? obj.right->clone() : nullptr;

        delete left;
        delete right;
        type = obj.type;
        left = newLeft;
        right = newRight;
    }

    return *this;
}

double FormulaOperator::evaluate() const
{
    if (left == nullptr || right == nullptr)
        throw std::runtime_error("Couldn't evaluate formula because of a null operand.");

    const double lhs = left->evaluate();
    const double rhs = right->evaluate();

    switch (type)
    {
        case Type::ADD:
            return lhs + rhs;

        case Type::SUB:
            return lhs - rhs;

        case Type::MUL:
            return lhs * rhs;

        case Type::DIV:
            if (approx_equals(rhs, 0.0))
                throw formula_division_by_zero_error("Division by zero while evaluating formula.");
            return lhs / rhs;

        case Type::EQ:
            return approx_equals(lhs, rhs);

        case Type::NEQ:
            return !approx_equals(lhs, rhs);

        case Type::LT:
            return lhs < rhs;

        case Type::GT:
            return lhs > rhs;

        case Type::LTE:
            return lhs <= rhs;

        case Type::GTE:
            return lhs >= rhs;

        default:
            throw std::runtime_error("Couldn't evaluate formula because of a invalid operation.");
    }
}

FormulaOperator* FormulaOperator::clone() const
{
    return new FormulaOperator(*this);
}

double FormulaCellReference::evaluate() const
{
    const Cell* cell = Application::getInstance().getActiveTable()->getCell(row, column);

    double result = 0.0;
    if (cell != nullptr)
        result = cell->asValue();

    return result;
}

FormulaCellReference* FormulaCellReference::clone() const
{
    return new FormulaCellReference(*this);
}

FormulaNumberLiteral* FormulaNumberLiteral::clone() const
{
    return new FormulaNumberLiteral(*this);
}
