#pragma once

#include <string>
class Person {
public:
  Person(std::string name);
  std::string greet();
  ~Person();
private:
  std::string name;
};

