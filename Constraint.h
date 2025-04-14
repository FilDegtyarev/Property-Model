#pragma once
#include "Method.h"
#include <algorithm>
#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

struct Status {
	enum type : unsigned int { satisfied = 1, enabled = 2, stay = 4 };
};

enum Priority : int {};

using StatusType = Status::type;
class Constraint {
  public:
	Constraint(std::vector<Variable*> variables, Priority priority, StatusType status);

	void add_method(std::unique_ptr<Method>&& method);
	const std::vector<Variable*>& variables() const;
	std::vector<Variable*> variables();
	const std::vector<std::unique_ptr<Method>>& methods() const;
	void enable();
	void satisfy(int method_index);
	void satisfy(Method* method);
	void unsatisfy();
	void disable();
	const Method* operator[](int index) const;
	Method* operator[](int index);
	bool is_stay() const;
	bool is_enable() const;
	bool is_satisfied() const;
	int priority() const;
	const Method* selected_method() const;
	static std::unique_ptr<Constraint> make_stay_constraint(Variable* variable, int priority);

  private:
	std::vector<std::unique_ptr<Method>> methods_;
	std::vector<Variable*> variables_;
	StatusType status_;
	int priority_;
	Method* selected_method_;
};
