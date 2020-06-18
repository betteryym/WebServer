#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem{
public:
    //无名信号，初始化时第二个变量==0，进程内线程共享
    //第三个变量：信号初始值
    sem(){
        if(sem_init(&m_sem, 0, 0) != 0){
            throw std::exception();
        }
    }
    sem(int num){
        if(sem_init(&m_sem, 0, num) != 0){
            throw std::exception();
        }
    }
    ~sem(){
        sem_destroy(&m_sem);
    }
    //被用来阻塞当前线程直到信号量sem的值大于0，
    //解除阻塞后将sem的值减一，表明公共资源经使用后减少。
    bool wait(){
        return sem_wait(&m_sem) == 0;
    }
    //用来增加信号量的值。当有线程阻塞在这个信号量上时，
    //调用这个函数会使其中的一个线程不再阻塞，选择机制同样是由线程的调度策略决定的
    bool post(){
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};

class locker{
public:
    locker(){
        if(pthread_mutex_init(&m_mutex, NULL) != 0){
            throw std::exception();
        }
    }
    ~locker(){
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock(){
        return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex) == 0;
    }
    pthread_mutex_t* get(){
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

class cond{
public:
    cond(){
        if(pthread_cond_init(&m_cond, NULL) != 0){
            throw std::exception();
        }
    }
    ~cond(){
        pthread_cond_destroy(&m_cond);
    }
    bool wait(pthread_mutex_t* m_mutex){
        int ret = 0;
        //wait函数，mutex用于保护条件变量的互斥量，保证该操作的原子性
        //把调用线程放入条件变量的等待队列中，然后将互斥锁解锁
        //也就是从该函数开始执行到其调用线程被放入条件变量的等待队列的这段时间里
        //条件变量不会被修改
        ret = pthread_cond_wait(&m_cond, m_mutex);
        return ret == 0;
    }
    bool timewait(pthread_mutex_t* m_mutex, struct timespec t){
        int ret = 0;
        ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        return ret == 0;
    }
    bool signal(){
        return pthread_cond_signal(&m_cond) == 0;
    }
    bool broadcast(){
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
};
#endif