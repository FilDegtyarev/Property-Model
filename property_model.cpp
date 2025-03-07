#include "constraint.cpp"
#include "utils.cpp"

template <typename A, typename B, typename C>
class PMBuilder;

template <typename A, typename B, typename C>
class PropertyModelImpl;

template <typename A, typename B, typename C>
class PropertyModel;

// В дальнейшем методов rough_* не будет - они содержатся для базовой демонстрации, того, что происходит
template <typename...DataArgs, typename...ValueArgs, typename...OutputArgs>
class PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> {
public:
    using Builder = PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

    friend PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;    // Propety Model Конструируется только от билдера, который имеет доступ к ее полям

    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>(const PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>&) = delete;
    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> operator=(const PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>&) = delete;

    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>(PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>&&) = default;
    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>& operator=(PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>&&) = default;
    
    PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>* operator->() {
        return pm_impl_.get();
    }

    void show(bool debug=false) {
        pm_impl_.get()->show(debug);
    }

    template <typename T>
    void set(std::string name, T value);

private:
    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>() {};

    std::unique_ptr<PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>> pm_impl_;
};

template <typename...DataArgs, typename...ValueArgs, typename...OutputArgs>
class PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...> > {
public:
    // Технические функции PMImpl не должны быть доступны по pm->, тогда билдер должен иметь к ним доступ
    friend PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;
    
    void show(bool debug) {
        std::cout << "\nProperty Model current state:\n";
        std::cout << "-----------------------------\n";
        if (debug) {
            std::cout << "Data:\n";
            int i = 0;
            ((std::cout << '\t' << data_[i] << ": " 
            << std::any_cast<DataArgs>(variables_[data_[i++]].value()) << '\n'), ...);

            std::cout << "Value:\n";
            i = 0;
            ((std::cout << '\t' << value_[i] << ": " 
            << std::any_cast<ValueArgs>(variables_[value_[i++]].value()) << '\n'), ...);
        }

        std::cout << "Output:\n";
        int i = 0;
        ((std::cout << '\t' << output_[i] << ": " 
        << std::any_cast<OutputArgs>(variables_[output_[i++]].value()) << '\n'), ...);
        std::cout << "-----------------------------\n";
    }

    void rough_execute(int c_i, int m_i) {
        constraints_[c_i][m_i].execute();
    }

    template <typename T>
    void rough_set(std::string name, T value) {
        variables_[name] = value;
    }

private:
    std::vector<std::string> data_;
    std::vector<std::string> value_;
    std::vector<std::string> output_;
    std::unordered_map<std::string, Variable> variables_;

    std::vector<Constraint> constraints_;

    PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>() {};

    template <Belong B, typename T>
    void initialize_variable(std::string name, T value) {
        variables_.insert({name, Variable(B, name, value)});
        switch (B) {
            case Belong::DataVariable:
                data_.push_back(name);
                break;
            case Belong::ValueVariable: 
                value_.push_back(name);
                break;
            case Belong::OutputVariable:
                output_.push_back(name);
                break;
        }
    }

    template <typename... Types, typename... OutputTypes>
    void add_method(std::function<std::tuple<OutputTypes...>(Types...)> function, std::vector<std::string> inputs, std::vector<std::string> outputs) {
        std::vector<std::reference_wrapper<Variable>> input_variables;
        std::vector<std::reference_wrapper<Variable>> output_variables;
        
        for (int i = 0; i < inputs.size(); ++i) {
            input_variables.emplace_back(std::ref(variables_[inputs[i]]));
        }

        for (int i = 0; i < outputs.size(); ++i) {
            output_variables.emplace_back(std::ref(variables_[outputs[i]]));
        }

        std::function<void(void)> method = (([this, function, inputs, outputs](){
            int i = 0;
            std::tuple<OutputTypes...> result = std::invoke(function, std::any_cast<Types>(variables_[inputs[i++]].value())...);
            i = 0;
            std::apply(
                [this, outputs, result](auto&&... args) {
                int j = 0;
                ((variables_[outputs[j++]] = args), ...);
                }
            , result);
        }));

        constraints_.back().add_method(Method(method, input_variables, output_variables));
    }

    void add_constraint(std::vector<std::string> variables, int priority, bool enable, bool stay) {
        constraints_.emplace_back(variables, priority, enable, stay);
    }

    void initialize_stay_constraints() {
        int stay_priority = constraints_.size() + 1;
        for (auto& [name, variable]: variables_) {
            this->add_constraint({name}, stay_priority++, true, true);
            std::function<std::tuple<>(void)> stay_execution = [](){return std::tuple<>();};
            this->add_method(stay_execution, {}, {name});
        }

        for (Constraint& constraint: constraints_) {
            if (constraint.is_stay()) {
                constraint.satisfy(0);   
            }
        }
    }

    void initialize_solution_graph() {
        int i = 0;
        for (Constraint& constraint: constraints_) {
            if (constraint.is_stay()) {
                //update_force(this, i);
            }
            ++i;
        }
    }

};

template <typename...DataArgs, typename...ValueArgs, typename...OutputArgs>
class PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...> > {
public:

    PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>() {
        property_model_ = PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>();
        Implementation* impl = new Implementation();
        property_model_.pm_impl_ = std::move(std::unique_ptr<Implementation>(impl));
    };

    template<Belong B, typename T>
    void initialize_variable(std::string name, T value) {
        property_model_->template initialize_variable<B>(name, value);
    }

    void add_constraint(std::vector<std::string> variables, int priority, bool enable) {
        property_model_->add_constraint(variables, priority, enable, false);
    }

    template <typename... Types, typename... OutputTypes>
    void add_method(std::function<std::tuple<OutputTypes...>(Types...)> function, std::vector<std::string> inputs, std::vector<std::string> outputs) {
        property_model_->add_method(function, inputs, outputs);
    }

    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>&& extract() {
        property_model_->initialize_stay_constraints();
        property_model_->initialize_solution_graph();
        return std::move(property_model_);
    }
private:
    using Implementation = PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;
    PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> property_model_;
};
