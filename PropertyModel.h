#pragma once
#include "PropertyModelImplementation.h"

namespace NSPropertyModel {

template<typename A, typename B, typename C>
class PropertyModel;

template<typename... DataArgs, typename... ValueArgs, typename... OutputArgs>
class PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> {
  private:
	using Implementation = detail::PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

  public:
	using Builder = detail::PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

	friend detail::PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

	Implementation* operator->() {
		return impl_.get();
	}

	const Implementation* operator->() const {
		return impl_.get();
	}
	void show(bool debug = false) const {
		assert(impl_.get());
		impl_.get()->show(debug);
	}

	template<typename T>
	void set(std::string name, T value) {
		assert(impl_.get());
		impl_.get()->set(name, value);
	}

  private:
	PropertyModel() noexcept = default;
	PropertyModel(std::unique_ptr<Implementation>&& impl) noexcept : impl_(std::move(impl)) {};
	std::unique_ptr<Implementation> impl_;
};
} // namespace NSPropertyModel
