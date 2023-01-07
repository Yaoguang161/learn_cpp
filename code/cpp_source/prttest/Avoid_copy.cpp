#include<iostream>
#include<vector>

struct Pig{
    std::string m_name;
    int m_weight;


    Pig(std::string name, int weight): m_name(name),m_weight(weight)
    {}

    Pig(Pig const &other):m_name(other.m_name),m_weight(other.m_weight)
    {
        std::cout << "拷贝了一只猪 !" << std::endl;
    }

};


//一下show执行
//拷贝了一只猪!
//name: 佩奇
//weight: 80
void show(Pig pig){         
    std::cout << "name: " << pig.m_name << std::endl;
    std::cout << "weight: " << pig.m_weight << std::endl;
}

//用常引用避免不必要的拷贝: 常引用
//执行效果:(没有执行拷贝)
//name: 佩奇
//weight: 80
void show(Pig const &pig){
    std::cout << "name:" << pig.m_name << std::endl;
    std::cout << "weight" << pig.m_weight << endl;

}


int main(){
    Pig pig{"佩奇", 80};
    show(pig);

    return 0;
}