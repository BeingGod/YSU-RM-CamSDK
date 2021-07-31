/*
 * @Copyright: © 2021, BeingGod. All rights reserved.
 * @Author: BeingGod
 * @Date: 2021-07-08 17:27:09
 * @LastEditors: BeingGod
 * @LastEditTime: 2021-07-10 23:00:58
 * @Description: 定制化循环队列
 */

#ifndef _ROUND_QUEUE_H_
#define _ROUND_QUEUE_H_

template<class T>
class RoundQueue
{
public:
    T *p_array_;

    RoundQueue(int size=10)
        : m_front_(0), m_rear_(0), m_size_(size)
    {
        p_array_ = new T[size];
    }

    ~RoundQueue()
    {
        delete[] p_array_;
    }

    T front()
    {
        return p_array_[m_front_];
    }
    
    T back()
    {
        return p_array_[(m_rear_-1)%m_size_];
    }

    bool empty()
    {
        return m_front_ == -1 && m_rear_ == -1;
    }

    int size()
    {
        return m_size_;
    }

    void push(T elem)
    {
        p_array_[m_rear_] = elem;
        m_rear_ = (m_rear_ + 1) % m_size_;
    }

private:
    int m_front_; // 队头元素下标
    int m_rear_; // 队尾元素下标
    int m_size_; // 队列容量
};

#endif