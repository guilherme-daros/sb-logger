# 0 "/home/geagaa/workspace/playground/antimony-logger/extern/antimony-types/examples/SingletonApp/Main.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "/home/geagaa/workspace/playground/antimony-logger/extern/antimony-types/examples/SingletonApp/Main.cpp"
# 1 "/home/geagaa/workspace/playground/antimony-logger/extern/antimony-types/include/types/Singleton.hpp" 1
       

namespace sb::types {

template <class T>
class Singleton {
 public:
  static auto Instance() -> T& {
    static T instance;
    return instance;
  }

 protected:
  Singleton() = default;
  ~Singleton() = default;

 public:
  Singleton(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  auto operator=(const Singleton&) -> Singleton& = delete;
  auto operator=(Singleton&&) -> Singleton& = delete;
};
}
# 2 "/home/geagaa/workspace/playground/antimony-logger/extern/antimony-types/examples/SingletonApp/Main.cpp" 2

class Example : public sb::types::Singleton<Example> {};

int main(int argc, char *argv[]) {
  auto &ex = Example::Instance();
  return 0;
}
