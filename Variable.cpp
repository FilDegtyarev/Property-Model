
#include "Constraint.h"


void Variable::determine(Method* method) { determined_by = method; }

const std::any& Variable::value() const { return data_; }

const Constraint* const Variable::defining_constraint() const { return determined_by->associated_constraint(); }

Constraint* Variable::defining_constraint() { return determined_by->associated_constraint(); }

const Method* const Variable::defining_method() const { return determined_by; }
Method* Variable::defining_method() { return determined_by; }

int Variable::force() const { return force_; }

const std::vector<Method*>& Variable::edges() const { return edged_to_methods_; }

void Variable::add_edge_to(Method* method) { edged_to_methods_.push_back(method); return; }
void Variable::remove_edge_to(Method *method) {
    for (int method_index = 0; method_index < edged_to_methods_.size(); ++method_index) {
        if (edged_to_methods_[method_index] == method) {
            edged_to_methods_.erase(edged_to_methods_.begin() + method_index);
            return;
        }
    }
}
