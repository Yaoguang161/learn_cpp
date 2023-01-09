//这里是关于引用的用法
#include<cstdio>
#include<iostream>


int main(){

    int x = 233;
    int &ref = x;
    ref = 42;
    printf("%d\n",x);    //42
    x = 1024;
    printf("%d\n", ref);  // 1024



    //等价于以下代码:
    int x = 233;
    int *ref = &x;
    *ref = 42;
    printf("%d\n",x);    // 42
    x = 1024;
    printf("%d\n",*ref);  // 1024





    system("pause");
    return 0;
}