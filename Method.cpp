#include "Method.h"
#include "Constraint.h"

Method::Method(std::function<void()> method, std::vector<Variable*> inputs, std::vector<Variable*> outputs,
			   Constraint* associated_constraint)
	: method_(std::move(method)), inputs_(std::move(inputs)), outputs_(std::move(outputs)),
	  associated_constraint_(std::move(associated_constraint)) {};

void Method::execute() const {
	assert(method_);
	method_();
}

void Method::satisfy_method() {
	for (Variable* variable : outputs_) {
		variable->determine(this);
	}

	for (Variable* variable : inputs_) {
		variable->add_edge_to(this);
	}
}

void Method::unsatisfy_method() {
	for (Variable* variable : outputs_) {
		variable->determine(nullptr);
	}

	for (Variable* variable : inputs_) {
		variable->remove_edge_to(this);
	}
}

const Constraint* Method::associated_constraint() const {
	return associated_constraint_;
}

Constraint* Method::associated_constraint() {
	return associated_constraint_;
}

const std::vector<Variable*> Method::inputs() const {
	return inputs_;
}

const std::vector<Variable*> Method::outputs() const {
	return outputs_;
}

bool Method::is_chosen() const {
	return associated_constraint()->selected_method() == this;
}
