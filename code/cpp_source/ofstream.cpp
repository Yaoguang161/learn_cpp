#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
void bubble_sort(vector<int> &vec, ofstream *ofil = 0){    //C++允许我们将全部或部分参数设定默认值.
    //code
}
int main(){
    int ia[8] = {8,32,3,13,1,21,5,2};
    vector<int> vec(ia,ia+8);

    //以下就像调用bubble_sort(vec,0); 一样
    //不会产生任何调试信息
    bubble_sort(vec);
    display(vec);


    //ok: 这样便会产生调试信息
    ofstream ofil("data.txt");
    bubble_sort(vec, &ofil);
    display(vec, ofil);

    return 0;

}