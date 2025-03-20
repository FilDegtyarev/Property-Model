#pragma once
#include "PropertyModelImplementation.h"

template <typename A, typename B, typename C>
class PropertyModel;

// В дальнейшем методов rough_* не будет - они содержатся для базовой демонстрации, того, что происходит
template <typename...DataArgs, typename...ValueArgs, typename...OutputArgs>
class PropertyModel<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>> {
private:
    using Implementation = PropertyModelImpl<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;
public:
    using Builder = PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

    friend PMBuilder<Data<DataArgs...>, Value<ValueArgs...>, Output<OutputArgs...>>;

    PropertyModel(PropertyModel&&) = default;
    PropertyModel& operator=(PropertyModel&&) = default;
    
    Implementation* operator->() { return impl_.get(); }

    const Implementation* operator->() const { return impl_.get(); }
    void show(bool debug=false) const {
        impl_.get()->show(debug);
    }

    template <typename T>
    void set(std::string name, T value);

private:
    PropertyModel() = default;
    PropertyModel(std::unique_ptr<Implementation>&& impl): impl_(std::move(impl)) {};
    std::unique_ptr<Implementation> impl_;
};
