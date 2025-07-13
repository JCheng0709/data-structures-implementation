#include <iostream>
#include <utility>
#include <stdexcept>
#include <limits>
#include <type_traits>
using namespace std;

template <typename T>
class queue
{
private:
    T* data;
    std::size_t cap;
    std::size_t frontIdx;
    std::size_t rearIdx;
    double growth = 2.0;

    void resize(size_t newCap){
        if(newCap <= cap){
            return;
        }
        T* newData = new T[newCap];
        size_t validCount = rearIdx - frontIdx;
        try
        {
            //newData = new T[newCap];
            for(size_t i = 0; i < validCount; i++){
                if(std::is_nothrow_move_constructible<T>::value){
                    newData[i] = std::move(data[i + frontIdx]);
                }
                else{
                    newData[i] = data[i + frontIdx];
                }
            }
        }
        catch(...)
        {
            delete [] newData;
            throw;
        }

        delete [] data;
        rearIdx = validCount;
        frontIdx = 0;
        data = newData;
        cap = newCap;
    }

    void ensureCapacity(){
        if(rearIdx >= cap){
            size_t newCap = calculateNewCap();
            resize(newCap);
        }
    }

    std::size_t calculateNewCap(){
        if(cap == 0){
            return 1;
        }

        std::size_t maxCap = std::numeric_limits<size_t>::max() / sizeof(T);
        if(cap > maxCap / growth){
            return maxCap;
        }
        return static_cast<std::size_t>(cap * growth);
    }

public:
    // ctor
    explicit queue(std::size_t cap) : cap(cap), frontIdx(0), rearIdx(0){
        data = new T[cap];
    }

    // destructor
    ~queue()noexcept{
        delete [] data;
    }

    // copy ctor
    queue(const queue& other) : data(new T[other.cap]), cap(other.cap), frontIdx(other.frontIdx), rearIdx(other.rearIdx){
        for(size_t i = frontIdx; i < rearIdx; i++){
            data[i] = other.data[i];
        }
    }

    // move ctor
    queue(queue&& other) noexcept
    : data(std::exchange(other.data, nullptr)),
      cap(std::exchange(other.cap, 0)),
      frontIdx(std::exchange(other.frontIdx, 0)),
      rearIdx(std::exchange(other.rearIdx, 0)){}

    // copy & move assignment
    queue& operator=(queue other)noexcept{
        swap(other);
        return *this;
    }

    void swap(queue& other)noexcept{
        std::swap(data, other.data);
        std::swap(cap, other.cap);
        std::swap(frontIdx, other.frontIdx);
        std::swap(rearIdx, other.rearIdx);
    }

    void enqueue(const T& value){
        /*if(rearIdx >= cap){
            throw runtime_error("Queue is full");
        }*/
        ensureCapacity();
        data[rearIdx] = value;
        rearIdx++;
    }

    void dequeue(){
        if(rearIdx == frontIdx){
            throw std::runtime_error("Queue is empty");
        }
        //data[frontIdx].~T();
        frontIdx++;
    }

    const T& front() const{
        if(rearIdx == frontIdx){
            throw std::runtime_error("Queue is empty");
        }
        return data[frontIdx];
    }

    const T& back() const{
        if(rearIdx == frontIdx){
            throw std::runtime_error("Queue is empty");
        }
        return data[rearIdx - 1];
    }

    bool empty() const{
        return (frontIdx == rearIdx);
    }

    size_t size() const{
        return rearIdx - frontIdx;
    }

    void print() const{
        std::cout << "Queue(front -> rear) : ";
        for(std::size_t i = frontIdx; i < rearIdx; i++){
            cout << data[i] << " ";
        }
        cout << endl;
    }

    void clear(){
        /*for(size_t i = frontIdx; i < rearIdx; i++){
            data[i].~T();
        }*/
        frontIdx = rearIdx = 0;
    }
};