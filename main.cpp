#include <iostream>
#include "Person.hpp"

int main() {
    Person p1 = Person("Enter your name");
    std::cout << p1.greet() << std::endl;
    return 0;
}
