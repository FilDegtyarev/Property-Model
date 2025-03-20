#pragma once
#include <functional>
#include "Constraint.h"

class DeltaBlue {
public:
    static std::vector<std::reference_wrapper<Method>> enable_constraint(std::vector<std::reference_wrapper<Variable>>& variables, std::vector<Constraint>& constraints);
    static std::vector<std::reference_wrapper<Method>> disable_constraint(std::vector<std::reference_wrapper<Variable>>& variables, std::vector<Constraint>& constraints);
private:
    // ...
};
