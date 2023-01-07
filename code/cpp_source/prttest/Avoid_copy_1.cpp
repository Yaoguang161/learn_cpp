#include<iostream>
#include<vector>

struct Pig{
    std::string m_name;
    int m_weight;


    Pig(std::string name, int weight): m_name(name),m_weight(weight)
    {}

    //将拷贝构造函数声明为explicit的,这样隐式的拷贝就会出错
    // 当的确需要拷贝的时候,也可以改成Pig(pig)的语法来强制拷贝.
    explicit Pig (Pig const &other) = default;


};
void show(Pig pig){         
    std::cout << "name: " << pig.m_name << std::endl;
    std::cout << "weight: " << pig.m_weight << std::endl;
}




int main(){
    Pig pig{"佩奇", 80};
    show(pig);  //  编译错误 
    show(Pig{pig});  //编译通过

    return 0;
}