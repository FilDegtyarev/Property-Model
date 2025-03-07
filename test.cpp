#include "property_model.cpp"

std::tuple<std::string> f(double a, double b) {
    std::string s = "a + b = " + std::to_string(a + b);
    return std::make_tuple(s);
}

void test1() {
    std::cout << "Test 1:\n";

    PMBuilder<Data<>, Value<double, double>, Output<std::string>> pmb;
    pmb.initialize_variable<Belong::OutputVariable, std::string>("output", "Default output");
    pmb.initialize_variable<Belong::ValueVariable>("a", 3.14);
    pmb.initialize_variable<Belong::ValueVariable>("b", 2.718);

    std::function function = f;
    
    pmb.add_constraint({"output", "a", "b"}, 1, true);
    pmb.add_method(function, {"a", "b"}, {"output"});

    PropertyModel pm = pmb.extract();

    pm.show(true);
}

std::tuple<std::string> add(std::string name, std::string surname) {
    return {name + " " + surname};
}

void test2() {
    std::cout << "Test 2:\n";
    
    PropertyModel<Data<>, Value<std::string, std::string>, Output<std::string>>::Builder pmb;
    pmb.initialize_variable<ValueVariable, std::string>("name", "Pavel");
    pmb.initialize_variable<ValueVariable, std::string>("surname", "Sokolov");
    pmb.initialize_variable<OutputVariable, std::string>("person", "unknown");

    std::function<std::tuple<std::string>(std::string, std::string)> add_method = add;
    pmb.add_constraint({"name", "surname", "person"}, 2, true);
    pmb.add_method(add_method, {"name", "surname"}, {"person"});
    
    auto pm = pmb.extract();
    
    std::cout << "Initial:\n";
    pm.show(true);


    pm->rough_execute(0, 0);
    pm->show(true);

    pm->rough_set<std::string>("name", "Dima");
    pm->rough_set<std::string>("surname", "Trushin");

    std::cout << "Dima Trushin check:\n";
    pm->rough_execute(0, 0);
    pm.show(true);
}


int main() {
    test1();
    test2();
    return 0;
}
