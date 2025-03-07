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

enum Belong {
    DataVariable,
    ValueVariable,
    OutputVariable
};

class Constraint;
class Method;

struct Variable {
    template <typename T>
    Variable(Belong, std::string, T);
    Variable();
    void determine(std::reference_wrapper<Constraint> constraint) {
        determined_by_ = constraint;
    }

    template <typename T>
    std::any& operator=(T value) {
        return (data_ = value);
    }

    std::any value() {
        return data_;
    }

    void update_force();
    Method& determining_method();

private:
    Belong belong_;
    int index_;
    std::any data_;
    std::string name_;
    std::reference_wrapper<Constraint> determined_by_;
    int force_;
};

class Method {
public:
    friend Constraint;

    Method(std::function<void(void)>, std::vector<std::reference_wrapper<Variable>>, std::vector<std::reference_wrapper<Variable>>);
    
    void execute() {
        method_();
    }

    void satisfy() {
        for (int i = 0; i < outputs_.size(); ++i) {
            outputs_[i].get().determine(assosiated_constraint_);
        }
    }

    const std::vector<std::reference_wrapper<Variable>>& outputs() const { return outputs_; }

private:
    std::vector<std::reference_wrapper<Variable>> inputs_;
    std::vector<std::reference_wrapper<Variable>> outputs_;
    std::function<void(void)> method_;
    std::reference_wrapper<Constraint> assosiated_constraint_;
};

class Constraint {
public:
    void add_method(Method m) {
        methods_.push_back(m);
    }

    Constraint(std::vector<std::string> variables, int priority, bool enable, bool stay): 
    priority_(priority), 
    satisfied_(false), 
    enable_(enable), 
    stay_(stay), 
    variables_(variables) {};

    void satisfy(int method_index) {
        satisfied_ = true;
        selected_method_ = method_index;
        methods_[method_index].satisfy();
    }

    Method& operator[](int i) {
        return methods_[i];
    }

    bool is_stay() {return stay_;}

    int priority() {return priority_;}

    Method& selected_method() { return methods_[selected_method_]; }
    
    const std::vector<Method>& methods() {return methods_;}

private:
    std::vector<Method> methods_;
    std::vector<std::string> variables_;
    int selected_method_;
    int priority_;
    bool satisfied_;
    bool enable_;
    bool stay_;
};

Constraint NO_CONSTRAINT({}, -1e9, false, false);

template <typename T>
Variable::Variable(Belong belong, std::string name, T value): 
determined_by_(std::ref(NO_CONSTRAINT)),
belong_(belong),
name_(name),
data_(value),
force_(-1e9) {};

Method::Method(std::function<void(void)> method, std::vector<std::reference_wrapper<Variable>> inputs, std::vector<std::reference_wrapper<Variable>> outputs): 
    assosiated_constraint_(std::ref(NO_CONSTRAINT)),
    method_(method),
    inputs_(inputs),
    outputs_(outputs) {};

Method& Variable::determining_method() {
    return determined_by_.get().selected_method();
}

Variable::Variable(): determined_by_(std::ref(NO_CONSTRAINT)), index_(-1), name_("NOT EXIST") {
    throw std::out_of_range("Default constructor is prohibited");
};

void Variable::update_force() {
    force_ = determined_by_.get().priority();
    for (const Method& method: determined_by_.get().methods()) {
        force_ = std::max(force_, method.outputs()[0].get().force_);
    }
}