# 1. 单例模式

在C++11中提供一种方法，使得函数可以线程安全的只调用一次

```c++
#include <iostream>
#include <memory>
#include <mutex>

class Singleton {
public:
  static Singleton& GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
      instance_.reset(new Singleton);
    });

    return *instance_;
  }

  ~Singleton() = default;

  void PrintAddress() const {
    std::cout << this << std::endl;
  }

private:
  Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

private:
  static std::unique_ptr<Singleton> instance_;
};

std::unique_ptr<Singleton> Singleton::instance_;

int main() {
  Singleton& s1 = Singleton::GetInstance();
  s1.PrintAddress();

  Singleton& s2 = Singleton::GetInstance();
  s2.PrintAddress();

  return 0;
}


```

