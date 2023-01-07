#include<iostream>

std::vector<std::shared_ptr<C>> objlist;

void func(std::shared_ptr<C> p){
    objlist.push_back(std::move(p));   //这里用移动可以更高效,但不必须
}

int main(){
    std::shared_ptr<C> p = std::make_shared<C>();   //引用计数初始化为1

    func(p);      //shared_ptr 允许拷贝! 和当前指针共享所有权, 引用计数加1
    func(p);      //多次也没有问题~ 多个 shared_ptr会共享所有权,引用计数加1

    p -> do_something();  // 正常执行, P指向的地址本来就没有改变

    objlist.clear();  //刚刚P 移交给func的生命周期结束了! 引用计数减2

    p -> do_something();  // 正常执行, 因为引用计数还剩1, 不会被释放

    return 0;      //到这里最后一个引用P也被释放, P指向的对象才终于被释放
}
