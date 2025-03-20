template <typename... D>
struct Data{};

template <typename... D>
struct Value{};

template <typename... D>
struct Output{};

enum Belong {
    DataVariable,
    ValueVariable,
    OutputVariable,
    Default
};
