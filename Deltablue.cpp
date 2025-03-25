#include "Deltablue.h"
#include <memory>
#include <stdexcept>
#include <unordered_map>

// ПЕРЕДЕЛАТЬ
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

    for (Method* output_method : from->edged_to_methods_) {
        Variable* to = output_method->outputs()[0];
        if (!visited[to]) {
            DeltaBlue::recalculate_forces(g, to, visited);
        }
    }
}

void DeltaBlue::detect_cycle(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, int>& visited) {
    visited[from] = 1;

    for (Method* output : from->edged_to_methods_) {
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
    std::vector<Method*> path;
    while (true) {
        if (current_variable->defining_constraint()->priority() == end_force) {
            current_variable->defining_constraint()->unsatisfy();
            g.define_by_stay(current_variable);
            break;
        }

        for (const std::unique_ptr<Method>& method_ptr : current_variable->defining_constraint()->methods()) {
            Method* method = method_ptr.get();
            if (method->outputs()[0]->force_ == end_force && (!method->is_chosen())) {
                current_variable = method->outputs()[0];
                path.push_back(method);
                break;
            }
        }
    }
    
    std::reverse(path.begin(), path.end());
    for (Method* method : path) {
        method->associated_constraint()->unsatisfy();
        method->associated_constraint()->satisfy(method);
    }

    return;
}

void DeltaBlue::enable_constraint(ConstraintGraph& g, int index) {
    Constraint* new_constraint = g[index];
    bool is_blocked = false;
    for (Variable* variable : new_constraint->variables()) {
        if (variable->force_ > new_constraint->priority()) {
            is_blocked = true;
        }
    }

    if (!is_blocked) {
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
    g[index]->satisfy(chosen_method);

    std::unordered_map<Variable*, int> cycle_visited;
    DeltaBlue::detect_cycle(g, chosen_method->outputs()[0], cycle_visited);

    std::unordered_map<Variable*, bool> visited;
    DeltaBlue::recalculate_forces(g, chosen_method->outputs()[0], visited);

}

void DeltaBlue::disable_stay(ConstraintGraph& g, int index) {
    std::unique_ptr<Constraint> new_stay = Constraint::make_stay_constraint(g[index]->variables()[0], g.new_stay_priority());
    g.constraints_.push_back(std::move(new_stay));
    DeltaBlue::enable_constraint(g, g.constraints_.size() - 1);

    g.constraints_[index] = std::move(g.constraints_.back());
    g.stay_constraint_table_[g[index]->variables()[0]] = g.constraints_[index].get();
    g.constraints_.pop_back();
}

void DeltaBlue::disable_constraint(ConstraintGraph& g, int index) {
    if (g[index]->is_stay()) {
        disable_stay(g, index);
        return;
    }

    if (!g[index]->is_satisfied()) {
        g[index]->disable();
        return;
    }

    g[index]->unsatisfy();
    Variable* not_defined = g[index]->variables()[0];
    g.define_by_stay(not_defined);

    std::unordered_map<Variable*, bool> visited;
    DeltaBlue::recalculate_forces(g, not_defined, visited);

    for (int constraint_index = 0; constraint_index < g.constraints_count(); ++constraint_index) {
        if (g[constraint_index]->is_enable() && !g[constraint_index]->is_satisfied()) {
            DeltaBlue::enable_constraint(g, constraint_index);
        }
    }
}
