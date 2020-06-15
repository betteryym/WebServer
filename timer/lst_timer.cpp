#include "lst_timer.h"
#include "../http/http_conn.h"

sort_timer_lst::sort_timer_lst(){
    head = NULL;
    tail = NULL;
}

sort_timer_lst::~sort_timer_lst(){
    util_timer* tmp = head;
    while(tmp){
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
}

void sort_timer_lst::add_timer(util_timer* timer){
    if(!timer){
        return;
    }
    if(!head){
        head = tail = timer;
        return;
    }
    if(timer->expire < head->expire){
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    add_timer(timer, head);
}

void sort_timer_lst::adjust_timer(util_timer* timer){
    if(!timer){
        return;
    }
    util_timer* tmp = timer->next;
    /*如果被调整的目标定时器在链表尾部，或者该定时器新的超时值仍然小于
    下一个定时器的超时值，则不用调整*/
    if(!tmp || (timer->expire < tmp->expire)){
        return;
    }
    /*如果目标定时器是链表的头结点，则将该定时器从链表中取出并重新插入链表*/
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        timer->next = NULL:
        add_timer(timer, head);
    }
    /*如果目标定时器不是链表的头结点，将该定时器从链表中取出，
    然后插入原来所在位置之后的部分链表中*/
    else{
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer, timer->next);
    }
}

void sort_timer_lst::del_timer(util_timer* timer){
    if(!timer){
        return;
    }
    if((timer == head) && (timer == tail)){
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
    if(timer == tail){
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        return;
    }
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}

void sort_timer_lst::tick(){
    if(!head){
        return;
    }

    time_t cur = time(NULL):
    util_timer* tmp = head;
    while(tmp){
        if(cur < tmp->expire){
            break;
        }
        tmp->cb_func(tmp->user_data);
        head = tmp->next;
        if(head){
            head->prev = NULL:
        }
        delete tmp;
        tmp = head;
    }
}

void sort_timer_lst::add_timer(util_timer* timer, util_timer* lst_head){
    util_timer* prev = lst_head;
    util_timer* tmp = prev->next;
    while(tmp){
        if(timer->expire < tmp->expire){
            prev->next = timer;
            timer->next = prev;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if(!tmp){
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}

