#include "Constraint.h"
#include <cassert>

namespace NSPropertyModel::detail {
StatusType operator|(StatusType first, StatusType second) {
	return StatusType((unsigned int)(first) | (unsigned int)(second));
}
StatusType operator&(StatusType first, StatusType second) {
	return StatusType((unsigned int)(first) & (unsigned int)(second));
}

void Constraint::add_method(std::unique_ptr<Method>&& method) {
	methods_.push_back(std::move(method));
}

const std::vector<Variable*>& Constraint::variables() const {
	return variables_;
}

std::vector<Variable*> Constraint::variables() {
	return variables_;
}

const std::vector<std::unique_ptr<Method>>& Constraint::methods() const {
	return methods_;
}

void Constraint::enable() {
	status_ = Status::type{status_ | Status::enabled};
}

void Constraint::disable() {
	status_ = Status::type{status_ & Status::stay};
}

Constraint::Constraint(std::vector<Variable*> variables, Priority priority, StatusType status) {
	status_ = status;
	variables_ = variables;
	priority_ = priority;
}

void Constraint::satisfy(int method_index) {
	assert(!(status_ & Status::satisfied));
	selected_method_ = methods_[method_index].get();
	methods_[method_index].get()->satisfy_method();
}

void Constraint::satisfy(Method* method) {
	assert(method);
	selected_method_ = method;
	method->satisfy_method();
}

void Constraint::unsatisfy() {
	selected_method_->unsatisfy_method();
}

const Method* Constraint::method(int index) const {
	return methods_[index].get();
}

Method* Constraint::method(int index) {
	return methods_[index].get();
}

Variable* Constraint::output() const {
	return selected_method_->output();
}

bool Constraint::is_stay() const {
	return status_ & Status::stay;
}

bool Constraint::is_enable() const {
	return status_ & Status::enabled;
}

bool Constraint::is_satisfied() const {
	return status_ & Status::satisfied;
}

int Constraint::priority() const {
	return priority_;
}

const Method* Constraint::selected_method() const {
	return selected_method_;
}

std::unique_ptr<Constraint> Constraint::make_stay_constraint(Variable* variable, int priority) {
	assert(variable);
	std::vector<Variable*> stay_variable = {variable};
	std::unique_ptr<Constraint> new_stay =
		std::make_unique<Constraint>(stay_variable, Priority{priority}, Status::enabled | Status::stay);

	std::function<void()> stay_function = []() { return; };
	new_stay->add_method(
		std::make_unique<Method>(stay_function, std::vector<Variable*>(), std::move(stay_variable), new_stay.get()));

	return new_stay;
}
} // namespace NSPropertyModel::detail
