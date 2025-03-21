#pragma once
#include "utils.h"
#include <string>
#include <vector>
#include <any>

struct Method;
class Constraint;
class DeltaBlue;

struct Variable {
    friend DeltaBlue;
    Variable() = default;

    template <typename T>
    Variable(Belong belong, T&& data): belong_(belong), data_(data) {};

    void determine(Method* method);

    const std::any& value() const;

    int force() const;
    
    template <typename T>
    void load_data(T&& value) {
        data_ = value;
    }

    const Constraint* const defining_constraint() const;
    Constraint* defining_constraint();

    const Method* const defining_method() const;
private:
    Belong belong_;
    std::any data_;
    Method* determined_by;
    int force_ = INF;
    std::vector<Method*> ouput_edges;
};
