#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
//#include <condition_variable>
//#include <thread>
//#include <functional>
#include <stdexcept>

/*
https://zhuanlan.zhihu.com/p/472021998
1. using Task = function<void()> 是类型别名，简化了 typedef 的用法。function<void()> 可以认为是一个函数类型，接受任意原型是 void() 的函数，或是函数对象，或是匿名函数。void() 意思是不带参数，没有返回值。
2. pool.emplace_back([this]{...}) 和 pool.push_back([this]{...}) 功能一样，只不过前者性能会更好；
3. pool.emplace_back([this]{...}) 是构造了一个线程对象，执行函数是拉姆达匿名函数 ；
4. 所有对象的初始化方式均采用了 {}，而不再使用 () 方式，因为风格不够一致且容易出错；
5. 匿名函数：[this]{...} 不多说。[] 是捕捉器，this 是引用域外的变量 this指针， 内部使用死循环, 由cv_task.wait(lock,[this]{...}) 来阻塞线程；
6. delctype(expr) 用来推断 expr 的类型，和 auto 是类似的，相当于类型占位符，占据一个类型的位置；auto f(A a, B b) -> decltype(a+b) 是一种用法，不能写作 decltype(a+b) f(A a, B b)，为啥？！c++ 就是这么规定的！
7. commit 方法是不是略奇葩！可以带任意多的参数，第一个参数是 f，后面依次是函数 f 的参数！(注意:参数要传struct/class的话,建议用pointer,小心变量的作用域) 可变参数模板是 c++11 的一大亮点，够亮！至于为什么是 Arg... 和 arg... ，因为规定就是这么用的！
8. commit 直接使用只能调用stdcall函数，但有两种方法可以实现调用类成员，一种是使用 bind：.commit(std::bind(&Dog::sayHello, &dog))；一种是用 mem_fn：.commit(std::mem_fn(&Dog::sayHello), &dog)；
9. make_shared 用来构造 shared_ptr 智能指针。用法大体是 shared_ptr<int> p = make_shared<int>(4) 然后 *p == 4 。智能指针的好处就是， 自动 delete ！
10. bind 函数，接受函数 f 和部分参数，返回currying后的匿名函数，譬如 bind(add, 4) 可以实现类似 add4 的函数！
11. forward() 函数，类似于 move() 函数，后者是将参数右值化，前者是... 肿么说呢？大概意思就是：不改变最初传入的类型的引用类型(左值还是左值，右值还是右值)；
12. packaged_task 就是任务函数的封装类，通过 get_future 获取 future ， 然后通过 future 可以获取函数的返回值(future.get())；packaged_task 本身可以像函数一样调用 () ；
13. queue 是队列类， front() 获取头部元素， pop() 移除头部元素；back() 获取尾部元素，push() 尾部添加元素；
14. lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()，是 c++ RAII 的 idea；
15. condition_variable cv; 条件变量， 需要配合 unique_lock 使用；unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()。cv.wait() 之前需要持有 mutex，wait 本身会 unlock() mutex，如果条件满足则会重新持有 mutex。
最后线程池析构的时候,join() 可以等待任务都执行完在结束,很安全!
*/
/**
 * @brief 线程池实现
 * @details 一个线程 pool，一个任务队列 queue ，应该没有意见；
 * 任务队列是典型的生产者-消费者模型，本模型至少需要两个工具：一个 mutex + 一个条件变量，
 * 或是一个 mutex + 一个信号量。mutex 实际上就是锁，保证任务的添加和移除(获取)的互斥性，
 * 一个条件变量是保证获取 task 的同步性：一个 empty 的队列，线程应该等待(阻塞)；
 * atomic<bool> 本身是原子类型，从名字上就懂：它们的操作 load()/store() 是原子操作，所以不需要再加 mutex。
 * 
 */
namespace std
{
//线程池最大容量,应尽量设小一点
#define THREADPOOL_MAX_NUM 16
//#define THREADPOOL_AUTO_GROW

//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不直接支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
class threadpool
{
    using Task = function<void()>; //定义类型
    vector<thread> _pool; //线程池
    queue<Task> _tasks; //任务队列
    mutex _lock; //同步
    condition_variable _task_cv; //条件阻塞
    atomic<bool> _run{ true }; //线程池是否执行
    atomic<int> _idlThrNum{ 0 }; //空闲线程数量

public:
    inline threadpool(unsigned short size = 4) { addThread(size); }
    inline ~threadpool()
    {
        _run=false;
        _task_cv.notify_all(); // 唤醒所有线程执行
        for (thread& thread : _pool) 
        {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
            thread.join(); // 等待任务结束， 前提：线程一定会执行完
        }
    }

public:
    // 提交一个任务
    // 调用.get()获取返回值会等待任务执行完,获取返回值
    // 有两种方法可以实现调用类成员，
    // 一种是使用 bind：.commit(std::bind(&Dog::sayHello, &dog));
    // 一种是用 mem_fn：.commit(std::mem_fn(&Dog::sayHello), this)
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->future<decltype(f(args...))>
    {
        if (!_run) // stoped ??
        throw runtime_error("commit on ThreadPool is stopped.");

        using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
        auto task = make_shared<packaged_task<RetType()>>(
        bind(forward<F>(f), forward<Args>(args)...)
    ); // 把函数入口及参数,打包(绑定)
    future<RetType> future = task->get_future();
    { // 添加任务到队列
        lock_guard<mutex> lock{ _lock };//对当前块的语句加锁 lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
        _tasks.emplace([task](){ // push(Task{...}) 放到队列后面
        (*task)();
        });
        }
        #ifdef THREADPOOL_AUTO_GROW
        if (_idlThrNum < 1 && _pool.size() < THREADPOOL_MAX_NUM)
        addThread(1);
        #endif // !THREADPOOL_AUTO_GROW
        _task_cv.notify_one(); // 唤醒一个线程执行

        return future;
    }

    //空闲线程数量
    int idlCount() { return _idlThrNum; }
    //线程数量
    int thrCount() { return _pool.size(); }
    #ifndef THREADPOOL_AUTO_GROW
private:
    #endif // !THREADPOOL_AUTO_GROW
    //添加指定数量的线程
    void addThread(unsigned short size)
    {
    for (; _pool.size() < THREADPOOL_MAX_NUM && size > 0; --size)
    { //增加线程数量,但不超过 预定义数量 THREADPOOL_MAX_NUM
        _pool.emplace_back( [this]{ //工作线程函数
        while (_run)
        {
            Task task; // 获取一个待执行的 task
            {
                // unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
                unique_lock<mutex> lock{ _lock };
                _task_cv.wait(lock, [this]{
                return !_run || !_tasks.empty();
                }); // wait 直到有 task
                if (!_run && _tasks.empty())
                return;
                task = move(_tasks.front()); // 按先进先出从队列取一个 task
                _tasks.pop();
            }
            _idlThrNum--;
            task();//执行任务
            _idlThrNum++;
        }});
        _idlThrNum++;
    }
    }
};

}

#endif 