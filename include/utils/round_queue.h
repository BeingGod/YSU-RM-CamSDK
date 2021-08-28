/**
 * @file round_queue.h
 * @author BeingGod (1085779572@qq.com)
 * @brief 定制化循环队列
 * @version 1.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ROUND_QUEUE_H_
#define _ROUND_QUEUE_H_

/**
 * @brief 定制化的循环队列 
 * 
 * @tparam T 容器中数据类型
 */
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
    int m_front_; //!< 队头元素下标
    int m_rear_;  //!< 队尾元素下标
    int m_size_;  //!< 队列容量
};

#endif