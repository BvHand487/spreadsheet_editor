#ifndef FORMULA_H
#define FORMULA_H

#include <stdexcept>


class Application;

class FormulaASTNode {
public:
    [[nodiscard]] virtual double evaluate() const = 0;
    virtual ~FormulaASTNode() = default;
};

class FormulaOperand : public FormulaASTNode {};

class FormulaOperator final : public FormulaASTNode
{
public:
    enum class Type
    {
        None,
        ADD,
        SUB,
        MUL,
        DIV,
        EQ,
        NEQ,
        LT,
        GT,
        LTE,
        GTE
    };

    static int getPrecedence(Type type);

private:
    FormulaASTNode *left, *right;
    Type type = Type::None;

public:
    FormulaOperator(FormulaASTNode *left, FormulaASTNode *right, const Type type) : left(left), right(right), type(type) {}

    [[nodiscard]] Type getType() const { return type; }
    [[nodiscard]] FormulaASTNode *getLeft() const { return left; }
    [[nodiscard]] FormulaASTNode *getRight() const { return right; }

    [[nodiscard]] double evaluate() const override;

    ~FormulaOperator() override
    {
        delete left;
        delete right;
    };
};

class FormulaCellReference final : public FormulaOperand
{
    size_t row{};
    size_t column{};

public:
    [[nodiscard]] double evaluate() const override;

    FormulaCellReference(const size_t row, const size_t col) : row(row), column(col) {}
};

class FormulaNumberLiteral final : public FormulaOperand
{
    double value{};

public:
    [[nodiscard]] double evaluate() const override { return value; }

    explicit FormulaNumberLiteral(const double value) : value(value) {}
};

#endif //FORMULA_H
