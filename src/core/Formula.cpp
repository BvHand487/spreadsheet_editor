#include <stdexcept>

#include "Application.h"
#include "Formula.h"


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

double FormulaOperator::evaluate() const
{
    if (left == nullptr || right == nullptr)
        throw std::runtime_error("Couldn't evaluate formula because of a null operand.");

    switch (type)
    {
        case Type::ADD:
            return left->evaluate() + right->evaluate();
        case Type::SUB:
            return left->evaluate() - right->evaluate();
        case Type::MUL:
            return left->evaluate() * right->evaluate();
        case Type::DIV:
            return left->evaluate() / right->evaluate();
        case Type::EQ:
            return left->evaluate() == right->evaluate();
        case Type::NEQ:
            return left->evaluate() != right->evaluate();
        case Type::LT:
            return left->evaluate() < right->evaluate();
        case Type::GT:
            return left->evaluate() > right->evaluate();
        case Type::LTE:
            return left->evaluate() <= right->evaluate();
        case Type::GTE:
            return left->evaluate() >= right->evaluate();

        default:
            throw std::runtime_error("Couldn't evaluate formula because of a invalid operation.");
    }
}

double FormulaCellReference::evaluate() const
{
    return Application::getInstance().getActiveTable()->getCell(row, column)->asValue();
}
