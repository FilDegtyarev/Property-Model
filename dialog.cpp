#include "Builder.h"

std::tuple<int> absolute_w(int relate_width, int initial_width) {
    return std::make_tuple(initial_width * relate_width / 100);
}

std::tuple<int> relative_w(int absoulute_width, int initial_width) {
    return std::make_tuple((absoulute_width * 100) / initial_width);
}

std::tuple<int> absolute_h(int relate_height, int initial_height) {
    return std::make_tuple(initial_height * relate_height / 100);
}

std::tuple<int> relative_h(int absolute_height, int initial_height) {
    return std::make_tuple((absolute_height * 100) / initial_height);
}

std::tuple<int> preserve_relative_h(int relative_w) {
    return relative_w;
}

std::tuple<int> preserve_relative_w(int relative_h) {
    return relative_h;
}

std::tuple<std::pair<int, int>> output(int absolute_h, int absolute_w) {
    return std::make_tuple<std::pair<int, int>>({absolute_h, absolute_w});
}

void dialog_test() {
    using DataVars = Data<int, int>;
    using ValueVars = Value<int, int, int, int, bool>;
    using OutputVars = Output<std::pair<int, int>>;
    using Builder = PropertyModel<DataVars, ValueVars, OutputVars>::Builder;
    Builder pmb;

    std::function<std::tuple<int>(int, int)> abs_w = absolute_w;
    std::function<std::tuple<int>(int, int)> abs_h = absolute_h;

    std::function<std::tuple<int>(int, int)> rel_w = relative_w;
    std::function<std::tuple<int>(int, int)> rel_h = relative_h;
    
    std::function<std::tuple<int>(int)> pr_w = preserve_relative_w;
    std::function<std::tuple<int>(int)> pr_h = preserve_relative_h;

    std::function<std::tuple<std::pair<int, int>>(int, int)> out = output;

    pmb.add_variable<DataVariable, int>("initial_h", 1000);
    pmb.add_variable<DataVariable, int>("initial_w", 2000);

    pmb.add_variable<ValueVariable, int>("relative_h", 100);
    pmb.add_variable<ValueVariable, int>("absolute_h", 1000);
    pmb.add_variable<ValueVariable, int>("relative_w", 100);
    pmb.add_variable<ValueVariable, int>("absolute_w", 2000);

    pmb.add_variable<OutputVariable, std::pair<int, int>>("output", {1000, 2000});

    pmb.add_constraint({"relative_w", "initial_w", "absolute_w"}, 0, true);
    pmb.add_method(abs_w, {"relative_w", "initial_w"}, {"absolute_w"});
    pmb.add_method(rel_w, {"absolute_w", "initial_w"}, {"relative_w"});

    pmb.add_constraint({"relative_h", "initial_h", "absolute_h"}, 0, true);
    pmb.add_method(abs_w, {"relative_h", "initial_h"}, {"absolute_h"});
    pmb.add_method(rel_w, {"absolute_h", "initial_h"}, {"relative_h"});
    
    pmb.add_constraint({"relative_h, relative_h"}, 0, false);
    pmb.add_method(pr_h, {"relative_w"}, {"relative_h"});
    pmb.add_method(pr_w, {"relative_h"}, {"relative_w"});

    auto pm = pmb.extract();
    pm.show();
}

int main() {
    dialog_test();
}
