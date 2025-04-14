#pragma once
#include "utils.h"
#include <any>
#include <string>
#include <vector>

struct Method;
class Constraint;
class DeltaBlue;
class ConstraintGraph;

struct Variable {
	static const int k_inf = 1e9;
	friend DeltaBlue;
	friend ConstraintGraph;

	Variable() = default;

	template<typename T>
	Variable(Belong belong, std::string name, T&& data)
		: belong_(std::move(belong)), name_(std::move(name)), data_(std::move(data)){};

	void determine(Method* method);
	const std::any& value() const;
	int force() const;

	template<typename T>
	void load_data(T&& value) {
		data_ = value;
	}

	const Constraint* defining_constraint() const;
	Constraint* defining_constraint();
	const Method* defining_method() const;
	Method* defining_method();
	const std::vector<Method*>& edges() const;
	void add_edge_to(Method* method);
	void remove_edge_to(Method* method);

  private:
	Belong belong_;
	std::string name_;
	std::any data_;
	Method* determined_by;
	int force_ = k_inf;
	std::vector<Method*> edges_to_methods_;
};
