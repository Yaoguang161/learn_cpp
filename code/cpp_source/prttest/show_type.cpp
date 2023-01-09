#include<iostream>
#include<vector>
int main(){
    std::cout << std::is_same <int const *,  int* const >::value << std::endl;
    system("pause");
    return 0;
}

// 效果相同返回1
//std::is_same<T1,T2>::value
