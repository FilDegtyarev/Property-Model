#pragma once
#include "ConstraintGraph.h"

class DeltaBlue {
public:
    static void enable_constraint(ConstraintGraph& graph, int index);
    static std::vector<Method*> disable_constraint(ConstraintGraph& graph);
private:
    static void recalculate_forces(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, bool>& visited);
    static void detect_cycle(ConstraintGraph& graph, Variable* from, std::unordered_map<Variable*, int>& visited);
    static void reverse_path(ConstraintGraph& graph, Variable* from);
};
