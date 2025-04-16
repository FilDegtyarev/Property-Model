#pragma once
namespace NSPropertyModel {
template<typename... D>
struct Data {};

template<typename... D>
struct Value {};

template<typename... D>
struct Output {};

enum class Belong { DataVariable, ValueVariable, OutputVariable };
} // namespace NSPropertyModel
