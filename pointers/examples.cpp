#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <fstream>
#include <array>

// Example 1: unique_ptr for Resource Management in a Game Engine
class Texture {
public:
  Texture(const std::string& name) : name_(name) {
    std::cout << "Loading texture: " << name_ << std::endl;
  }
  ~Texture() {
    std::cout << "Destroying texture: " << name_ << std::endl;
  }
private:
  std::string name_;
};

class Entity {
public:
  Entity(const std::string& name, std::shared_ptr<Texture> texture)
    : name_(name), texture_(texture) {
  }

  void display() {
    std::cout << "Entity " << name_ << " is using the texture." << std::endl;
  }

private:
  std::string name_;
  std::shared_ptr<Texture> texture_;
};

void game_example() {
  std::shared_ptr<Texture> texture = std::make_shared<Texture>("character_texture");

  Entity player("Player", texture);
  Entity enemy("Enemy", texture);

  player.display();
  enemy.display();
  // Texture is shared between player and enemy
  // The texture will be destroyed automatically when all shared_ptr references go out of scope
}

// Example 2: shared_ptr for Task Management in a Thread Pool
void processTask(std::shared_ptr<int> task) {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::cout << "Processing task with value: " << *task << std::endl;
}

void thread_pool_example() {
  std::vector<std::thread> threadPool;
  std::shared_ptr<int> task = std::make_shared<int>(42);  // shared_ptr used to share the task among threads

  for (int i = 0; i < 5; ++i) {
    threadPool.emplace_back(processTask, task);  // Each thread shares the same task
  }

  for (auto& t : threadPool) {
    t.join();  // Wait for all threads to complete
  }

  // Task will be automatically cleaned up when the last shared_ptr (inside the threads) goes out of scope
}

// Example 3: weak_ptr for Observer Pattern to prevent circular references
class Observer {
public:
  virtual void onNotify() = 0;
  virtual ~Observer() = default;
};

class ConcreteObserver : public Observer {
public:
  void onNotify() override {
    std::cout << "Observer notified!" << std::endl;
  }
};

class Subject {
public:
  void addObserver(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
  }

  void notify() {
    for (auto& weakObserver : observers_) {
      if (auto observer = weakObserver.lock()) {  // Check if the observer is still valid
        observer->onNotify();
      }
    }
  }

private:
  std::vector<std::weak_ptr<Observer>> observers_;  // Using weak_ptr to avoid circular references
};

void observer_pattern_example() {
  std::shared_ptr<Subject> subject = std::make_shared<Subject>();
  std::shared_ptr<ConcreteObserver> observer = std::make_shared<ConcreteObserver>();

  subject->addObserver(observer);

  subject->notify();  // Notify the observer

  observer.reset();  // Observer is destroyed

  subject->notify();  // No notification since observer is gone
}

// Example 4: unique_ptr for File Management
struct FileCloser {
  void operator()(std::FILE* file) const {
    if (file) {
      std::cout << "Closing file." << std::endl;
      std::fclose(file);
    }
  }
};

void file_management_example() {
  std::unique_ptr<std::FILE, FileCloser> filePtr(std::fopen("example.txt", "w"));
  if (filePtr) {
    std::fputs("Hello, world!", filePtr.get());
  }
  // File automatically closed when filePtr goes out of scope
}

int main() {
  std::cout << "Game Example with shared_ptr:\n";
  game_example();
  std::cout << "\nThread Pool Example with shared_ptr:\n";
  thread_pool_example();
  std::cout << "\nObserver Pattern Example with weak_ptr:\n";
  observer_pattern_example();
  std::cout << "\nFile Management Example with unique_ptr:\n";
  file_management_example();

  return 0;
}


