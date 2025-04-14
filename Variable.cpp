#include "Constraint.h"
void Variable::determine(Method* method) {
	determined_by = method;
}

const std::any& Variable::value() const {
	return data_;
}

const Constraint* Variable::defining_constraint() const {
	return determined_by->associated_constraint();
}

Constraint* Variable::defining_constraint() {
	return determined_by->associated_constraint();
}

const Method* Variable::defining_method() const {
	return determined_by;
}

Method* Variable::defining_method() {
	return determined_by;
}

int Variable::force() const {
	return force_;
}

const std::vector<Method*>& Variable::edges() const {
	return edges_to_methods_;
}

void Variable::add_edge_to(Method* method) {
	edges_to_methods_.push_back(method);
}

void Variable::remove_edge_to(Method* method) {
	auto iter_to_delete = std::find(edges_to_methods_.begin(), edges_to_methods_.end(), method);
	if (iter_to_delete != edges_to_methods_.end()) {
		edges_to_methods_.erase(iter_to_delete);
	}
}
