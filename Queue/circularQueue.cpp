#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>

class circularQueue
{
private:
    T* data;
    int cap;
    int frontIdx;
    int rearIdx;
    int count;

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
            enque(other.data[(other.frontIdx + i) % other.cap]);
        }
    }

    // move ctor
    circularQueue(circularQueue&& other) noexcept : 
        data(std::exchange(other.data, nullptr)), 
        cap(other.cap), frontIdx(other.frontIdx), 
        rearIdx(other.rearIdx), count(other.count){

            other.cap = 0;
            other.frontIdx = 0;
            other.rearIdx = 0;
            other.count = 0;
    }

    // copy & move assignment
    circularQueue& operator=(circularQueue other){
        swap(other);
        return *this;
    }

    // destructor
    ~circularQueue(){
        delete [] data;
    }

    bool enque(const T& value){
        if(count == cap){
            std::throw overflow_error("Queue is full");
        }
        data[rearIdx] = value;
        rearIdx = (rearIdx + 1) % cap;
        count++;
        return true;
    }

    bool deque(){
        if(count == 0){
            std::throw underflow_error("Qeueu is empty");
        }
        
        frontIdx = (frontIdx + 1) % cap;
        count--;
        return true;
    }

    T front() const{
        if(count == 0){
            throw std::runtime_error("Queue is empty");
        }
        return data[frontIdx];
    }

    T back() const{
        if(count == 0){
            throw std::runtime_error("Queue is empty");
        }
        return data[(rearIdx - 1 + cap) % cap];
    }

    bool isEmpty() const{
        return count == 0;
    }

    bool isFull() const{
        return count == cap;
    }

    int capacity(){
        return cap;
    }

    int size() const{
        return count;
    }

    void clear(){
        frontIdx = rearIdx = 0;
        count = 0;
    }

    void print() const{
        std::cout << "Queue(front -> back) : ";
        for(int i = 0; i < count; i++){
            std::cout << data[(frontIdx + i) % cap] << " ";
        }
        std::cout << std::endl;
    }

    void swap(circularQueue& other) noexcept {
        std::swap(data, other.data);
        std::swap(cap, other.cap);
        std::swap(frontIdx, other.frontIdx);
        std::swap(rearIdx, other.rearIdx);
        std::swap(count, other.count);
    }
};

int main() {
    try {
        circularQueue<int> q(3);
        
        q.enque(1);
        q.enque(2);
        q.enque(3);
        q.print();  // 1 2 3
        
        std::cout << "Front: " << q.front() << std::endl;  // 1
        std::cout << "Back: " << q.back() << std::endl;    // 3
        
        q.deque();
        q.print();  // 2 3
        
        // 測試 copy
        circularQueue<int> q2 = q;
        q2.print();  // 2 3
        
        // 測試 move
        circularQueue<int> q3 = std::move(q);
        q3.print();  // 2 3
        q.print();   // empty
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}