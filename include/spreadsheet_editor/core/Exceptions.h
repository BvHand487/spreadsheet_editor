#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "Cell.h"


// Errors when evaluating cells in general
class cell_evaluation_error : public std::runtime_error
{
public:
    explicit cell_evaluation_error(const std::string& msg) :
        runtime_error(msg) {}
};

class cell_evaluation_critical_error : public std::runtime_error
{
public:
    explicit cell_evaluation_critical_error(const std::string& msg) :
        runtime_error{msg} {}
};


// Errors when evaluating formulas
class formula_division_by_zero_error final : public cell_evaluation_error
{
public:
    explicit formula_division_by_zero_error(const std::string& message) :
            cell_evaluation_error{message} {}
};

class formula_cyclic_dependency_error final : public cell_evaluation_critical_error
{
public:
    explicit formula_cyclic_dependency_error(const std::string& msg) :
        cell_evaluation_critical_error{msg} {}
};


#endif // EXCEPTIONS_H