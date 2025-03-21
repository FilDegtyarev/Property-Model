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

    const Constraint* operator[](int index) const;
    Constraint* operator[](int index);

    void define_by_stay(Variable* variable);
private:
    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;

    std::unordered_map<Variable*, Constraint*> stay_constraint_table_;
};
