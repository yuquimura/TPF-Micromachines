#ifndef MICROMACHINES_SAFEQUEUE_H
#define MICROMACHINES_SAFEQUEUE_H

#include <bits/stdc++.h>
#include <fstream>
#include <arpa/inet.h>
#include <condition_variable>
#include <queue>
#include <mutex>


template <class T>
class SafeQueue{
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
    std::atomic<bool> open;

public:
    SafeQueue();

    void push(T& event);



    T pop();

    void setOpen(bool v);

    bool isOpen();

    bool isEmpty();

    template<class... Args>
    void emplace(Args&&... args){
        q.emplace(std::forward<Args>(args)...);
    }
};

template<class T>
void SafeQueue<T>::push(T &event) {
    std::unique_lock<std::mutex> lock(this->m);
    this->q.push(event);
    this->cv.notify_one();
}


template<class T>
T SafeQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(this->m);
    while (this->q.empty()){
        if (!open) return T();
        this->cv.wait(lock);
    }
    T comp = this->q.front();
    this->q.pop();
    return comp;
}


template<class T>
SafeQueue<T>::SafeQueue() {
    open = true;
}

template<class T>
bool SafeQueue<T>::isOpen() {
    return open;
}

template<class T>
void SafeQueue<T>::setOpen(bool v) {
    this->open = v;
    this->cv.notify_all();
}

template<class T>
bool SafeQueue<T>::isEmpty() {
    return this->q.empty();
}

#endif //MICROMACHINES_SAFEQUEUE_H
