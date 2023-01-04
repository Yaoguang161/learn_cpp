* [1.如何实现线程安全](#1-C++如何实现线程安全)

* [2.需要一个指针,指向一个"元素类型为int"的vector](#2.一个指针指向"元素类型为int" 的vector)

* [3. RAII与智能指针](#3.RAII与智能指针)

* [4.lambda表达式替换案例](#4.lambda表达式替换案例)
* [5.自定义构造函数](#5. 自定义构造函数)
  * [5.1三五法则](#5.1三五法则)








# 1. C++如何实现线程安全

实现线程安全通常有一下集中方法:

1. 使用互斥锁(mutex) : 互斥锁是一种用于保护临界区的同步对象, 它可以防止多个线程同时访问临界区. C++中的互斥锁可以使用 `std::mutex`或者`std::lock_guard`来实现

2. 使用信号量(semaphore): 信号量是一种用于保护临界区的同步对象,它能限制线程对临界区的访问次数.C++中的信号量可以使用`std::semaphore`来实现.

3. 使用读写锁(reader-writer lock) : 读写锁是一种用于保护临界区的同步对象, 它允许多个线程同时读写临界区, 但只允许一个线程写入临界区.C++中的读写锁可以使用`std::shared_mutex`或`std::shared_lock`来实现

4. 使用原子操作(atomic operation): 原子操作是一种在单个操作内完成的操作, 它不会被其他线程打断. C++的原子操作可以使用`std::atomic`来实现.

   

#  2.一个指针指向"元素类型为int" 的vector

````C++
vector<int> *pv  = 0;
pv = &fibonacci;
```
pv = &lucas;
// 这种赋值方式会牺牲程序的透明性
````

另一种方案将每个数列的内存地址存入某个vector中,可以通过索引的方式,透明的访问这些数

```c++
const int seq_cnt = 6;

// 一个指针数组, 大小为seq_cnt
//每个指针都指向vector<int>对象
vector<int> *seq_addres[ seq_cnt ] ={
	&fibonacci, &lucas, &pell , 
	&triangular, &square, &pentagonal
}
```

这样`seq_addrs[0]`所持有的`fibonacci  vector`的地址, `seq_addrs[1]`的值是`lucas vector`的地址

```C++
vector <int> * current_vec = 0;
// ````

for(int ix = 0; ix < seq_cnt; ++ ix){
    current_vec = seq_addrs[ix];
    //要显示的元素都通过current_vec间接访问到
}
```





# 3.RAIA与智能指针

# 4. lambda表达式替换案例

```C++
#include<vector>
#include<iostream>
#include<algorithm>

int sum = 0;
void func(int vi){
    sum += vi;
}
int main(){
    std::vector<int> v = {4,3,2,1};
    
    std::for_each(v.begin(),v.end(),func);
    std::cout<< sum << std::endl;
    return 0;
}


//需要定义一个全局的func比较麻烦
//C++11引入lambda表达式
#include<vector>
#include<iostream>
#include<algorithm>

int main(){
    std::vector<int> v = {4,3,2,1};
    
    int sum = 0;
    std::for_each(v.begin(),v.end(),[&](auto vi){
        sum += vi;
    });
    std::cout<< sum << std::endl;
    return 0;
}

```





# 5. 自定义构造函数

```C++
#include<iostream>
#include<stirng>

struct Pig{
    std::string m_name;
    int m_weight;
     // 只用初始化一次
    Pig() : m_name("佩奇"),m_weigth(80)
    {}
    
    /*
    Pig(){
    	m_name  = "佩奇";
    	m_weight = 80;
    }
    */
};

int main(){
    Pig pig;
    std::cout <<  "name: " << pig.m_name << std::endl;
    std::cout <<  "weight: " << pig.m_weight << std::endl;
    return 0;
}
```



自定义构造函数: 多个参数

```C++
#include<iostream>
#include<string>


struct Pig{
    std::string m_name;
    int m_weight;
     // 只用初始化一次
    Pig(std::string name, int weight) : m_name(name),m_weigth(weight)
    {}

};
    
int main(){
   Pig pig("佩奇",80);
   std::cout <<  "name: " << pig.m_name << std::endl;
   std::cout <<  "weight: " << pig.m_weight << std::endl;
   return 0;
}
```

编译器默认生成的构造函数: 初始化列表()

```C++
#include<iostream>
#include<string>
struct Pig{
    std::string m_name;
    int m_weight;
};

void show(Pig pig){
    std::cout << "name: " << pig.m_name << std::endl;
    std::cout << "weight: " << pig.m_weigth << std::endl;
}

int main(){
    Pig pig1 = {"佩奇" , 80}; // 编译通过
    Pig pig2 {"佩奇", 80};   //编译通过
    //Pig pig3("佩奇", 80); //编译错误 !
    
    show(pig1);
    return 0;
}
```

拷贝构造函数: 用户自定义

```C++
struct Pig{
    std::string m_name;
    int m_weight{0};
    
    Pig(Pig const &other)
        : m_name(other.m_name),m_weight(other.m_weight)
        {}
}
```

不想要编译器自动生成拷贝构造函数怎么办: = delete

```C++
struct Pig{
    std::string m_name;
    int m_weight{0};
    
    Pig() = default;
    
    Pig(std::string name, int weight) : m_name(name), m_weight(weight){}
    
    Pig(Pig const &) = delete; //禁止拷贝构造Pig对象
    Pig &operator=(Pig const &) = delete; //禁止拷贝赋值
    
    //例如 
    //Pig pig = pig2; //拷贝构造
    //
    //Pig pig //无参构造
    //pig = pig2; //拷贝赋值
    
}
```

## 5.1 三五零法则

1. 如果一个类定义了**析构函数**, 那么必须同时定义或删除**拷贝构造函数**和**拷贝赋值函数,**否则出错.
2. 如果一个类定义了**拷贝构造函数**,那么您必须同时定义或删除**拷贝赋值函数**,否则出错,删除可导致低效.
3. 如果一个类定义了**移动构造函数**, 那么您必须同时定义或删除**移动赋值函数**,否则出错,删除可导致低效.
4. 如果一个类定义了**拷贝构造函数**或**拷贝赋值函数**,那么您最好同时定义**移动构造函数**或**移动赋值函数**,否则低效.



![image-20230104215907008](C:\Users\Asaki\AppData\Roaming\Typora\typora-user-images\image-20230104215907008.png)

![image-20230104215930751](C:\Users\Asaki\AppData\Roaming\Typora\typora-user-images\image-20230104215930751.png)



举例子

```C++
struct C{
    C();                         //默认构造函数
    
    C(C const &c);               //拷贝构造函数
    C(C  &&c);                   //移动构造函数( C++11引入)
    C &operator = (C const &c);  //拷贝赋值函数
    C &operator = (C &&c);       //移动赋值函数(C++11引入)
    
    ~C();                        //解构函数
}
```

```C++
C c1 = c2;              //拷贝构造函数
C c1 = std::move(c2);   //移动构造函数

c1 = c2; 	            //拷贝构造函数
c1 = std::move(c2);     //移动赋值函数 

C c1 = C();             //移动构造函数
c1 = C();               //移动赋值函数
return c2;              //移动赋值函数
```





## 5.2 C++11:为什么区分拷贝和移动?

* 有时候,我们需要把一个对象v2移动到v1上,而不需要涉及实际数据的拷贝.
* 时间复杂度:移动是O(1), 拷贝是O(n)
* 我们可以用`std:: move`实现移动
* v2被移动到v1后,原来的v2会被清空,因此仅当v2**再也用不到时**才用移动.

```C++
void test_copy(){
    std::vectro<int> v1(10);
    std::vectro<int> v2(200);
	
    v1 = v2;				//拷贝赋值 O(n)
    
    std::cout << " after copy: " << std::endl;
    std::cout  <<"v1 lenght" << v1.size() << std::endl; // 200
    std::cout << "v2 lenght" << v2.size() << std::endl; //200    
}

void test_move(){
    std::vector<int> v1(10);
    std::vector<int> v2(200);
    
    v1 = std::move(v2); 			//移动赋值 O(1);
    
    std::cout << "after move:" << std::endl;
    std::cout << "v1 lenght" << v1.size() << std::endl;   //200
    std::cout << "v2 lenght" << v2.size() << std::endl; // 0
}


```





## 5.3移动进阶: 交换两者的值

* 除了`std::move`可以把v2移动到v1外,
* 还可以通过`std::swap`交换v1和v2
* swap在高性能计算中可以用来实现双缓存(ping-pong buffer)





## 5.4哪些情况会触发"移动" 

* 这些情况编译器会调用**移动**:

  ```C++
  return v2                  // v2做返回值
  
  v1 = std::vector<int>(200)  //就地构造的v2
  v1 = std::move(v2)             //显示地移动
  
  ```

  

* 这些情况下编译器会调用**拷贝**

  ```C++
  return std::as_const(v2)    //显示地拷贝
  v1 = v2                    //默认拷贝
  ```

* 注意, 以下语句**没有任何作用**:

  ``` C++
  std::move(v2)			//不会清空v2,需要清空可以用v2 = {} 或 v2.clear()
  std::as_constd(v2)      //不会拷贝v2,需要拷贝可以用{auto_ = v2;}
  ```

  

```c++
std::move(t) 相当于 (T &&) t
std::as_const(t)  相当于(T const &)t
```



# 6.智能指针

## 6.1RAII解决内存管理的问题: unique_ptr

```C++
#include<cstdio>
#include<memory>

struct C{
    C(){
        printf("分配内存!\n");
    }
    
    ~C(){
        printf("释放内存!\n");
    }
};

int main(){
    std::unique_ptr<C> p = std::make_unique<C>();  //
    
    if(1 + 1 == 2){
        printf("出了点小状况....\n");
        return 1;         //自动释放 p
    }
    
    return 0;  //自动释放 p
}
```

## 6.2 防止悬空指针

* 释放一个指针后,必须把这个指针设成NULL, 防止空悬指针

delete p;

p  = nullptr;

* 然后unique_ptr则把他们封装成一个操作: 只需要

p = nullptr;   //等价于: p.reset()

```C++
#include<cstdio>
#include<memory>

struct C{
    C(){
        printf("分配内存!\n");
    }
    
    ~C(){
        printf("释放内存!\n");
    }
};

int main(){
    std::unique_ptr<C> p = std::make_unique<C>();  //
    
	printf("提前释放.....\n");
    p = nullptr;
    printf(".....释放成功\n");
    
    return 0;  // p  不会再释放一遍
     
}
```

* 其次 unique_ptr 是删除了拷贝构造函数的
* unique是不能被拷贝的
