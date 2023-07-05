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

# 2.快速排序

```c++
#include <iostream>

using namespace std;

const int N = 100010;

int q[N];

void quick_sort(int q[], int l, int r)
{
    if (l >= r) return;

    int i = l - 1, j = r + 1, x = q[l + r >> 1];
    while (i < j)
    {
        do i ++ ; while (q[i] < x);
        do j -- ; while (q[j] > x);
        if (i < j) swap(q[i], q[j]);
    }

    quick_sort(q, l, j);
    quick_sort(q, j + 1, r);
}

int main()
{
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i ++ ) scanf("%d", &q[i]);

    quick_sort(q, 0, n - 1);

    for (int i = 0; i < n; i ++ ) printf("%d ", q[i]);

    return 0;
}

```

