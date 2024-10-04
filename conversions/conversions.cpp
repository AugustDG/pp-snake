#include <iostream>

class Person {
private:
  std::string name;
  int age;

public:
  Person(const std::string& _name)
    :name(_name), age(-1) {
  };


  Person(const int _age) :
    name("Unknow"), age(_age) {
  };

  std::string get_name() {
    return name;
  }
  int get_age() {
    return age;
  }
  ~Person() {

  };
};

void print_person(Person& person) {
  std::cout << "Hi my name is " << person.get_name() << " I am " << person.get_age() << std::endl;

}
int main(int argc, char const* argv[]) {
  Person a("Ivan");
  Person b(2);
  // Person c = "Not Ivan";
  Person d = 3;

  print_person(a);
  print_person(b);
  // print_person(c);
  print_person(d);
  return 0;
}


