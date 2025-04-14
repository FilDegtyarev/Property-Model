#pragma once
#include "PropertyModel.h"

namespace NSPropertyModel::detail {
template<typename A, typename B, typename C>
class PMBuilder;

template<typename... DataArgs, typename... ValueArgs, typename... OutputArgs>
class PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> {
  private:
	using Implementation = PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;
	using PropertyModel_ = PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

  public:
	PMBuilder() noexcept = default;

	template<Belong B, typename T>
	void add_variable(std::string name, T value) {
		property_model_->template add_variable<B, T>(name, value);
	}

	void add_constraint(std::vector<std::string> variables, Priority priority, bool enable) {
		if (current_constraint_ != nullptr) {
			property_model_->add_constraint(std::move(current_constraint_));
		}

		std::vector<Variable*> refs = property_model_->bind_variables(variables);
		if (enable) {
			current_constraint_ = std::make_unique<Constraint>(refs, priority, Status::enabled);
		} else {
			current_constraint_ = std::make_unique<Constraint>(refs, priority, StatusType{0});
		}
		return;
	}

	template<typename... Types, typename... OutputTypes>
	void add_method(std::function<std::tuple<OutputTypes...>(Types...)>&& function, std::vector<std::string> inputs,
					std::vector<std::string> outputs) {
		std::function<void()> method = property_model_->bind_method(std::move(function), inputs, outputs);
		std::vector<Variable*> input_refs = property_model_->bind_variables(inputs);
		std::vector<Variable*> output_refs = property_model_->bind_variables(outputs);
		current_constraint_.get()->add_method(
			std::make_unique<Method>(method, input_refs, output_refs, current_constraint_.get()));
		return;
	}

	PropertyModel_&& extract() {
		if (current_constraint_ != nullptr) {
			property_model_->add_constraint(std::move(current_constraint_));
		}

		property_model_->add_stay_constraints();
		property_model_->prepare_solution_graph();
		return std::move(property_model_);
	}

  private:
	std::unique_ptr<Implementation> make_default_implementation() {
		return std::unique_ptr<Implementation>(new Implementation());
	}
	PropertyModel_ property_model_ = make_default_implementation();
	std::unique_ptr<Constraint> current_constraint_;
};
} // namespace NSPropertyModel::detail
