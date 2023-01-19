
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







---





















































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





* C++函数前和函数后加const修饰符区别

函数名后面加了const修饰符，这样说明函数的成员对象是不允许修改的，在类的成员函数里面，默认是在成员函数的第一个位置是this指针，如果在成员函数（只能是成员函数，要是类的静态函数或者是非成员函数就不可以在函数后面加上const）后面加const，则说明this指针的值是不可以修改的，只能读取。而如果函数里面试图改变成员变量的值，这样编辑器肯定是不允许的，所以会出现错误的。

解决办法：在类的成员变量前面加上mutable修饰符。

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



## 13.1tuple:结构化绑定

```C++
#include<iostream>
#include<tuple>

int main(){
    auto tup = std::tuple(3,3.14f,'h');
    
    auto [first,second,third] = tup;
    //结构化绑定也支持绑定为引用:
    //auto &[x,y...] = tup;
    
    
    std::cout << first  <<std::endl;
    std::cout << second <<std::endl;
    std::cout << third  <<std::endl;
        
    return 0;
}
```



* 结构化绑定为万能推导但是不是`decltype(auto)`

  ```C++
  auto &&[x,y,...] = tup;  //正确
  decltype(auto)[x,y,..] = tup; //错误 !
  
  ```



* 结构化绑定: 还可以是任意自定义类

  ```C++
  #include<iostream>
  #include<tuple>
  
  struct MyClass{
      int x;
      float y;
  };
  
  int main(){
      MyClass mc = {42, 3.14f};
      
      //这里也可以是结构化绑定
      auto [x,y] = mc;
      
      std::cout << x << ","
      
      return 0;
  }
  
  ```

* tuple:用于函数多个返回值

  ```C++
  #include<iostream>
  #include<tuple>
  #include<cmath>
  
  std::tuple<bool,float> mysqlt(float x){
      if(x >= 0.f){
          return {true, std::sqrt(x)};
      }else{
          return {false,0.0f};
      }
  }
  int main(){
      auto [success,value] = mysqrt(3.f);
      if(success){
          printf("成功!结果为: %f\n",value);
      }else{
          printf("失败 !找不到平方根!\n");
      }
      return 0;
  }
  ```

  

  

    



# 14.常用容器: optional

* 上个例子用`std::tuple<bool,T>`其中第一个bool表示成功与否. 但是这样尽管失败了还是需要指定一个值`0.0f`,非常麻烦

* 这种情况推荐用`std::optional<T>`

* 成功时,直接返回`T`. 失败时,只需返回`std::nullopt`即可

  ```C++
  #include<iostream>
  #include<optional>
  #include<cmath>
  
  std::optional<float> mysqrt(float x){
      if (x >= 0.f){
          return std::sqrt(x);
      }else{
          return std::nullopt;   //注意哦: nullopt和null是完全不一样的
      }
  }
  int main(){
      auto ret = mysqrt(3.f);
      if(ret.has_value()){
          printf("成功! 结果为: %f\n",ret.value());
      }else{
          printf("失败! 找不到平方根! \n");
      }
      return 0;
  }
  ```

  

* `optional: value()`会检测是否为空, 空则抛出异常

* 当`ret`没有值时(即`nullopt`), `ret.value()`会抛出一个异常,类型为`std::bad_optional_access`

  

# 15.常用容器:variant

* `variant`:更安全的`union`,存储多个不同类型的值
* 有时候需要一个类型"要么存储int,要么存储float",这时候可以用`std::variant<int,float>`
* 和`union`相比,variant符合RAII思想,更加安全易用
* 给`variant`赋值只需要用普通的 = 即可. 
* 容器的选择:
  * `variant`的特点是只存储其中一种类型
  * `tuple`的特点是每个类型都有存储

```C++
#include<iostream>
#include<variant>
int main(){
    std::variant<int,float> v = 3;  //选择其中的一个容器
    
    std::cout << std::get<int>(v) << std::endl;  //3
    std::cout << std::get<0>(v) << std::endl;  //3
    
    v = 3.14f;
    
    std::cout << std::get<float>(v) << std::endl;  // 3.14f
    std::cout << std::get<int>(v) << std::endl;  //  运行时错误
    
    
    return 0;
}
```

* 要获取某个类型的值,比如要获取`int`用`std::get<int>`,如果当前`variant`里不是这个类型,就会抛出异常`std::bad_variant_access`



* 那么如果判断当前是哪个类型呢?
  * 可以用`std::holds_alternative<int>`判断当前里存储的是不是`int`.

​	

``` C++
#include<iostream>
#include<variant>
void printf(std::variant<int,float> const &v){
    if(std::holds_alternative<int>(v)){
        std::cout << std::get<int>(v) << std::endl;
    }else if(std::holds_alternative<float>(v)){	
        std::cout << std::get<float>(v) << std::endl;
	}
}

int main(){
    
    std::variant<int,float> v = 3;
    printf(v);
    v = 3.14f;
    printf(v);
    
    return 0;
}

```



* 判断当前是哪个类型用`v.index()`,判断当前是参数列表中的第几个类型.

  ```C++
  #include<iostream>
  #include<variant>
  void printf(std::variant<int,float> const &v){
      if(v.index() == 0){
          std::cout << std::get<int>(v) << std::endl;
      }else if(v.index() == 1){	
          std::cout << std::get<float>(v) << std::endl;
  	}
  }
  
  int main(){
      
      std::variant<int,float> v = 3;
      printf(v);
      v = 3.14f;
      printf(v);
      
      return 0;
  }
  #include
  ```

  

* 批量匹配 `std::visit`:

* 可以有多个`variant`作为参数

* 相应的lambda的参数数量要与之匹配

* `std::visit`会自动罗列出所有的排列组合

* 但是会编译变慢

  

```C++
auto add(std::variant<int,float> const &v1,
        std::variant<int,float> const &v2,){
    std::variant<int,float> ret;
    std::visit([&](auto const &t1, auto const &t2){   //先判断是不是int + int,判断是不是float +float  ,最后判断是不是int + float , 判断是不是float + int
       ret = t1 + t2; 
    },v1,v2);
    return ret ;
}


int main(){
    std::variant<int,float> v = 3;
    print(add(v,3.14f));
    return 0;
}


//6.14
```



* `std::visit`可以有返回值

* `std::visit`里面lambda可以有返回值, 不过都得同样类型

* 利用这一点进一步优化

  ```C++
  auto add(std::variant<int,float> const &v1,
          std::variant<int,float> const &v2){
      return std::visit([&](auto const &t1, auto const &t2)
       			-> std::variant<int,float>{
            return t1 + t2;
      },v1,v2)
  }
  ```

  





# 16. C++引入的时间标准库

```C++
auto t0 = chrono::steady_clock::now();          //获取当前的时间段
auto t1 = t0 + chrono::seconds(30);           //当前时间点的30秒后
auto dt = t1 - t0;                           // 获取两个时间点的差(时间段);
int64_t sec = chrono::druation_cast<chrono::seconds>(dt).count();   // 时间差的秒数

```



* 跨平台的sleep: `std::this_thread::sleep_for`

* 可以用`std::this_thread::sleep_for`代替`Unix`类操作系统专用的`usleep`,他可以让当前线程休眠一段时间,然后继续

* 而且单位也可以自己指定,比如这里是`milliseconds`表示毫秒,也可以换成`microseconds`表示微秒,`seconds`表示秒,`chrono`的强类型让单位更加自由

* ```C++
  #include<iostream>
  #include<thread>
  #include<chrono>
  
  int main(){
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
      
      return 0;
  }
  
  ```



* 除了接受一个时间段的`sleep_for`,还有接受一个时间点的`sleep_until`,表示让当前的线程休眠到某个时间点

* 等价于上面

  ```C++
  #include<iostream>
  #include<thread>
  #include<chrono>
  
  int main(){
      auto t = std::chrono:steady_clock::now() + std::chrono::millisenconds(400);
      std::this_thread::sleep_until(t);
      return 0;
  }
  ```

  

# 17. 进程与线程

* **进程**是一应用程序被操作系统拉起来加载到内存之后从开始执行到执行结束的这样一个过程. 简单来说, 进程是程序(应用程序, 可执行文件)的一次执行. 比如双击打开一个桌面应用软件就是开启了一个进程.
* **线程**是进程中的一个实体,是被系统独立分配和调度的基本单位. 也有说,线程才是CPU可以执行的最小单位. 也就是说,进程本身并不能获取CPU时间,只有它的线程才可以.
* 从属关系: 进程 > 线程. 一个进程可以拥有多个线程
* 每个线程共享同样的内存空间, 开销比较小.
* 每个进程拥有独立的内存空间, 因此开销更大.
* 对于高性能计算,更好的是多线程.
* 进程地址独立, 沟通起来困难





# 18. 现代C++中的多线程

* C++11开始, 为多线程提供了语言级别的支持. 他开始用`std::thread`这个类来表示线程
* `std::thread`构造函数的参数可以是任意lambda表达式
* 当那个线程启动时,会执行这个lambda里的内容
* 这样就可以一边和用户交互,一边在另一个线程里慢吞吞的下载文件了.

```C++
#include<iostream>
#include<thread>
#include<string>

void download(std::string file){
    for(int i = 0;i < 10; i ++){
        std::cout << "Downloading" << file
            	  << "(" << i * 10 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete: " << file << std::endl;
}
```

* 然后就会出现一个错误

  * 找不到符号`pthread_create`

  * 原因: `std::thread`的实现背后是基于`pthread`的

  * 解决: `CMakeLists.txt`里连接`Threads::Threads`即可:

    * ```cmake
      cmake_minimum_required(VERSION 3.10)
          
      set(CMAKE_CXX_STANDARD 17)
          
      project(cppptest LANGUAGES CXX)
      
      add_executable(cpptest main.cpp)
          
      find_package(Threads REQUIRED)
          
      target_link_libraries(cpptest PUBLIC Threads::Threads)
      ```

      





# 19. 主线程等待子线程结束: t1.join()

* 因此, 我们想要让主线程不要着急退出, 等子线程结束了再退出
* 可以用`std::thread`类的成员函数`join()`来等待该进程结束

```C++
#include<iostream>
#include<thread>
#include<string>

void download(std::string file){
    for(int i = 0; i < 10; i++){
        std::cout << "Downloading" << file
            << "(" << i *  10 << "%)...." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete: " << file << std::endl;
}

void interact(){
    std::string name;
    std::cin >> name;
    std::cout << "Hi, " << name << std::endl;
}


int main(){
    std::thread t1([&]{
        download("hello.zip");
    });
    interact();
    std::cout << "Waiting for child thread..." << std::endl;
    t1.join();
    std::cout << "Child thread exited! " << std::endl;
    return 0;
}


```



# 20.分离线程detach()

* 问题: `std::thread`的析构函数会销毁线程
* 解决: 调用成员函数`detach()`分离该线程--意味着线程的生命周期不再由当前`std::thread`对象管理,而是在线程退出以后自动销毁自己
* 不过这样还是会在进程退出时候自动退出

```C++
#include<iostream>
#include<thread>
#include<string>

void download(std::string file){
    for(int i = 0; i < 10; i++){
        std::cout << "Downloading" << file
            << "(" << i *  10 << "%)...." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cout << "Download complete: " << file << std::endl;
}

void interact(){
    std::string name;
    std::cin >> name;
    std::cout << "Hi, " << name << std::endl;
}

void myfunc(){
    std::thread t1([&]{
        download("hello.zip");
    });
    t1.detach();
    //t1所代表的线程被分离了, 不在随t1对象销毁
}

int main(){
	myfunc();
    interact();
    return 0;
}


```







# 21.析构函数不再销毁线程: 移动到全局线程池

* 但是`detach`的问题是进程退出时候不会等待所有子进程执行完毕. 所有另一种解法是把t1对象移动到一个全局变量去,从而延长其生命周期到myfunc函数体外

* 这样就可以等下载完再退出了

  ```C++
  std::vector<std::thread> pool;
  
  void myfunc(){
      std::thread t1([&]{
          download("hello.zip");
      });
      //移交控制权到全局的 pool列表,以延长t1的生命周期
      pool.push_back(std::move(t1));
  }
  
  int main(){
      myfunc();
      interact();
      for(auto &t: pool) t.join();  // 等待池里的线程全部执行完毕
      return 0;
  }
  
  
  
  ```

  



# 22.main函数退出后自动join全部线程

* 但是需要在main里面手动`join`全部线程还是有点麻烦,我们可以自定义一个类ThreadPool,并用他创建一个全局变量,其结构函数会在`main`退出后自动调用

* ```C++
  class ThreadPool{
      std::vector<std::thread> m_pool;
      
  public:
      void push_back(std::thread thr){
          m_pool.push_back(std::move(thr));
      }
      
      ~ThreadPool(){                          //main函数退出后会自动调用
          for(auto &t: m_pool) t.join();      //等待池里的线程全部执行完毕
      }    
  };
  
  ThreadPool tpool;
  
  void myfunc(){
      std::thread t1([&]{
          download("hello.zip");
      });
      //移交控制权到全局的pool列表, 以延长t1的生命周期
      tpool.push_back(std::move(t1));
  }
  int main(){
      myfunc();
      interact();
      return 0;
  }
  
  
  
  ```

  



# 23.异步: std::async

* `std::async`接受一个带返回值的`lambda`,自身返回一个`std::futrue`对象

* `lambda`的函数体将在另一个线程里执行

* 接下来你可以在main里面做一些特别的事情,`download`会持续在后台悄悄运行

* 最后调用`future`的get方法,如果此时`download`还没有完成,会等待`download`完成,并获取`download`的返回值

  ```C++
  #inlcude<iostream>
  #include<string>
  #include<thread>
  #include<future>
  
  int download(std::string file){
      for(inti = 0; i < 10; i ++){
          std::cout << "Downloading" << file
              << "( " <<i * 10 << "%)..." << std::endl;\
          std::this_thread::sleep_for(std::chrono::milliseconds(400));
      }
      std::cout << "Download complete: " << file << std::endl;
      return 404;
  }
  
  void interact(){
      std::string name;
      std::cin >> name;
      std::cout << "HI, " << name << std::endl;
  }
  
  int main(){
      std::future<int> fret = std::async([&]{
          return download("hello.zip");
      });
      interact();
      int ret = fret.get();
      std::cout << "Download result: " << ret << std::endl;
      return 0;
  }
  
  ```

  

## 23.1等待一段时间: wait_for()

* 只要线程没有执行完,`wait()`会无限等下去
* 而`wait_for()`则可以指定一个最长等待时间, 用`chrono`里的类表示单位.他会返回一个`std::future_status`表示等待是否成功
* 如果超过这个时间线程还没有执行完毕,则放弃等待,返回`future_status::timeout`
* 如果线程在指定的时间内执行完毕,则认为等待完成,返回`future_status::ready`
* 同理还有`wait_until`其参数是一个时间点.

```C++
int main(){
    std::future<int> fret = std::async([&]{
        return download("hello.zip");
    });
    while(true){
        std::cout << "Waiting for download complete..." << std::endl;
        auto stat  = fret.wait_for(std::chrono::milliseconds(1000));
        if(stat == std::future_status::ready){
            std::cout << "future is ready!!! " << sdt::endl;
            break;
        }else{
            std::cout << "Future not ready!!" << std::endl;
        }
    }
    int ret = fret.get();   //获取下载的结果
    std::cout << "Download result: " << ret << std::endl;
    return 0;
}
```

# 24. 避免互斥

# 24.1 `std::mutex`上锁

* 调用`std::mutex`的`lock()`时,会检测`mutex`是否已经**上锁**
* 如果没有**锁定**,则对**mutex**进行上锁.
* 如果已经**锁定**,则陷入等待,知道`mutex`被另一个**线程**解锁后,才再次**上锁**
* 而调用`unlock()`则会进行**解锁**
* 这样,就可以保证`mtx.lock()`和`mtx.unlock()`之间的代码段,同一时间只有一个线程在执行,从而避免数据竞争

```C++
#include<iostream>
#include<string>
#include<thread>
#include<vector>
#include<mutex>

int main(){
    std::vector<int> arr;
    std::mutex mtx;
    
    std::thread t1([&]{
        for(int i = 0;  i< 1000; i ++){
            mtx.lock();
            arr.push_back(1);
            mtx.unlock();
        }
    });
 	std::thread t2([&]{
        for(int i = 0; i < 1000; i++){
            mtx.lock();
            arr.push_back(2);
            mtx.unlock();
        }            
    });
    t1.join();
    t2.join();
    return 0;
}
```





## 24.2 `std::lock_guard`: 符合RAII思想的上锁和解锁

* 根据RAII思想,可将锁的持有视为资源,上锁视为锁的获取,解锁视为锁的释放

* `std::lock_guard`就是这样一个工具类,他的构造函数里会调用`mtx.lock()`,结构函数会调用`mtx.unlock()`.从而退出函数作用域时能自动解锁,避免程序员粗心不小心忘记解锁

  ```C++
  #include<iostream>
  #include<stirng>
  #include<thread>
  #include<vector>
  #include<mutex>
  
  int main(){
      std::vector<int> arr;
      std::mutex mtx;
      std::thread t1([&]{
          for(int i = 0; i < 1000; i ++){
              std::lock_guard grd(mtx);
              arr.push_back(1);
          }
      });
      
      std::thread t1([&]{
          for(int i = 0; i < 1000; i ++){
              std::lock_guard grd(mtx);
              arr.push_back(2);
          }
      });
      t1.join();
      t2.join();
     
      return 0;
  }
  ```

  

## 24.3 `std::unique_lock`:也符合RAII思想,但自由度更高

* `std::lock_guard`严格在解构时 `unlock()`, 但是有时候我们会希望提前`unlock()`.这时可以用`std::unique_lock`,他额外存储了一个`flag`表示是否已经被释放. 他会在解构检测这个`flag`,如果没有释放,则调用`unlock()`,否则不调用

* 然后可以直接调用`unique_lock`的`unlock()`函数来提前解锁,但是即使忘记解锁也没关系,退出作用域时候`他还会自动检查一下要不要解锁

  ```C++
  #include<iostream>
  #include<string>
  #include<thread>
  #include<vector>
  #include<mutex>
  
  int main(){
      std::vector<int> arr;
      std::mutex mtx;
      std::thread t1([&]{
          for(int i = 0; i < 1000; i ++){
              std::unique_lock grd(mtx);
              arr.push_back(1);
          }
      });
      std::thread t2([&]{
          for(int i = 0; i < 1000; i++){
              std::unique_lock grd(mtx);
              arr.push_back(2);
              grd.unlock();
              printf("outside of lock\n");
              //grd.lock();     //如果需要, 还可以从新上锁
          }
      });
      t1.join();
      t2.join();
      return 0;
      
  }
  ```

  

* `sdt::unique_lock`: 用`std::defer_lock`作为参数

  * `std::unique_lock`的构造函数还可以有一个额外参数,那就是`std::defer_lock`

  * 指定了这个参数的话,`std::unique_lock`不会在构造函数中调用`mtx.lock()`,需要之后再手动调用`grd.lock()`才能上锁

  * 好处依然是忘记了`grd.unlock()`也能够自动调用`mtx.unlock()`

  * ```C++
    #include<iostream>
    #include<string>
    #include<thread>
    #include<vector>
    #include<mutex>
    
    int main(){
        std::vector<int> arr;
        std::mutex mtx;
        sdt::thread t1([&]{
            for(int i = 0; i <  1000; i ++){
                std::unique_lock grd(mtx);
                arr.push_back(1);
            }
        });
        
        std::thread t2([&]{
            for(int i = 0; i< 1000; i++){
                std::unique_lock grd(mtx,std::defer_lock);  //
                printf("before the lock\n"); 
                grd.lock();                                 //
                arr.push_back(2);
                grd.unlock();
                printf("outside of lock\n");
            }
        });
        
        t1.join();
        t2.join();
        
        return 0;
    }
    ```

  * 多个对象就每个对象一把锁





##  24.4 如果上锁失败, 不要等待: try_lock()

* `lock()`如果发现`mutex`已经上锁的话, 会等待他直到他解锁
* 也可以用无阻塞的`try_lock()`,他在上锁失败后不会陷入等待,而是直接返回`false`;如果上锁成功,则会返回`true`

```C++
#include<iostream>
#include<mutex>

std::mutex mtx1;

int main(){
    if(mtx1.try_lock()){
        printf("succeed! \n");
    }else{
        printf("failed\n");
    }
    
    if(mtx1.try_lock()){
        printf("succeed\n");
    }else{
        printf("failed\n");
    }
    
    mtx1.unlock();
    return 0;
}
```





* 只等待一段时间: `try_lock_for()`,参数可以设定时间
* 同理还有接受时间点的`try_lock_until()`

## 24.5同时锁住多个`mutex`:死锁难题

由于同时执行的两个线程,他们发生的指令不一定是同步的,因此有可能出现这种情况:

* t1执行`mtx1.lock()`

* t2执行`mtx2.lock()`

* t1执行`mtx2.lock()`:失败,陷入等待

* t2执行`mtx1.lock()`: 失败,陷入等待

* 双方都在等着对方释放锁,但是因为等待而无法释放锁,从而要无线等待下去

* 这种现象称为死锁(dead-lock)

* ```C++
  #include<iostream>
  #include<string>
  #include<thread>
  #inlclude<mutex>
  
  int main(){
      std::mutex mtx1;
      std::mutex mtx2;
      
      std::thread t1([&]{
          for(int i = 0;i < 1000; i++){
              mtx1.lock();
              mtx2.lock();
              mtx2.unlock();
              mtx1.unlock();
          }
      });
      
      std::thread t2([&]{
          for(int i =0; i < 1000; i++){
              mtx2.lock();
              mtx1.lock();
              mtx1.unlock();
              mtx2.unlock();
          }
      });
      
      t1.join();
      t1.join();
         
      
      return 0;
  }
  ```

* 永远不要同时持有两个锁,分别上锁,这样也可以避免死锁

* 用`std::lock()`同时对多个上锁`std::lock(mtx1,mtx2)`

* RAII版本的`std::scoped_lock()`



* 同一个线程重复调用`lock()`也会造成死锁

## 24.6 `std::shared_lock()`:符合RAII思想的`lock_shared()`

* 正如`std::unique_lock()`,也可以用`std::shared_lock()`针对`lock_shared()`,这样就可以在函数体退出时自动调用`unlock_shared()`,更加安全了

* ```C++
  #include<iostream>
  #include<thread>
  #include<vector>
  #include<mutex>
  #include<shared_mutex>
  
  class MTVcector{
      std::vector<int> m_arr;
      mutable std::shared_mutex m_mtx;
  public:
      void push_back(int val){
          std::unique_lock grd(m_mtx);
          m_arr.push_back(val);
       }
      
      size_t size() const{
          std::shared_lock grd(m_mtx);
          return m_arr.size();
      }
  };
  
  int main(){
      MTVector arr;
      
      std::thread t1([&] (){
          for(int i = 0; i < 1000; i ++){
              arr.push_back(i);
          }
      });
      
      std::thread t2([&] (){
          for(int i = 0; i < 1000;  i++){
              arr.push_back( 1000 + i);
          }
      });
      
      t1.join();
      t2.join();
      
      std::cout << arr.size() << std::endl;
      
      return 0;
      
  }
  
  ```





# 25.条件变量

## 25.1 条件变量: 等待被唤醒

* `cv.wait(lck)`将会让当前线程陷入等待

* 在其他线程中调用`cv.notify_one()`则会唤醒那个陷入等待的线程

* 可以发现`std::condition_varibale`必须和`std::unique_lock<std::mutex>`一起用

* ```C++
  #include<iostream>
  #include<thread>
  #include<mutex>
  #include<condition_variable>
  
  int main(){	
      std::condition_variable cv;
      std::mutex mtx;
      
      std::thread t1([&]{
          std::unique_lock lck(mtx);
          cv.wait(lck);              //这里在等待
          
          std::cout << "t1 is awake" << std::endl;
      });
      
      std::this_thread::sleep_for(std::chrono::milliseconds(400))
      
      std::cout << "notifying...." << std::endl;
      cv.notify_one();           //will awake t1
      
      t1.join();
      return 0;
  }
  ```

  

  

## 25.2原子级别的操作

* `fetch_add`对应于 +=

* `store`对应于 =

* `load`用于读取其中的`int`值

  ```C++
  #include<iostream>
  #inlcude<thread>
  #include<atomic>
  int main(){
      std::atomic<int> counter;
      counter.store(0);
      
      
      std::vector<int> data(20000);
      
      std::thread t1([&]{
         for(int i = 0;i < 10000; i++){
             int index = counter.fetch_add(1);
             data[index] = i;
         } 
      });
      
      std::thread t2([&]{
          for(int i = 0; i < 10000; i ++){
              int index = counter.fetch_add(1);
              data[index] =  i + 10000;
          }
      });
      
      t1.join();
      t2.join();
      
      std::cout << data[10000] << std::endl;
        
      return 0;
  }
  
  #include<iostream>
  #inlcude<thread>
  #include<atomic>
  int main(){
      std::atomic<int> counter;
      counter.store(0);
      
      
      std::thread t1([&]{
         for(int i = 0;i < 10000; i++){
  			counter.fetch_add(1);
         } 
      });
      
      std::thread t2([&]{
          for(int i = 0; i < 10000; i ++){
  			counter.fetch_add(1);
          }
      });
      
      t1.join();
      t2.join();
      
      std::cout << counter.load() << std::endl;
        
      return 0;
  }
  
  // 20000
  //原子级别的操作能在多线程中准确得到结果
  
  ```

  

* `fetch_add`会返回旧值

* `exchange`:读取的同时写入

* `compare_exchange_strong`:读取,比较是否相等, 相等则写入

  

# 26. `forword`与完美转发

*  所谓完美转发是指在函数模板中,完全依照模板的参数的类型(即保持参数的左值,右值),将参数传递给函数模板中调用的另一个函数.  
* C++11中提供了这样一个函数`std::forward`, 不管参数是T&&这种未定的引用还是明确的左值引用或者右值引用,它会按照参数本来的类型转发.

* 右值引用, 完美转发再结合可变模板参数, 我们可以写一个万能的函数包装器, 带返回值的, 不带返回值的, 带参数的, 和不带参数的函数都可以委托这个万能的函数包装器执行.

* ```C++
  // 万能的函数包装器
  template <class Function, class ...Args>
  inline auto FuncWarpper(Function && f, Args && ... args) -> decltype(f(std::forward<Args>(args)...))
  {
      return f(std::foward<Args>(args)...);
  }    
  
  
  
  //测试代码如下
  void test(){
      cout << "void" << endl;
  }
  int test1(){
      return 1;
  }
  int test2(int x){
      return x;
  }
  string test3(string s1, string s2){
      return s1 + s2;
  }
  test(){
      FunWrapper(test0);          //没有返回值
      FunWrapper(test1);          //返回1
      FunWrapper(test2,1);		//返回1
      FunWrapper(test3,"aa","bb");//返回"aabb"
  }
  
  ```

  
