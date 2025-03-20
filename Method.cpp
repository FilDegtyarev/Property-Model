#include "Method.h"

Method::Method(std::function<void()> method, std::vector<Variable*> inputs, std::vector<Variable*> outputs, Constraint* associated_constraint):
    method_(method),
    inputs_(inputs),
    outputs_(outputs),
    associated_constraint_(associated_constraint) {};

void Method::execute() const {
    method_();
}

void Method::satisfy_method() {
    for (Variable* variable : outputs_) {
        variable->determine(this);
    }
}

const Constraint* const Method::associated_constraint() const {
    return associated_constraint_;
}

const std::vector<Variable*> Method::inputs() const { return inputs_; }
const std::vector<Variable*> Method::outputs() const { return outputs_; }
