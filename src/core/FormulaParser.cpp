#include "FormulaParser.h"

#include <stdexcept>
#include <vector>


char FormulaParser::peek() const
{
    if (!isValid())
        return 0;

    return source.at(position);
}

bool FormulaParser::consume(const char symbol)
{
    if (peek() == symbol)
    {
        position++;
        return true;
    }

    return false;
}

bool FormulaParser::consumeWhitespaces()
{
    while (std::isspace(peek()))
        position++;

    return true;
}

bool FormulaParser::consumeSign(bool &outSign)
{
    // get sign in-front of number
    bool isPositive = true;

    bool processing = true;
    while (isValid() && processing)
    {
        const bool plus = consume('+');
        const bool minus = consume('-');

        if (minus) isPositive = !isPositive;

        processing = plus || minus;
    }

    outSign = isPositive;
    return true;
}

bool FormulaParser::consumeInt(int32_t &out)
{
    bool isPositive = true;
    consumeSign(isPositive);

    uint32_t number = 0;
    if (!consumeUInt(number))
        return false;

    out = static_cast<int32_t>(number);
    out = isPositive ? out : -out;

    return true;
}

bool FormulaParser::consumeUInt(uint32_t &out)
{
    if (!std::isdigit(peek()))
        return false;

    int32_t number = 0;
    while (std::isdigit(peek()))
    {
        const int digit = peek() - '0';
        number = number * 10 + digit;

        position++;
    }

    out = number;
    return true;
}

bool FormulaParser::consumeDouble(double& out)
{
    if (!std::isdigit(peek()))
        return false;

    bool isPositive = true;
    consumeSign(isPositive);

    int32_t whole = 0;
    if (!consumeInt(whole)) return false;

    if (!consume('.')) {
        out = static_cast<double>(whole);
        return true;
    }

    const size_t previous_position = position;

    int32_t decimal = 0;
    if (!consumeInt(decimal)) return false;

    int factor = 1;
    for (size_t i = 0; i < position - previous_position; i++) factor *= 10;

    out = whole + static_cast<double>(decimal) / factor;
    out = isPositive ? out : -out;

    return true;
}

FormulaCellReference* FormulaParser::consumeCellReference()
{
    uint32_t row = 0, col = 0;

    if (!consume('R')) throw std::runtime_error("Expected a 'R' while parsing a cell reference.");
    if (!consumeUInt(row)) throw std::runtime_error("Expected a row number while parsing a cell reference.");
    if (!consume('C')) throw std::runtime_error("Expected a 'C' while parsing a cell reference.");
    if (!consumeUInt(col)) throw std::runtime_error("Expected a column number while parsing a cell reference.");

    return new FormulaCellReference(row, col);
}

FormulaNumberLiteral* FormulaParser::consumeNumberLiteral()
{
    double value;

    if (!consumeDouble(value)) throw std::runtime_error("Expected a number while parsing a number literal.");

    return new FormulaNumberLiteral(value);
}


// Shunting yard algorithm
FormulaASTNode* FormulaParser::consumeExpression()
{
    std::vector<FormulaASTNode*> nodes;
    std::vector<FormulaOperator::Type> ops;

    try
    {
        while (isValid())
        {
            FormulaOperand *operand = nullptr;

            consumeWhitespaces();
            const char nextSymbol = peek();
            if (nextSymbol == 'R') operand = consumeCellReference();
            else if (std::isdigit(nextSymbol) || nextSymbol == '+' || nextSymbol == '-') operand = consumeNumberLiteral();
            else
                throw std::runtime_error("Expected an operand while parsing a formula expression.");

            nodes.push_back(operand);

            FormulaOperator::Type currentOp = FormulaOperator::Type::None;

            consumeWhitespaces();
            if (!isValid()) { break; }

            if (consume('+')) currentOp = FormulaOperator::Type::ADD;
            else if (consume('-')) currentOp = FormulaOperator::Type::SUB;
            else if (consume('*')) currentOp = FormulaOperator::Type::MUL;
            else if (consume('/')) currentOp = FormulaOperator::Type::DIV;
            else if (consume('>')) {
                if (consume('=')) currentOp = FormulaOperator::Type::GTE;
                else currentOp = FormulaOperator::Type::GT;
            }
            else if (consume('<')) {
                if (consume('=')) currentOp = FormulaOperator::Type::LTE;
                else currentOp = FormulaOperator::Type::LT;
            }
            else if (consume('=')) {
                if (consume('=')) currentOp = FormulaOperator::Type::EQ;
                else throw std::runtime_error("Expected '=' after '='.");
            }
            else if (consume('!')) {
                if (consume('=')) currentOp = FormulaOperator::Type::NEQ;
                else throw std::runtime_error("Expected '=' after '!'.");
            }

            if (currentOp != FormulaOperator::Type::None)
            {
                while (!ops.empty() && FormulaOperator::getPrecedence(ops.back()) >= FormulaOperator::getPrecedence(currentOp))
                {
                    const FormulaOperator::Type type = ops.back();
                    ops.pop_back();

                    FormulaASTNode* right = nodes.back();
                    nodes.pop_back();

                    FormulaASTNode* left = nodes.back();
                    nodes.pop_back();

                    nodes.push_back(new FormulaOperator(left, right, type));
                }

                ops.push_back(currentOp);
            }
            else
            {
                throw std::runtime_error("Unknown operator.");
            }
        }

        while (!ops.empty()) {
            const FormulaOperator::Type type = ops.back();
            ops.pop_back();

            FormulaASTNode* right = nodes.back();
            nodes.pop_back();

            FormulaASTNode* left = nodes.back();
            nodes.pop_back();

            nodes.push_back(new FormulaOperator(left, right, type));
        }
    }
    catch (std::exception &err)
    {
        while (!nodes.empty()) { delete nodes.back(); nodes.pop_back(); }
        throw err;
    }

    return nodes.back();
}

FormulaASTNode* FormulaParser::parseFormula(const std::string &text)
{
    auto parser = FormulaParser(text);

    if (!parser.consume('='))
        throw std::runtime_error("Expected '='.");

    return parser.consumeExpression();
}
