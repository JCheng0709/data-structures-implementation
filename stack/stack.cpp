#include <iostream>
using namespace std;

template <typename T>
class stack
{
private:
    T* data;
    int ptr;
    int cap;
    
public:
    stack(int cap): cap(cap), ptr(-1){
        data = new T[cap];
    }

    stack(int cap, T defaultValue): cap(cap), ptr(cap - 1){
        data = new T[cap];
        for(int i = 0; i < cap; i++){
            data[i] = defaultValue;
        }
    }

    ~stack(){
        delete[] data;
    }

    bool push(T value){
        if(ptr >= cap - 1){
            cout << "Stack overflow" << endl;
            return false;
        }
        data[++ptr]  = value;
        return true;
    }

    bool pop(){
        if(ptr < 0){
            cout << "Stack is empty" << endl;
            return false;
        }
        --ptr;
        return true;
    }

    T top() const{
        if(ptr >= 0){
            return data[ptr];
        }
        throw runtime_error("Stack is empty");
    }

    bool isEmpty() const{
        return ptr == -1;
    }

    bool isFull() const{
        return ptr == cap - 1;
    }

    int size() const{
        return ptr + 1;
    }

    void clear(){
        ptr = -1;
    }

    void print() const {
        cout << "Stack (top->bottom): ";    
        for(int i = ptr; i >= 0; i--){
            cout << data[i] << " ";
        }
        cout << endl;
    }
};