#include <iostream>
#include <functional>
#include <vector>

//... 在左边 define
//... 右边   use

template <class Self, class MemFn>
auto bind(Self* self, MemFn memfn) {
    return[self, memfn](auto ...t) {  //...在左边,属于定义形参包
        (self->*memfn)(t...);         //成员函数指针用法
        };
}

template <class ...T>  //...在左边表示定义一个形参包  使用大写T
struct Signal {
    std::vector<std::function<void(T...)>> m_callbacks; //...在右边表示使用一个形参包 使用大写T

    void connect(std::function<void(T...)> callback) {
        m_callbacks.push_back(std::move(callback));
    }

    template <class Self, class MemFn>
    void connect(std::function<void(T...)> callback) {
        m_callbacks.push_back(std::move(callback));
    }

    void emit(T... t) {  //使用大写T,  定义小写t
        for (auto&& callback : m_callbacks) {
            callback(t...);
        }
    }
};

struct Foo {
    void on_input(int i) {
        std::cout << "Foo of age " << age << " got " << i << '\n';
    }

    int age = 14;
};

struct Bar {
    void on_input(int i) {
        std::cout << "Bar of age " << age << " got " << i << '\n';
    }

    void on_exit(std::string msg1, std::string msg2) const {
        std::cout << "Bar got exit event: " << msg1 << " " << msg2 << '\n';
    }


    int age = 42;
};

struct Input {
    void main_loog() {
        int i;
        while (std::cin >> i) {
            on_input.emit(i);
        }
        on_exit.emit("hello", "world");
    }

    Signal<int> on_input;
    Signal<std::string, std::string> on_exit;
};


void (Bar::* memfn) (std::string, std::string);
void (*putfn)(std::string, std::string);

//#define FUN(fun) [&] (auto ...t) {return fun(t...);}
//万能引用版本
#define FUN(__fun) [&] (auto &&...__t) {return __fun(std::forward<decltype(__t)>(__t)...);}

////方法一
//void test(std::string msg1, std::string msg2) {
//    std::cout << "main received exit event: " << msg1 << " " << msg2 << '\n';
//}
//
//void test(int msg1, std::string msg2) {
//    std::cout << "main received exit event: " << msg1 << " " << msg2 << '\n';
//}

//方法二
struct Test {
    void operator() (std::string msg1, std::string msg2) {
        std::cout << "main received exit event: " << msg1 << " " << msg2 << '\n';
    }

    void operator() (int msg1, std::string msg2) {
        std::cout << "main received exit event: " << msg1 << " " << msg2 << '\n';
    }
} test;  //定义成对象

int main()
{
    Input input;
    Bar bar;
    Foo foo;

    input.on_input.connect([&](auto ...t) {foo.on_input(t...); });
    //input.on_input.connect(foo.on_input); 这样不允许, 但是下面的套皮就可以
    input.on_input.connect(FUN(foo.on_input));
    input.on_input.connect(bind(&foo, &Foo::on_input));
    input.on_input.connect(bind(&bar, &Bar::on_input));
    input.on_exit.connect(bind(&bar, &Bar::on_exit));

    //定义一个全局函数,如果直接调用,无法判断是哪个,但是套皮就可以
    //input.on_exit.connect(test);  //如果使用struct 定义成对象就可以 见方法二
    //input.on_exit.connect(static_cast<void(*)(std::string, std::string)>(test));  //方法三
    input.on_exit.connect(FUN(test));  //套皮法,最简单   方法一


    input.on_exit.connect([&](std::string msg1, std::string msg2) {
        std::cout << "main received exit event: " << msg1 << " " << msg2 << '\n';
        });

    input.main_loog();   //循环输入
}
