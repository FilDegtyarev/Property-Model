#pragma once
#include "ConstraintGraph.h"
template <typename A, typename B, typename C>
class PMBuilder;

template <typename A, typename B, typename C>
class PropertyModelImpl;

template <typename...DataArgs, typename...ValueArgs, typename...OutputArgs>
class PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...> > {
public:
    friend PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;
    
    void show(bool debug) {
        std::cout << "\nProperty Model current state:\n";
        std::cout << "-----------------------------\n";
        if (debug) {
            std::cout << "Data:\n";
            int i = 0;
            ((std::cout << '\t' << data_[i] << ": " 
            << std::any_cast<DataArgs>(variables_[data_[i++]]->value()) << '\n'), ...);

            std::cout << "Value:\n";
            i = 0;
            ((std::cout << '\t' << value_[i] << ": " 
            << std::any_cast<ValueArgs>(variables_[value_[i++]]->value()) << '\n'), ...);
        }

        std::cout << "Output:\n";
        int i = 0;
        ((std::cout << '\t' << output_[i] << ": " 
        << std::any_cast<OutputArgs>(variables_[output_[i++]]->value()) << '\n'), ...);
        std::cout << "-----------------------------\n";
    }

private:
    PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>() = default;

    template <Belong B, typename T>
    void add_variable(std::string name, T value) {
        /// ГДЕ ЛУЧШЕ АЛЛОЦИРОВАТЬ VARIABLE???
        //Variable* pointer = constraint_graph.add_variable(B, name, value);
        Variable* pointer = constraint_graph.add_variable(B, name, value);
        variables_.insert({name, pointer});
        switch (B) {
            case Belong::DataVariable:
                data_.push_back(std::move(name));
                break;
            case Belong::ValueVariable: 
                value_.push_back(std::move(name));
                break;
            case Belong::OutputVariable:
                output_.push_back(std::move(name));
                break;
            case Belong::Default:
                assert(false);
        }
    }

    std::vector<Variable*> bind_variables(std::vector<std::string> names) {
        std::vector<Variable*> references;
        for (auto && name: names) {
            references.push_back(variables_[name]);
        }
        return references;
    }

    template <typename... Types, typename... OutputTypes>
    std::function<void()> bind_method(std::function<std::tuple<OutputTypes...>(Types...)> function, std::vector<std::string> inputs, std::vector<std::string> outputs) {
        std::vector<Variable*> input_variables;
        for (int i = 0; i < inputs.size(); ++i) {
            input_variables.push_back(variables_[inputs[i]]);
        }

        std::vector<Variable*> output_variables;
        for (int i = 0; i < outputs.size(); ++i) {
            output_variables.push_back(variables_[output_[i]]);
        }

        std::function<void()> method = (([this, function, inputs, outputs](){
            int i = 0;
            auto args = std::make_tuple(std::any_cast<Types>(variables_[inputs[i++]]->value())...);
            std::tuple<OutputTypes...> result = std::apply(function, args);
            std::apply(
                [this, outputs, result](auto&&... args) {
                int j = 0;
                ((variables_[outputs[j++]]->load_data(std::forward<decltype(args)>(args))), ...);
                }
            , result);
        }));
        return method;
    }

    void add_constraint(std::unique_ptr<Constraint>&& constraint) {
        constraint_graph.add_constraint(std::move(constraint));
    }

    // void initialize_stay_constraints() {
    //     int stay_priority = constraints_.size() + 1;
    //     for (auto& [name, variable]: variables_) {
    //         add_constraint({name}, stay_priority++, true, true);
    //         std::function<std::tuple<>()> stay_execution = [](){return std::tuple<>();};
    //         add_method(stay_execution, {}, {name});
    //     }

    //     for (Constraint& constraint: constraints_) {
    //         if (constraint.is_stay()) {
    //             constraint.satisfy(0);   
    //         }
    //     }
    // }

    // void initialize_solution_graph() {
    //     int i = 0;
    //     for (Constraint& constraint: constraints_) {
    //         if (constraint.is_stay()) {
    //             //update_force(this, i);
    //         }
    //         ++i;
    //     }
    // }
    std::vector<std::string> data_;
    std::vector<std::string> value_;
    std::vector<std::string> output_;
    
    std::unordered_map<std::string, Variable*> variables_;

    ConstraintGraph constraint_graph;
};
