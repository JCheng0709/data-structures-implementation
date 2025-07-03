#include <iostream>
using namespace std;

template <typename T>
class queue
{
private:
    T* data;
    int cap;
    int frontIdx;
    int rearIdx;
public:
    queue(int cap) : cap(cap), frontIdx(0), rearIdx(0){
        data = new T[cap];
    }

    queue(int cap, T defaultValue) : cap(cap), frontIdx(0), rearIdx(0){
        data = new T[cap];
        for(int i = 0; i < cap; i++){
            data[i] = defaultValue;
            rearIdx++;
        }
    }

    ~queue(){
        delete [] data;
    }

    bool enqueue(T value){
        if(rearIdx >= cap){
            cout << "Queue is overflow" << endl;
            return false;
        }

        data[rearIdx++] = value;
        return true;
    }

    bool dequeue(){
        if(rearIdx == frontIdx){
            cout << "Queue is empty" << endl;
            return false;
        }

        frontIdx++;
        return true;
    }

    T front() const{
        if(rearIdx == frontIdx){
            throw runtime_error("Queue is empty");
        }
        return data[frontIdx];
    }

    bool isEmpty() const{
        return (frontIdx == rearIdx);
    }

    int size() const{
        return rearIdx - frontIdx;
    }

    void print() const{
        cout << "Queue(front -> rear) : ";
        for(int i = frontIdx; i < rearIdx; i++){
            cout << data[i] << " ";
        }
        cout << endl;
    }

    void clear(){
        frontIdx = rearIdx = 0;
    }
};