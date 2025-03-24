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

    static std::unique_ptr<Constraint> make_stay_constraint(Variable* variable, int priority) {
        std::vector<Variable*> stay_variable = {variable};
        std::unique_ptr<Constraint> new_stay = std::make_unique<Constraint>(stay_variable, priority, true, true);

        std::function<void()> stay_function = [](){ return; };
        new_stay->add_method(std::make_unique<Method>(stay_function, std::vector<Variable*>(), stay_variable, new_stay.get()));

        return new_stay;
    }
private:
    std::vector<std::unique_ptr<Method>> methods_;
    std::vector<Variable*> variables_;
    StatusType status_;
    int priority_;
    Method* selected_method_;
};
