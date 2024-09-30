#include "Person.hpp"

Person::Person(std::string _name) {
  name = _name;
}

Person::~Person() {
}

std::string Person::greet() {
  std::string greeting = "Hi my name is " + name;
  return greeting;
}