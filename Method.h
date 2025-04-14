#pragma once
#include "Variable.h"
#include <functional>
#include <vector>

class Constraint;

struct Method {
	Method(std::function<void()> method, std::vector<Variable*> inputs, std::vector<Variable*> outputs,
		   Constraint* associated_constraint);

	void execute() const;
	void satisfy_method();
	void unsatisfy_method();
	const Constraint* associated_constraint() const;
	Constraint* associated_constraint();
	const std::vector<Variable*> inputs() const;
	const std::vector<Variable*> outputs() const;
	bool is_chosen() const;

  private:
	std::function<void(void)> method_;
	std::vector<Variable*> inputs_;
	std::vector<Variable*> outputs_;
	Constraint* associated_constraint_;
};
