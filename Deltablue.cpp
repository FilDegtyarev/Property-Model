#include "Deltablue.h"
#include <stdexcept>

void DeltaBlue::recalculate_forces(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, bool>& visited) {
    visited[from] = true;
    // from определена кем-то. Сила from = max(constraint, possible outputs)
    int new_force = from->defining_constraint()->priority();

    for (const std::unique_ptr<Method>& method : from->defining_constraint()->methods()) {
        if (method.get()->outputs()[0] != from) {
            new_force = std::max(new_force, method->outputs()[0]->force_);
        }
    }

    from->force_ = new_force;

    for (Method* output_method : from->ouput_edges) {
        Variable* to = output_method->outputs()[0];
        if (!visited[to]) {
            DeltaBlue::recalculate_forces(g, to, visited);
        }
    }
}

void DeltaBlue::detect_cycle(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, int>& visited) {
    visited[from] = 1;

    for (Method* output : from->ouput_edges) {
        Variable* to = output->outputs()[0];

        if (visited[to] == 1) {
            throw std::logic_error("There is a cycle.");
        } else if (!visited[to]) {
            DeltaBlue::detect_cycle(g, to, visited);
        }
    }
    visited[from] = 2;
}

void DeltaBlue::reverse_path(ConstraintGraph& g, Variable* from) {
    // det_cst -> from -> ???
    int end_force = from->force_;
    Variable* current_variable = from;
    while (true) {
        if (current_variable->defining_constraint()->priority() == end_force) {
            current_variable->defining_constraint()->unsatisfy();
            g.define_by_stay(current_variable);
            break;
        }

        // Current обладает possible output силой равной end_force
        for (const std::unique_ptr<Method>& method_ptr : current_variable->defining_constraint()->methods()) {
            Method* method = method_ptr.get();
            if (method->outputs()[0]->force_ == end_force && (!method->is_chosen())) {
                method->associated_constraint()->unsatisfy();
                method->associated_constraint()->satisfy(method);
                current_variable = method->outputs()[0];
                break;
            }
        }
    }
}

void DeltaBlue::enable_constraint(ConstraintGraph& g, int index) {
    // Blocked???
    Constraint* new_constraint = g[index];
    bool is_blocked = false;
    for (Variable* variable : new_constraint->variables()) {
        if (variable->force_ > new_constraint->priority()) {
            is_blocked = true;
        }
    }
    // Is not blocked
    if (!is_blocked) {
        // Добавили в "Constraint Graph"
        new_constraint->enable();
        return;
    }

    Method* chosen_method = (*new_constraint)[0];
    for (const std::unique_ptr<Method>& method_ptr : new_constraint->methods()) {
        if (chosen_method->outputs()[0]->force_ < method_ptr.get()->outputs()[0]->force_) {
            chosen_method = method_ptr.get();
        }
    }
    
    DeltaBlue::reverse_path(g, chosen_method->outputs()[0]);
    
    std::unordered_map<Variable*, int> cycle_visited;
    DeltaBlue::detect_cycle(g, chosen_method->outputs()[0], cycle_visited);

    std::unordered_map<Variable*, bool> visited;
    DeltaBlue::recalculate_forces(g, chosen_method->outputs()[0], visited);

}
