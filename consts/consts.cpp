#include <iostream>
#include <iostream>

class Person {
private:
  std::string name;
  int age;
  mutable int mut;


public:
  Person(const std::string& _name)
    :name(_name), age(-1) {
  };


  Person(const int _age) :
    name("Unknow"), age(_age) {
  };

  std::string get_name() const {
    // name = std::string("Other name");
    // mut = 1; // can be mu
    return name;
  }
  int get_age() {
    return age;
  }
  ~Person() {

  };
};

void print_person(const Person& person) {
  std::cout << "Hi my name is "
    << person.get_name()
    << " I am " << person.get_age() << std::endl;

}



int main(int argc, char const* argv[]) {
  const int RANDOM_NUMBER = 100;
  const int* a = new int;
  int* const b = new int;

  a = nullptr;
  b = nullptr;


  *a = 200; // 
  *b = 200;


  return 0;
}
