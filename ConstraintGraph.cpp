#include "ConstraintGraph.h"
#include <cassert>

namespace NSPropertyModel::detail {

void ConstraintGraph::add_constraint(std::unique_ptr<Constraint>&& constraint) {
	if (constraint->is_stay()) {
		stay_constraint_table_.insert({constraint->variables()[0], constraint.get()});
	}
	constraints_.push_back(std::move(constraint));
}

const Constraint* ConstraintGraph::operator[](int index) const {
	return constraints_[index].get();
}

Constraint* ConstraintGraph::operator[](int index) {
	return constraints_[index].get();
}

void ConstraintGraph::define_by_stay(Variable* variable) {
	assert(variable);
	stay_constraint_table_[variable]->satisfy(0);
	variable->force_ = stay_constraint_table_[variable]->priority();
}

int ConstraintGraph::constraints_count() const {
	return constraints_.size();
}

int ConstraintGraph::new_stay_priority() {
	return --stay_constraint_priority;
}

int ConstraintGraph::find_index_of_stay(Variable* variable) {
	assert(variable);
	for (int constraint_index = 0; constraint_index < constraints_.size(); ++constraint_index) {
		Constraint* constraint = constraints_[constraint_index].get();
		if (constraint->is_stay() && constraint->variables()[0] == variable) {
			return constraint_index;
		}
	}
	assert(false);
}

void ConstraintGraph::update_values() {
	std::unordered_map<Variable*, bool> visited;
	for (const std::unique_ptr<Variable>& variable : variables_) {
		visited[variable.get()] = false;
	}

	std::vector<Method*> topsort;
	for (const std::unique_ptr<Variable>& variable : variables_) {
		if (!visited[variable.get()]) {
			collect_methods(variable.get(), topsort, visited);
		}
	}

	std::reverse(topsort.begin(), topsort.end());

	for (const Method* method : topsort) {
		method->execute();
	}
}

void ConstraintGraph::collect_methods(Variable* from, std::vector<Method*>& topsort,
									  std::unordered_map<Variable*, bool>& visited) {
	assert(from);
	visited[from] = true;
	for (const Method* method : from->edges()) {
		if (!visited[method->outputs()[0]]) {
			collect_methods(method->outputs()[0], topsort, visited);
		}
	}
	topsort.push_back(from->defining_method());
}
} // namespace NSPropertyModel::detail
