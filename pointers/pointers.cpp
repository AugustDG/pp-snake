#include <iostream>
#include <memory>

class Person {
private:
  std::string name;
  int age;

public:
  Person(const std::string& _name)
    :name(_name), age(-1) {
    std::cout << "Created Person" << std::endl;
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
    std::cout << "Deleted Person" << std::endl;
  };
};

void print_person(Person& person) {
  std::cout << "Hi my name is " << person.get_name() << " I am " << person.get_age() << std::endl;

}
int main(int argc, char const* argv[]) {
  {
    // std::unique_ptr<Person> p1 = new Person("p1"); // does not work
    std::unique_ptr<Person> p2(new Person("p2"));
    print_person(*p2); // print_person works because w

    // std::unique_ptr<Person> p3 = p2; // cannot make a copy of 
    // other way we can do initialize unique_ptr 
    std::unique_ptr<Person>p3 = std::make_unique<Person>(22);
    // main difference is exception safety
  }
  // outside of the scope, p2 cannot be accessed
  // print_person(p2);

  //shared ptr
  std::shared_ptr<Person> shared_p = std::make_shared<Person>("shared p");
  {
    std::shared_ptr<Person> new_shared_p = shared_p;
    print_person(*shared_p);
    print_person(*new_shared_p);
  }


  std::weak_ptr<Person> weak_p = shared_p; // does not increase ref count
  return 0;
}


