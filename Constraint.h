#pragma once
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <functional>
#include <any>
#include <utility>
#include <cassert>
#include <unordered_map>
#include <memory>
#include "Method.h"

struct Status {
    enum type : int {
        satisfied = 1,
        enabled = 2,
        stay = 4
    };
};

using StatusType = Status::type;

class Constraint {
public:
    void add_method(std::unique_ptr<Method>&& method);

    Constraint(std::vector<Variable*> variables, int priority, bool enable, bool stay);

    void satisfy(int method_index);

    const Method* operator[](int index);

    bool is_stay() const;

    int priority() const;

    const Method* selected_method() const;

private:
    std::vector<std::unique_ptr<Method>> methods_;
    std::vector<Variable*> variables_;
    StatusType status_;
    int priority_;
    Method* selected_method_;
};
