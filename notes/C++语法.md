
# 1.C++如何实现线程安全

实现线程安全通常有一下集中方法:

1. 使用互斥锁(mutex) : 互斥锁是一种用于保护临界区的同步对象, 它可以防止多个线程同时访问临界区. C++中的互斥锁可以使用 `std::mutex`或者`std::lock_guard`来实现

2. 使用信号量(semaphore): 信号量是一种用于保护临界区的同步对象,它能限制线程对临界区的访问次数.C++中的信号量可以使用`std::semaphore`来实现.

3. 使用读写锁(reader-writer lock) : 读写锁是一种用于保护临界区的同步对象, 它允许多个线程同时读写临界区, 但只允许一个线程写入临界区.C++中的读写锁可以使用`std::shared_mutex`或`std::shared_lock`来实现

4. 使用原子操作(atomic operation): 原子操作是一种在单个操作内完成的操作, 它不会被其他线程打断. C++的原子操作可以使用`std::atomic`来实现.

   

# 2.一个指针指向"元素类型为int"的vector

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





# 5.自定义构造函数

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

## 5.1三五零法则

1. 如果一个类定义了**析构函数**, 那么必须同时定义或删除**拷贝构造函数**和**拷贝赋值函数,**否则出错.
2. 如果一个类定义了**拷贝构造函数**,那么您必须同时定义或删除**拷贝赋值函数**,否则出错,删除可导致低效.
3. 如果一个类定义了**移动构造函数**, 那么您必须同时定义或删除**移动赋值函数**,否则出错,删除可导致低效.
4. 如果一个类定义了**拷贝构造函数**或**拷贝赋值函数**,那么您最好同时定义**移动构造函数**或**移动赋值函数**,否则低效.

## 5.2什么时候需要担心三五法则

* 一般来说,可以可以认为符合三五法则的类型是安全的
* 以下类型是安全的:
* int id;                                                //基础类型
* std::vector<int> arr; 			        //STL容器
* std::shared_ptr<Object> child;  //智能指针
* Object *parent;                              //原始指针, 如果是从智能指针里的get() 出来的



* 一下对象是不安全的:

* char *ptr;                                      //原始指针, 如果是通过malloc/free或new/delete分配的

* CLint tex;                                      //是基础类型int , 但是对应这某种资源

* std::vector<Object *> objs;       //STL容器, 但存了不安全的对象

  

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





## 5.3C++11:为什么区分拷贝和移动?

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





## 5.4移动进阶: 交换两者的值

* 除了`std::move`可以把v2移动到v1外,
* 还可以通过`std::swap`交换v1和v2
* swap在高性能计算中可以用来实现双缓存(ping-pong buffer)





## 5.5哪些情况会触发"移动" 

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





## 5.6函数参数类型优化规则: 按引用还是按值?

* 用常引用来避免深拷贝
* 如果是基础类型(比如int , float) 则按值传递:
* float squareRoot(float val);
* 如果是原始指针(比如int *, Object *) 则按值传递:
* void doSOmethingWith(Object *ptr);
* 如果是数据容器类型(比如vector, string) 则按常引用传递:
* int sumArray(std::vector<int> const &arr);
* 如果数据容器不大(比如tuple<int,int>) ,则可以按值传递:
* glm::vec3 calculateGravityAt<glm::vec3 pos);
* 如果是智能指针(比如shared_ptr), 且需要**生命周期控制权**, 则按值传递:
* void addObject(std::shated_ptr<Object> obj);
* 如果是智能指针但是**不需要生命周期**,则通过.get获取原始指针后,按值传递:
* void  modifyObject(Object *obj);





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

## 6.3动态内存管理

* 分配通过new,释放通过delete,堆内存由程序员自行管理

* C++没有提供任何语法让我们得以从heap分配数组的同时为其元素设定初值

* 如果要删除数组中的所有对象,必须是在数组指针和delete表达式之间,加上一个空的下标运算符  `delete [] pia;`

  

## 6.4更智能的指针:shared_ptr

使用起来困难的原因, 在于unique_ptr解决重复释放的方式是禁止拷贝, 这样虽然有更高的效率,但导致使用困难, 容易犯错等

相比之下, 牺牲效率换来自由度的 `shared_ptr`则允许拷贝, 他解决重复释放的方式是通过引用计数:

1. 当一个`shared_ptr`初始化时, 将计数器设为1.
2. 当一个`shared_ptr`被拷贝时, 计数器加1
3. 当一个`shared_ptr`被解构时, 计数器减1, 减到0时, 则自动销毁他指向的对象

* 从而可以保证, 只要还有存在哪怕一个指针指向该对象, 就不会被解构

* 可以使用`use_count()`获取引用计数

* C++中所有对象的拷贝都是深拷贝, 只有`shared pointerer`和`weak pointer`是浅拷贝,`unique_pointer`是浅拷贝

  

  

## 6.5解决循环引用:解决方案一

   把其中逻辑上"不具有所有权" 的拿一个改成`weak_ptr`即可: 

* 因为父窗口"拥有"子窗口是天经地义的, 而子窗口并不"拥有"父窗口

* 其实主要是一个父窗口可以有多个子窗口, 只有规定子窗口从属于父窗口才能解决引用计数的问题

  ```c++
  #include<memory>
  struct C{
      std::shared_ptr<C>  m_child;
      std::weak_ptr<C> m_parent;
  };
  int main(){
      auto parent = std::make_shared<C> ();
      auto child = std::make_shared<C> ();
      
      //建立相互引用:
      parent->m_child = child;
      child ->m_parent = parent;
      
      parent = nullptr;  //parent 会被释放, 因为child 指向他的是 **弱引用**
    
      child =nullptr;  //child 会被释放. 因为指向child 的parent 已经释放了
      
      return 0;
      
  }
  
  
  ```

可以把C *理解为`unique_ptr`的弱引用. 

`weak_ptr`理解为`shared_ptr`的弱引用.

但`weak_ptr`能提供失效检测, 更安全.

```c++
int main(){
    std::shared_ptr<C> p  = std::make_shared<C> ();  //引用计数初始化为1
    
    printf("use count = %ld\n ",p.use_count());  //1
    
    std::weak_ptr<C> weak_p = p ;  //创建一个不影响计数器的弱引用
    
    printf("use count = %ld \n", p.use_count()) ;    // 1
    
    func(std::move(p));   //控制权转移, p 变成 null, 引用计数加不变
    
    if(weak_p.expired())
        printf("错误: 弱引用已经失效");
    else
        weak_p.lock() -> do_something();  // 正常执行, p的生命周期仍被 objlist延续着
    
    objlist.clear();    // 刚刚p移交给func的生命周期结束了! 引用计数减1, 变成0了
    
    if(weak_p.expired())       //因为shared_ptr 指向的对象已经释放,弱引用会失效
        printf("错误: 弱引用已经失效~ ");
    else
        weak_p.lock() -> do_something(); //不会执行
    
    return 0; //到这里最后一个弱引用weak_p 也会被释放, 他指向的"管理块" 被释放
            
}
```



## 6.5解决循环引用:解决方案二

父对象使用原始指针

```C++
#include<memory>
struct C{
    std::unique_ptr<C> m_child;
    C *m_parent;
};

int main(){
    auto parent = std::make_unique<C> ();
    auto child  = std::make_unique<C> ();
   	
    //建立互相引用
    parent -> m_child = std::move(child);  //移交child的所有权给parent
    child -> m_parent = parent.get();		
    
    parent = nullptr;   //parent 会被完全释放. 因为child指向的是他的原始指针
    //此时child也已经被释放了,因为child完全隶属于parent
    
}

```



# 7.一些关键字的使用

* `inline`仅仅是一种请求而没有强制性,最适合声明我inline的函数: 体积小,常被调用,所从事的计算并不复杂.`inline bool fibon_elem(int pos,int &elem)`

# 8.模板函数

```C++
void display_message(const string &, const vector<int>&);
void display_message(const string &, const vector<double>&);
void display_message(const string &, const vector<string>&);
```

使用`template`模板类替换,重载使代码更优雅

```C++
template <typename elemType>
void display_message(const string &msg, const vector<elemType> &vec){
    cout << msg;
    for(int ix = 0; ix <vec.size(); ++ix)
    {
        elemType t = vec[ix];
        cout << t << ' ';
    }
}
```

* 建议模板不要分离声明和定义,直接写在头文件里即可. 如果分离还要罗列出所有模板参数的排列组合,违背开闭原则

1. 类型作为参数 : `template<class T>`

2. 整数作为参数: `template <int N>`

3. 定义默认参数: `template <int N  = 0, class T = int >`

4. 使用模板函数: `myfunc<T,N>(...)`

5. 模板函数可以自动推断类型,从而参与重载

6. 模板具有惰性, 多次编译的特点

   ​                                                                                      

# 9.自动类型推导auto

一些局限性:

1. 因为需要等号右边的类型信息,所有没有 = 单独声明一个auto变量是不行的

   ```C++
   auto p;
   p = std::make_shared<MyClssWithVeryLongName>();
   ```

2. 而且类成员也不可以定义为`auto`

   ```C++
   struct MyClassWithVeryLongName{
       auto x = std::make_shared<int>();
   };
   ```

   

# 10 .C++特征:引用(int &)

* 引用的本质无非是指针,当我们试图修改一个引用时,实际上修改了原来的对象:

  ```C++
  #include<cstdio>
  
  int main(){
      int x = 233;
      int &ref = x;
      ref = 42;
      printf("%d\n",x);    //42
      x = 1024;
      printf("%d\n", ref);  // 1024
  }
  
  //修改ref后, x 也会变化, 修改x后, ref也会变化
  
  ```

  以上代码等价于:

  ```C++
  #include<cstdio>
  
  int main(){
  	int x = 233;
      int *ref = &x;
      *ref = 42;
      printf("%d\n",x);    // 42
      x = 1024;
      printf("%d\n",*ref);  // 1024
  }
  ```

  

# 11.C++特性: 常引用(int const &)

* 如果说`int &` 相当于`int *`,那么`int const &`就相当于`int const *`

* `const`修饰符的存在, 使得`ref`不能被写入(赋值)

* 这样的好处是更加安全(编译器也能够放心大胆地做自动优化)

  ```C++
  #include<cstdio>
  
  int main(){
      int x = 233;
      int const &ref = x;
      //ref = 42;    //会出错
      printf("%d\n",x);    //   233
      x = 1024;
      printf("%d\n", ref);  // 1024
  }
  ```

  

* 也可以有`auto` 来实现, `auto const &`  或`auto &`

* 函数也可以返回引用

#  12.const的用法

如果const位于星号*的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；

如果const位于星号的右侧，const就是修饰指针本身，即指针本身是常量。



```C++
const int* a = & [1]          //非常量数据的常量指针    指针常量
int const *a = & [2]          //非常量数据的常量指针     a is a pointer to the constant char variable
int* const a = & [3]          //常量数据的非常量指针指针常量 常量指针 a is a constant pointer to the (non-constant) char variable
const int* const a = & [4]    //常量数据的常量指针
```

因此，[1]和[2]的情况相同，都是指针所指向的**内容**为常量，这种情况下不允许对**内容**进行更改操作，如不能*a = 3 ；

[3]为指针本身是常量，而指针所指向的内容不是常量，这种情况下不能对**指针本身**进行更改操作，如a++是错误的；

[4]为**指针本身**和指向的**内容**均为常量。  





* 忽略类型就是`* const p`和`const *p`.  前者`p`不能改,即不能改指向, `p`不能指向另一个变量; 后者`*p`不能改,就是指向的内容`*p`不能改
* 举例子: `const int *p` 和`int const *p`是一样的,但是 `int* const p`和`int const *p`不一样
* `const int* p`和`int const *p`是一样的



# 13.常用容器:tuple

* `std::tuple<...>`可以将多个不同类型的值**打包**成一个.尖括号里填各个元素的类型.
* 之后可以用`std::get<0>`获取第0个元素,`std::get<1>`获取第一个元素,以此类推(从0开始数)

```C++
#include<iostream>
#include<tuple>

int main(){
    auto tup = std::tuple<int, float, char>(3, 3.14f, 'h');

    int first = std::get<0>(tup);
    float second = std::get<1>(tup);
    char third = std::get<2>(tup);


    std::cout << first << std::endl;
    std::cout << second << std::endl;
    std::cout << third << std::endl;


    system("pause");
    return 0;
}
```

