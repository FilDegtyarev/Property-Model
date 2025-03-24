#pragma once
#include "Constraint.h"
#include <algorithm>

class ConstraintGraph {
public:
    friend DeltaBlue; // Мне нужно "грубо удалять" или явно изменять список ограничений
    ConstraintGraph() = default;

    template <typename T>
    Variable* add_variable(Belong b, std::string name, T value) {
        std::unique_ptr<Variable> variable = std::make_unique<Variable>(b, std::move(value));
        Variable* pointer = variable.get();
        variables_.push_back(std::move(variable));
        return pointer;
    }

    void add_constraint(std::unique_ptr<Constraint>&& constraint);

    const Constraint* operator[](int index) const;
    Constraint* operator[](int index);

    void define_by_stay(Variable* variable);

    int constraints_count() const;
    int new_stay_priority();

    // Это костыль
    int find_stay(Variable* variable);

    void update_values();
private:
    void collect_methods(Variable* from, std::vector<Method*>& topsort, std::unordered_map<Variable*, bool>& visited);

    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;

    std::unordered_map<Variable*, Constraint*> stay_constraint_table_;

    int stay_constraint_priority = INF;
};
