#pragma once
#include "PropertyModelImplementation.h"

namespace NSPropertyModel {

namespace detail {
class PMDebug {};
class PMRelease {};
} // namespace detail

constexpr inline detail::PMDebug Debug = detail::PMDebug();
constexpr inline detail::PMRelease Release = detail::PMRelease();

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
		assert(impl_.get());
		return impl_.get();
	}

	const Implementation* operator->() const {
		assert(impl_.get());
		return impl_.get();
	}

  private:
	PropertyModel() noexcept = default;
	PropertyModel(std::unique_ptr<Implementation>&& impl) noexcept : impl_(std::move(impl)) {};
	std::unique_ptr<Implementation> impl_;
};
} // namespace NSPropertyModel
