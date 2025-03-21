#include "ConstraintGraph.h"

Variable* ConstraintGraph::add_variable(Belong b) {
    return nullptr;
}

void ConstraintGraph::add_constraint(std::unique_ptr<Constraint>&& constraint) {
    if (constraint->is_stay()) {
        //stay_constraint_table_[constraint->variables()[0]] = constraint.get();
        stay_constraint_table_.insert({constraint->variables()[0], constraint.get()});
    }
    constraints_.push_back(std::move(constraint));
}

const Constraint* ConstraintGraph::operator[](int index) const { return constraints_[index].get(); }

Constraint* ConstraintGraph::operator[](int index) { return constraints_[index].get(); }

void ConstraintGraph::define_by_stay(Variable* variable) {
    stay_constraint_table_[variable]->satisfy(0);
}
