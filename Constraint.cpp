#include "Constraint.h"
#include <cassert>

void Constraint::add_method(std::unique_ptr<Method>&& method) {
    methods_.push_back(std::move(method));
}

const std::vector<Variable*>& Constraint::variables() const { return variables_; }
std::vector<Variable*> Constraint::variables() { return variables_; }

const std::vector<std::unique_ptr<Method>>& Constraint::methods() const { return methods_; }

void Constraint::enable() { status_ = Status::type{status_ | Status::enabled}; }

Constraint::Constraint(std::vector<Variable*> variables, int priority, bool enable, bool stay) {
    status_ = Status::type((Status::type::enabled & enable) | (Status::type::stay & stay));
    variables_ = variables;
    priority_ = priority;
}

void Constraint::satisfy(int method_index) {
    assert(!(status_ & Status::enabled));
    selected_method_ = methods_[method_index].get();
    methods_[method_index].get()->satisfy_method(); 
}

void Constraint::satisfy(Method* method) {
    selected_method_ = method;
    method->satisfy_method();
}

void Constraint::unsatisfy() {
    selected_method_->unsatisfy_method();
}

const Method* Constraint::operator[](int index) const { return methods_[index].get(); }

Method* Constraint::operator[](int index) { return methods_[index].get(); }

bool Constraint::is_stay() const { return status_ & Status::stay; }

int Constraint::priority() const { return priority_; }

const Method* Constraint::selected_method() const { return selected_method_; }
