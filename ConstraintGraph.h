#pragma once
#include "Constraint.h"
class ConstraintGraph {
public:
    ConstraintGraph() = default;

    template <typename T>
    Variable* add_variable(Belong b, std::string name, T value) {
        std::unique_ptr<Variable> variable = std::make_unique<Variable>(b, std::move(value));
        Variable* pointer = variable.get();
        variables_.push_back(std::move(variable));
        return pointer;
    }
    
    Variable* add_variable(Belong b);

    void add_constraint(std::unique_ptr<Constraint>&& constraint);

private:
    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
};
