
#include "Constraint.h"


void Variable::determine(Method* method) {
    determined_by = method;
}

const std::any& Variable::value() const { return data_; }

const Constraint* const Variable::defining_constraint() const {
    return determined_by->associated_constraint();
}

const Method* const Variable::defining_method() const {
    return determined_by;
}
