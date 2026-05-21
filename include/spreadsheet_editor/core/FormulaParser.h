#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <cstdint>
#include <string>
#include <utility>

#include "Formula.h"


class FormulaParser
{
    std::string source;
    size_t position = 0;

    [[nodiscard]] bool isValid() const { return position < source.length(); }
    [[nodiscard]] char peek() const;

    bool consume(char symbol);
    bool consumeWhitespaces();

    // if outSign == true -> positive, else negative
    bool consumeSign(bool& outSign);
    bool consumeInt(int32_t& out);
    bool consumeUInt(uint32_t& out);
    bool consumeDouble(double& out);

    FormulaCellReference* consumeCellReference();
    FormulaNumberLiteral* consumeNumberLiteral();

    FormulaASTNode* consumeExpression();

public:
    explicit FormulaParser(std::string source): source(std::move(source)) {}

    static FormulaASTNode* parseFormula(const std::string& text);
};

#endif //FORMULAPARSER_H
