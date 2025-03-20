#pragma once
#include "utils.h"
#include <string>
#include <any>

struct Method;
class Constraint;

struct Variable {
    Variable() = default;

    template <typename T>
    Variable(Belong belong, T&& data): belong_(belong), data_(data) {};

    void determine(Method* method);

    const std::any& value() const;

    template <typename T>
    void load_data(T&& value) {
        data_ = value;
    }

    const Constraint* const defining_constraint() const;
    const Method* const defining_method() const;
private:
    Belong belong_;
    std::any data_;
    Method* determined_by;
};
