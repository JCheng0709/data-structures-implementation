#include <iostream>
#include <utility>
#include <stdexcept>
#include <limits>
#include <type_traits>
#include <stdexcept>

template <typename T>

class circularQueue
{
private:
    T* data;
    std::size_t cap;
    std::size_t frontIdx;
    std::size_t rearIdx;
    std::size_t count;

public:
    // deafult constructor
    explicit circularQueue(int cap): cap(cap), frontIdx(0), rearIdx(0), count(0){
        data = new T[cap];
    }

    // ctor
    circularQueue(int cap, T defaultValue) : circularQueue(cap) {
        for(int i = 0; i < cap; i++){
            data[i] = defaultValue;
        }
    }

    // copy ctor
    circularQueue(const circularQueue& other) : cap(other.cap), frontIdx(0), rearIdx(0), count(0) {
        data = new T[cap];
        for(int i = 0; i < other.count; i++){
            enqueue(other.data[(other.frontIdx + i) % other.cap]);
        }
    }

    // move ctor
    circularQueue(circularQueue&& other) noexcept : 
        data(std::exchange(other.data, nullptr)), 
        cap(std::exchange(other.cap, 0)),
        frontIdx(std::exchange(other.frontIdx, 0)), 
        rearIdx(std::exchange(other.rearIdx, 0)), 
        count(std::exchange(other.count, 0)){}

    // copy & move assignment
    circularQueue& operator=(circularQueue other){
        swap(other);
        return *this;
    }

    // destructor
    ~circularQueue(){
        delete [] data;
    }

    void swap(circularQueue& other) noexcept {
        std::swap(data, other.data);
        std::swap(cap, other.cap);
        std::swap(frontIdx, other.frontIdx);
        std::swap(rearIdx, other.rearIdx);
        std::swap(count, other.count);
    }

    void enqueue(const T& value){
        if(count == cap){
            throw std::overflow_error("Queue is full");
        }
        data[rearIdx] = value;
        rearIdx = (rearIdx + 1) % cap;
        count++;
    }

    void dequeue(){
        if(count == 0){
            throw std::underflow_error("Queue is empty");
        }
        frontIdx = (frontIdx + 1) % cap;
        count--;
    }

    const T& front() const{
        if(count == 0){
            throw std::runtime_error("Queue is empty");
        }
        return data[frontIdx];
    }

    const T& back() const{
        if(count == 0){
            throw std::runtime_error("Queue is empty");
        }
        return data[(rearIdx - 1 + cap) % cap];
    }

    bool empty() const{
        return count == 0;
    }

    bool isFull() const{
        return count == cap;
    }

    std::size_t capacity(){
        return cap;
    }

    std::size_t size() const{
        return count;
    }

    void clear(){
        frontIdx = rearIdx = 0;
        count = 0;
    }

    void print() const{
        std::cout << "Queue(front -> back) : ";
        for(std::size_t i = 0; i < count; i++){
            std::cout << data[(frontIdx + i) % cap] << " ";
        }
        std::cout << std::endl;
    }

    
};