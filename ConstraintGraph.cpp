#include "ConstraintGraph.h"

Variable* ConstraintGraph::add_variable(Belong b) {
    return nullptr;
}

void ConstraintGraph::add_constraint(std::unique_ptr<Constraint>&& constraint) {
    constraints_.push_back(std::move(constraint));
}
