# IEEE Advanced C/C++ Workshop
## Installation
Make sure to install all build dependencies:
### Linux
```shell
sudo apt install build-essential cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev -y
```
### Windows
Install using the (Windows installer)[https://cmake.org/download/]

### MacOS
If you don’t have Homebrew installed, you can install it by opening the Terminal and running the following command:
```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
This will install Homebrew. Follow the prompts during installation.

Step 2: Install CMake using Homebrew
Once Homebrew is installed, open the Terminal and run the following command to install CMake:
```shell
brew install cmake
```

# Objectives

<aside>
❓

C++ development is a very large and we don’t claim to be experts, nor are we gonna cover EVERYTHING about the language.

</aside>

**Our aim today is :** 

- Cover advanced topics of cpp such as
    - CMake
    - Special keywords in c++ that will make your life easier and faster (auto, constexpr, etc…)
    - Vectors vs C arrays
    - Smart pointers vs c-pointers
    - Debugging with core files (GDB)
- Use the topics covered to make a simple game
- Have FUN doing it!

# Section 1 : Concepts

## **Header Files: `.h` vs `.hpp`  vs `.inc`**

- `.h` is more ambiguous, as it can be used for both C and C++ header files.
- `.hpp` is more explicit, indicating a C++specific header file (by convention).

Some build systems and IDEs use the extension to determine how to process the file, especially for syntax highlighting and code completion.

Projects that include both C and C++ code, using `.hpp` for C++ headers and `.h` for C headers helps prevent confusion and potential compilation issues.

When integrating with third-party libraries or APIs written in C, maintaining the `.h` extension for C headers aids in compatibility and clarity.

## Namespaces

### **Why `using namespace std` Is generally Discouraged**

```cpp
// avoid using
using namespace std;

// instead use 
using std::string, std::cout // etc
```

- Naming Conflicts

```cpp
#include <iostream>
#include <vector>

using namespace std;

int cout = 100;  // This conflicts with std::cout

int main() {
    cout << "Hello";  // Error: The local 'cout' variable shadows std::cout
    return 0;
}
```

- `using namespace std;`, you're essentially pulling the entire contents of the `std` namespace into the global namespace, which increases the chances of accidental conflict

## Values initialization vs Default Initialization

```cpp
// default initialization
int x;
int *x2 = new int;

// direct initialization 
int y{};
int *y2 = new int{};
int *y3 = new int();
```

- Default Initialization
    - For primitives like `int`, `double`, etc., if you don't initialize it, the value of `x` will be **indeterminate** (essentially garbage data; for most compilers). In practice, this means that `x` could contain any random value.
    - The `new int` expression allocates memory for an integer on the heap (free store), and `x2` is a pointer to that memory. The newly allocated integer is also **default initialized**, which means it has an indeterminate value, just like `x`.
- Direct Initialization
    - For primitives, value initialization will **set the value to zero**. In this case, `y` is guaranteed to be initialized to `0`.
    - The expression `new int{}` allocates memory for an integer and initializes it to **0**. So, the memory that `y2` point.

### `struct` vs `class`

They can often be used interchangeably in C++, but there are key differences in their typical use cases.

## When to use Struct

Ideal for simple data structures without methods or constructors, maintaining consistency with C-style programming.

### Access modifiers

By default, members of a struct are public, whereas members of a class are private.

## Implicit Conversion and the `explicit` Keyword

In C++, implicit conversion occurs when the compiler automatically converts types to match function parameters or constructor arguments, even when they don't exactly align with the expected types.

Let's take a look at the following example:

```cpp
// section/conversions.cpp
#include <iostream>

class Person {
private:
  std::string name;
  int age;

public:
  // Constructor that takes a string
  Person(const std::string& _name)
    : name(_name), age(-1) {
  };

  // Constructor that takes an integer
  Person(const int _age)
    : name("Unknown"), age(_age) {
  };

  // Getters
  std::string get_name() const {
    return name;
  }

  int get_age() const {
    return age;
  }

  ~Person() {}
};

void print_person(const Person& person) {
  std::cout << "Hi, my name is " << person.get_name() << " and I am " << person.get_age() << " years old." << std::endl;
}

int main() {
  Person a("Ivan");   // Matches the Person(const std::string&) constructor
  Person b(2);        // Matches the Person(const int) constructor
  // Person c = "Not Ivan"; // This would cause an implicit conversion error
  Person d = 3;       // Implicit conversion from int to Person

  print_person(a);
  print_person(b);
  // print_person(c);  // Uncommenting this would cause a compilation error
  print_person(d);

  return 0;
}
```

In the above code:

- For `Person a("Ivan")`, the compiler calls the `Person(const std::string&)` constructor directly.
- For `Person b(2)`, the `Person(const int)` constructor is called.
- For `Person d = 3;`, the compiler **implicitly** converts the integer `3` to a `Person` object using the `Person(const int)` constructor. This is equivalent to writing `Person d = Person(3)`.

### Preventing Implicit Conversion

Sometimes, implicit conversions can lead to unexpected behavior. To prevent implicit conversions, you can use the `explicit` keyword. This forces the programmer to use **explicit** conversions, making the code clearer and reducing the chance of errors.

```cpp
class Person {
  // Constructors marked as explicit
public:
  explicit Person(const std::string& _name)
    : name(_name), age(-1) {
  };

  explicit Person(const int _age)
    : name("Unknown"), age(_age) {
  };
};
```

Now, the following code will no longer compile:

```cpp
Person d = 3; // Error: Implicit conversion is not allowed
```

Instead, you'll need to explicitly create the `Person` object like this:

```cpp
Person d(3) ; // Explicit declaratin 
```

## Pointers

Just because you know how to use pointers, doesn’t mean you know how to use pointers.

**Raw pointers**

- Memory leaks
- Ownership issues

```cpp
int* ptr = new int(10); // Dynamically allocate memory for an int, set to 10
std::cout << *ptr << std::endl; // Output: 10

delete ptr; // Manually free the memory
ptr = nullptr; // Avoid dangling pointer by setting to nullptr
```

- **`new int(10);`** allocates memory on the heap and initializes it with `10`.
- **`delete ptr;`** releases the allocated memory.
- **`ptr = nullptr;`** avoids the pointer referencing freed memory (a dangling pointer).

### Use cases of shared_ptrs

### Game Example with shared_ptr

We use a shared_ptr to manage a texture resource (Texture class) shared between multiple entities (like a player and an enemy). The texture memory is freed automatically when all shared_ptr instances go out of scope.

### Thread Pool Example with shared_ptr

A shared_ptr is used to share a task (in this case, an int) among multiple threads. Each thread processes the task, and the memory is automatically cleaned up when the reference count reaches zero.

### Observer Pattern Example with weak_ptr

In the Observer pattern, weak_ptr is used to avoid circular dependencies between the Subject and its Observer objects. This prevents memory leaks while allowing the subject to notify active observers.

### File Management Example with unique_ptr

A unique_ptr is used to manage a file handle (FILE*). The file is automatically closed when the unique_ptr goes out of scope, ensuring safe resource management.

## Const Correctness and “constexpr”

Would you rather catch errors during **compile time** or **runtime**? Ideally, you want to catch bugs as early as possible—at **compile time**—to make your code more reliable and easier to maintain.

**If something won’t change in the current scope, mark it as `const`**

```cpp
std::string get_name() const { // 'const' makes this member function read-only
    name = std::string("Other name"); // This will cause a compile-time error
    return name;
}
```

The lack of protection can lead to unintended side effects, especially when passing objects by reference.

This version allows `print_person` to modify the passed `Person` object, which may not be what you want. Instead, using a `const` reference ensures that the `Person` object remains unmodified:

```cpp
void print_person(const Person& person) {
    std::cout << "Hi, my name is " << person.get_name() << " and I am " << person.get_age() << std::endl;
}
```

Now, the caller knows that the function will **not modify** the passed object (which is super useful when dealing with parallel computing, i.e. multithreading).

### What if `age` was a pointer?

You can return a pointer in a safe manner by combining multiple `const` qualifiers:

```cpp
const int* const get_age() const {
    return age;
}
```

This ensures that:

1. The **pointer itself** cannot be modified (`const int* const`)
2. The **value pointed to** cannot be modified (`const int*`)
3. The **method itself** will not modify any member variables (`const` at the end)

### `mutable`

What if you need to modify a member variable, but still want the method to be `const`? You can use the `mutable` keyword to allow modification of specific member variables, even within `const` methods:

```cpp
class Person {
private:
  std::string name;
  mutable int age;

public:
  // Other methods...
};
```

In this case, `age` can be modified even if the method is marked `const`, while `name` remains immutable.

### `constexpr`

The `constexpr` keyword allows you to compute values at compile time (through functions or pre-defined variables).

Let’s look at a simple example using **Compiler Explorer** (https://godbolt.org/):

```cpp
int add(int a, int b) {
    return a + b;
}

int main() {
    int i = add(2, 2);
    return 0;
}
```

Generated assembly code:

```nasm
add(int, int):
    push    rbp
    mov     rbp, rsp
    mov     DWORD PTR [rbp-4], edi
    mov     DWORD PTR [rbp-8], esi
    mov     edx, DWORD PTR [rbp-4]
    mov     eax, DWORD PTR [rbp-8]
    add     eax, edx
    pop     rbp
    ret
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 16
    mov     esi, 2
    mov     edi, 2
    call    add(int, int)  // Function call required
    mov     DWORD PTR [rbp-4], eax
    mov     eax, 0
    leave
    ret
```

The function call is executed at runtime. However, if we mark the `add` function as `constexpr`, the compiler can optimize the calculation and eliminate the function call altogether:

```cpp
constexpr int add(int a, int b) {
    return a + b;
}

int main() {
    constexpr int i = add(2, 2);
    return 0;
}
```

Generated assembly code:

```nasm
main:
    push    rbp
    mov     rbp, rsp
    mov     DWORD PTR [rbp-4], 4 // No function call, result is computed at compile time
    mov     eax, 0
    pop     rbp
    ret
```

In this case, the result of `add(2, 2)` is computed at compile time, and there is no need for a function call at runtime. This is especially useful when working with recursion or other complex compile-time calculations.

**Note**: `constexpr` works well in situations where values can be computed at compile time, but the compiler might still call the function at runtime if certain conditions aren’t met.

# Arrays

### **Static array**

Using a raw C array:

```cpp
int data[5];  // Static array of 5 integers
data[0] = 1;  // Assign value to first element
data[4] = 10; // Assign value to fifth element
```

Using a C++ `std::array` :

```cpp
#include <array>
std::array<int, 5> data;  // Create an array of 5 integers
data[0] = 1;  // Assign value to the first element
data[4] = 10; // Assign value to the fifth element
```

**Advantages of std::array<>**

```cpp
data.at(10);  // Throws std::out_of_range if index is invalid
std::cout << data.size();  // tracks size

// range based loop
for (const auto& value : data) {
    std::cout << value << " ";
}
```

`std::array` support a lot of stl algorithms

- **`all_of`**: Returns `true` if all elements in the range satisfy a condition.
- **`any_of`**: Returns `true` if any element in the range satisfies a condition.
- **`none_of`**: Returns `true` if none of the elements in the range satisfy a condition.
- **`for_each`**: Applies a function to each element in the range.
- **`count`**: Counts the number of elements that are equal to a specified value.
- **`count_if`**: Counts the number of elements that satisfy a condition.
- **`mismatch`**: Compares two ranges and returns the first mismatch.
- **`equal`**: Returns `true` if two ranges are equal.
- **`find`**: Finds the first occurrence of a value in a range.
- **`find_if`**: Finds the first element that satisfies a condition.
- **`adjacent_find`**: Finds the first occurrence of two consecutive equal elements.
- **`search`**: Finds a subrange in a range.
- **`search_n`**: Finds the first occurrence of `n` consecutive elements in a range that are equal to a value.
- **`copy`**: Copies elements from one range to another.
- **`copy_n`**: Copies exactly `n` elements from one range to another.
- **`copy_if`**: Copies elements that satisfy a condition from one range to another.
- **`move`**: Moves elements from one range to another.
- **`transform`**: Applies a function to a range and stores the result in another range.

# Vectors

### **Heap vs Stack Memory**

In C++, a `std::vector` allocates memory dynamically on the heap, which is generally slower compared to stack memory. The stack is faster because its allocation is simpler and does not involve the overhead of dynamic memory management (heap).

### Non performant  code

One common pitfall when using `std::vector` is overlooking the performance cost of operations like `push_back`. When you `push_back` an element into a vector, the vector may need to reallocate memory to accommodate the new element. This results in extra overhead.

```cpp
std::vector<int> my_vector;
my_vector.push_back(42); // This operation could cause a reallocation if the capacity is exceeded.
```

*How much allocation happens with this code?*

```cpp
std::vector<int> my_vector;
my_vector.push_back(42);
my_vector.push_back(42);
my_vector.push_back(42);
my_vector.push_back(42);
```

To minimize the performance hit of reallocations, you should **reserve** capacity for your vector when you know the approximate size:

```cpp
std::vector<int> my_vector;
my_vector.reserve(4);  // Reserves space for 4 elements, 
```

### L-Values

An **l-value** is an expression that refers to an object with a persistent memory location. It can be thought of as something that has a name or an address and exists beyond a single statement. You can assign a new value to an l-value (hence the term "l-value" for "left-hand side").

```cpp
int x = 5;      // 'x' is an l-value because it has a persistent address in memory.
std::string str = "Hello";  // 'str' is an l-value because it persists beyond the expression.
```

Here, `push_back()` copies the `word` into the vector, which means the string `word` is duplicated—one in the vector and one in its original location. This copy operation can be expensive if the object is large.

```cpp
std::string word = "Hello";
my_vector.push_back(word);  // 'word' is an l-value.
```

### **R-Values**

An **r-value**, on the other hand, refers to temporary objects or values that do **not** have a persistent memory address. These are values that exist only during a single expression and are typically discarded after the expression is evaluated.

```cpp
int y = 5 + 10;  // '5 + 10' is an r-value because it's a temporary result.
std::string temp = "Hello" + " World";  // The result of this concatenation is an r-value.
```

R-values are temporary and cannot be assigned a new value (you cannot do something like `5 = x`). In the context of vectors, **r-values** are extremely useful because they allow you to avoid copying temporary objects. By moving an r-value, you take ownership of its data rather than duplicating it.

## Move Semantics and `std::move`

C++11 introduced **move semantics** to optimize the handling of temporary objects (r-values). Move semantics allows you to transfer ownership of resources (like memory) from one object to another, rather than copying the entire object. This is especially useful in performance-critical situations, such as when working with vectors or other containers that manage large objects.

To explicitly indicate that you want to move an object (i.e., treat an l-value as an r-value), you can use the `std::move` function. This allows you to avoid deep copying and use the existing resources of the temporary object.

```cpp
std::string word = "Hello";
my_vector.push_back(std::move(word));  // Move 'word' into the vector instead of copying it.
```

When dealing with large objects or temporary values, using r-values and move semantics in conjunction with vectors can help you significantly optimize performance.

```cpp
std::vector<std::string> sentences;
std::string sentence = "This is a long sentence.";

sentences.push_back(std::move(sentence)); // Moves the 'sentence' into the vector.
```

You can go one step further by using `emplace_back` when constructing objects directly inside a vector, thus avoiding both copies and moves in some cases.
```cpp
std::vector<std::string> my_vector;
my_vector.emplace_back("Directly placed string");  // No copying, no moving; the string is constructed in place.
```
# Section 2 : Building

> C++ is a compiled language
> 

**From source code to machine code**

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/4a58a8f9-adf4-4950-892d-de228d10e2da/90253ae1-d162-48af-bb4d-d5d4e628aa73/image.png)

### **What is CMake and Why Use It?**

Introduction to build systems and CMake’s role.

### Example without using CMake

```bash
g++ main.cpp Person.cpp -o Hello
```

This command compiles two source files (`main.cpp` and `Person.cpp`) and links them into a single executable (`Hello`).

<aside>
❓

**What happens when you have 10 or more classes referencing each other?**

- **Dependency Management**:
    - If one of the classes changes, you'd need to recompile only the relevant parts of the project manually.
    - Pain in the A**

**The manual build command might work fine on Linux with `g++`, but what if you are sharing your code with Windows or MacOS user?**

</aside>

### How are these issues solved?

- Makefile (Linux)
- Ninja build system (linux)
- IDE built in solution (Visual Studio Windows)
    - Problem is you’re trapped
- CMake

### **Cross-Platform Development with CMake**

CMake solves many of these issues:

- **Automates Compilation and Linking**: CMake automatically detects which files need to be compiled and how they should be linked, saving you from manually managing large numbers of files.
- **Handles Dependencies**: If a single file is updated, CMake only recompiles the necessary parts of the project instead of everything.
- **Cross-Platform Builds**: CMake generates platform-specific build files, allowing you to target Windows, macOS, Linux, and more without modifying the core project structure. For example:
    - On Linux, CMake can generate `Makefiles`.
    - On Windows, CMake can generate Visual Studio project files.
    - On macOS, CMake can generate Xcode project files.

**Additional reason to learn**

- Any big project you’ll work on/use uses cmake
- It does legitemately make things
- When you’re not using your computer
- Wide spread tooling support

### **Basic CMake Structure**

- Explanation of `CMakeLists.txt` structure
- Basic commands (`cmake_minimum_required`, `project`, `add_executable`)

```makefile
# Specify the minimum CMake version required
# Newer version may introduce new features
cmake_minimum_required(VERSION 3.11)

# Define the project name and language (C, C++, etc.)
project(game)

# Specify an executable target and its source files
add_executable(
${PROJECT_NAME} main.cpp)
```

The project name is an identifier your project

### Build

```makefile
cmake -B build 
cd build
make 
```

The `-B` option tells CMake to specify an **output build directory**. In this case, the directory is named `build`. CMake will create the directory (if it doesn't already exist) and place all generated build files in this directory.

- **Why a build dir?**
    - This helps keep your source directory clean because all the build-related files are kept separate in the `build` directory.

### Importing Libraries with CMake

```cpp
# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif ()
endif ()

# Our Project
add_executable(${PROJECT_NAME} src/main.cpp)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif ()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()
```

### Introduction to raylib and Game Loops

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/4a58a8f9-adf4-4950-892d-de228d10e2da/743ea815-2e8a-40f9-9313-000688dd2d2c/image.png)

```cpp
// src/main.cpp
const int screenWidth = 1600;
const int screenHeight = 900;

InitWindow(screenWidth, screenHeight, "IEEE C++ Workshop!");
SetTargetFPS(60);
while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Where our game logic will go 
    DrawRectangle(350, 250, 100, 100, RED);
    DrawText("Hello, Snake!", 10, 10, 20, BLACK);
    EndDrawing();
}
CloseWindow();
```

- **`InitWindow(int width, int height, const char *title)`**
    - Initializes and opens a window with the specified dimensions and title.
    - Sets up the rendering context.
- **`SetTargetFPS(int fps)`**
    - Sets the target frame rate for the game.
    - Helps to control the game's speed and resource usage.
- **`while (!WindowShouldClose())`**
    - The main game loop continues until the window is closed.
    - `WindowShouldClose()` checks if the user has requested to close the window (e.g., by clicking the close button or pressing `Escape`).
- **`BeginDrawing()` and `EndDrawing()`**
    - Marks the beginning and end of the drawing phase each frame.
    - All rendering code goes between these two functions.
- **`ClearBackground(Color color)`**
    - Clears the screen with the specified background color before drawing new frames.
- **`CloseWindow()`**
    - Closes the window and releases resources allocated by Raylib.

### Public and Private Include

```cpp
// include/settings.h
#pragma once

#include <cstdint>

constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1600;
constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 900;
```

```cpp
// src/main.cpp
InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "IEEE C++ Workshop!");
```

```cpp
// CMakeList.txt
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} raylib)
```

All our private includes goes in **`src/include/*`**  

```cpp
//src/include/
```

# Section 3

- Initializes the window and sets the frame rate.
- Enters the main game loop, which does three key things:
    1. **Update**: Checks for user input, updates the game state (e.g., snake movement).
    2. **Render**: Draws the game on the screen (snakes, apples, etc.).
    3. **Cleanup**: Handles the end of the game, and if the player hits a key, restarts.

### Snake Logic

- **Movement**: Each snake consists of a body made up of segments, and the head moves in the direction the player has selected.
- **Growth**: When a snake eats an apple, it grows by adding a new segment to its body.
- **Collision**: The snake can collide with itself, other snakes, or the edges of the game map. If a collision occurs, the game ends.
- **Resetting**: When the game ends, the snakes are reset to their starting positions.

### Game Logic

- **Initialization**: Initializes the snakes at specific positions on the game map, with set colors and directions.
- **Input Handling**: Detects key presses for both Player 1 (W, A, S, D) and Player 2 (arrow keys) to control their respective snakes' movements.
- **State Update**: Each frame, the snakes move in the direction they're currently facing. It also handles collisions, score updates, and checks for game over conditions.
- **Rendering**: This part of the code draws the snakes, apples, and "Game Over" screen using Raylib.
