# C++多线程开发笔记

## C++11之后的线程创建方式

普通函数创建线程

```C++
#include <iostream>
#include <thread>

void threadFunc() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::thread t(threadFunc);  // 启动线程
    t.join();                   // 等待线程结束
    return 0;
}

```

lambda方式

```C++
int main() {
    std::thread t([] {
        std::cout << "Hello from lambda thread!" << std::endl;
    });
    t.join();
    return 0;
}

```

使用类的成员函数

```C++
class MyClass {
public:
    void run() {
        std::cout << "Hello from member thread!" << std::endl;
    }
};

int main() {
    MyClass obj;
    std::thread t(&MyClass::run, &obj); // 注意传入对象指针
    t.join();
    return 0;
}

```

C++11 同时引入了：

- `std::mutex`, `std::lock_guard`, `std::unique_lock` 等锁机制
- `std::condition_variable` 条件变量
- `std::future`, `std::async` 异步任务
- `std::atomic` 原子操作支持





## 线程的管理

等待线程完成 join ()
