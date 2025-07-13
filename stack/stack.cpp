#include <iostream>
#include <utility>
#include <stdexcept>
#include <limits>
#include <type_traits>

template <typename T>
class stack
{
private:
    T* data;
    std::size_t ptr;
    std::size_t cap;
    double growth = 2.0;

    void resize(size_t newCap){
        if(newCap <= cap){
            return;
        }
        T* newData = nullptr;
        try
        {
            newData = new T[newCap];
            for(std::size_t i = 0; i < ptr; i++){
                // checks whether type T has a noexcept move constructor
                // if it does, return true, then false
                if (std::is_nothrow_move_constructible<T>::value){
                   newData[i] = std::move(data[i]);
                }
                else{
                    newData[i] = data[i];
                }
            }
        }
        catch(...)
        {
            delete [] newData;
            throw;
        }
        delete [] data;
        data = newData;
        cap = newCap;
    }

    void ensureCapacity(){
        if(ptr >= cap){
            std::size_t newCap = calculateNewCapacity();
            resize(newCap);
        }
    }

    std::size_t calculateNewCapacity(){
        if(cap == 0){
            return 1;
        }

        std::size_t maxCap = std::numeric_limits<size_t>::max() / sizeof(T);
        if(cap > maxCap / growth){
            return maxCap;
        }
        return static_cast<size_t>(cap * growth);
    }
    
public:
    // ctor
    explicit stack(std::size_t cap): cap(cap), ptr(0){
        data = new T[cap];
    }

    // copy ctor
    stack(const stack& other) : data(new T[other.cap]), ptr(other.ptr), cap(other.cap){
        for(std::size_t i = 0; i < ptr; i++){
            data[i] = other.data[i];
        }
    }

    // move ctor
    stack(stack&& other) noexcept
     : data(std::exchange(other.data, nullptr)), 
       ptr(std::exchange(other.ptr, 0)), 
       cap(std::exchange(other.cap, 0)){}

    // copy & move assignment
    stack& operator=(stack other)noexcept{
        swap(other);
        return *this;
    }

    // destructor
    ~stack()noexcept{
        delete[] data;
    }

    void swap(stack& other) noexcept{
        std::swap(data, other.data);
        std::swap(ptr, other.ptr);
        std::swap(cap, other.cap);
    }

    void push(const T& value){
        /*if(ptr >= cap){
            throw runtime_error("stack overflow : cannot push with full stack");
        }*/
        ensureCapacity();
        data[ptr] = value;
        ++ptr;
    }

    void pop(){
        if(ptr == 0){
            throw std::runtime_error("stack underflow : cannot pop with empty stack");
        }
        
        --ptr;
    }

    T& top(){
        if(ptr > 0){
            return data[ptr - 1];
        }
        throw std::runtime_error("Cannot access top of empty stack");
    }

    bool isEmpty() const noexcept{
        return ptr == 0;
    }

    bool isFull() const noexcept{
        return ptr == cap;
    }

    size_t size() const noexcept{
        return ptr;
    }

    void clear() noexcept{
        ptr = 0;
    }

    void print() const {
        std::cout << "Stack (top->bottom): ";    
        for(std::size_t i = 0; i < ptr; i++){
            std::cout << data[ptr - 1 - i] << " ";
        }
        std::cout << std::endl;
    }
};