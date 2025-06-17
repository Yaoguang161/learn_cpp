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

-   `std::mutex`, `std::lock_guard`, `std::unique_lock` 等锁机制
-   `std::condition_variable` 条件变量
-   `std::future`, `std::async` 异步任务
-   `std::atomic` 原子操作支持

## 线程的管理

等待线程完成 join ()

##  启动线程

```C++
struct func
{
    int& i;
    func(int& i_) : i(i_) {}
    void operator() ()
    {
        for (unsigned j=0 ; j<1000000 ; ++j)
        {
            do_something(i); // 1 潜在访问隐患：空引用
        }
    }
};
void oops()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach(); // 2 不等待线程结束
} // 3 新线程可能还在运行
```

当 std::thread 对象销毁之前还没有做出决定，程序就会终止( std::thread 的析构函数会调用 std::terminate() )。因此，即便是有异常存在，也需要确保线程能够正确**汇入**(joined)或**分离**(detached)。

在分离线程在局部变量销毁后，仍在对该变量访问的情况，这种情况的常规处理方法：将数据复制到线程中。如果使用一个可调用的对象作为线程函数，这个对象就会复制到线程中，而后原始对象会立即销毁，以通过join()函数来确保线程在主函数完成前结束。

## 等待线程完成

等待线程需要使用`join()`

调用`join()`，还可以清理了线程相关的内存，这样 `std::thread` 对象将不再与已经完成的线`std::thread` 对象就不能再次汇入了。当对其使用`joinable()`时，将返回 false



## 特殊情况下的等待

需要对一个未销毁的 std::thread 对象使用join()或detach()。

*   如果想要分离线程，可以在线程启动后，直接使用detach()进行分离。

*   如果等待线程，则需要细心挑选使用join()的位置。

当在线程运行后产生的异常，会在join()调用之前抛出，这样就会跳过join()。避免应用被抛出的异常所终止。通常，在无异常的情况下使用join()时，需要在异常处理过程中调用join()，从而避免生命周期的问题。
