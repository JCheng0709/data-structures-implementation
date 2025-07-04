#include <iostream>
#include <utility>
using namespace std;

struct Node
{
    int val{};
    Node* next{nullptr};
    Node() = default;
    Node(int val) : val(val), next(nullptr) {}
    Node(int val, Node* next) : val(val), next(next) {}
};

class LinkedList
{
private:
    Node* head{nullptr};
    Node* tail{nullptr};
    mutable size_t _size{0};
    mutable bool _sizeValid{true};

    size_t calculateSize() const{
        Node* curr = head;
        size_t length = 0;
        while (curr != nullptr)
        {
            length++;
            curr = curr->next;
        }
        return length;
    }

    void invalidateSize() const{
        _sizeValid = false;
    }

    static std::pair<Node*, Node*> deepcopy(Node* srcHead){
        if(srcHead == nullptr){
            return {nullptr, nullptr};
        } 

        Node* newHead = nullptr;

        try
        {
            newHead = new Node(srcHead->val);
            Node* newTail = newHead;
            Node* src = srcHead->next;
            Node* dst = newHead;

            while (src != nullptr)
            {
                dst->next = new Node(src->val);
                dst = dst->next;
                newTail = dst;
                src = src->next;
            }
            return {newHead, newTail};
        }
        catch(const std::bad_alloc&)
        {
            while (newHead != nullptr)
            {
                Node* temp = newHead;
                newHead = newHead->next;
                delete temp;
            }
            throw;
        }       
    }

    void clear(){
        while(head != nullptr){
            Node* curr = head;
            head = head->next;
            delete curr;
        }
        tail = nullptr;
        _size = 0;
        _sizeValid = true;
    }

public:
    // default constructor
    LinkedList() = default;

    // copy constructor
    LinkedList(const LinkedList& other){
        std::pair<Node*, Node*> result = deepcopy(other.head);
        head = result.first;
        tail = result.second;

        if(other._sizeValid){
            _size = other._size;
            _sizeValid = true;
        }
        else{
            _sizeValid = false;
        }
    }

    // move constructor
    LinkedList(LinkedList&& other)noexcept 
        : head(std::exchange(other.head, nullptr)),
          tail(std::exchange(other.tail, nullptr)),
          _size(std::exchange(other._size, 0)),
          _sizeValid(std::exchange(other._sizeValid, true)) {}
    

    // copy assignment
    LinkedList& operator=(LinkedList other){
        swap(other);
        return *this;
    }

    // destructor
    ~LinkedList(){
        clear();
    }

    void swap(LinkedList& other) noexcept { 
        std::swap(head, other.head); 
        std::swap(tail, other.tail);
        std::swap(_size, other._size);
        std::swap(_sizeValid, other._sizeValid);
    }   

    size_t size() const noexcept{
        if(!_sizeValid){
            _size = calculateSize();
            _sizeValid = true;
        }
        return _size;
    }

    bool empty() const noexcept{
        return (head == nullptr);
    }

    void insertFront(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
            _size = 1;
            _sizeValid = true;
        }
        else{
            newNode->next = head;
            head = newNode;
            invalidateSize();
        }
    }

    void insertBack(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
            _size = 1;
            _sizeValid = true;
        }
        else{
            tail->next = newNode;
            tail = newNode;
            invalidateSize();
        }
    }

    void popFront(){
        if(empty()){
            throw std::runtime_error("Cannot pop from empty list");
        }

        Node* temp = head;
        if(head == tail){
            head = tail = nullptr;
            _size = 0;
            _sizeValid = true;
        }
        else{
            head = head->next;
            invalidateSize();
        }
        delete temp;
    }

    void popBack(){
        if(empty()){
            throw std::runtime_error("Cannot pop from empty list");
        }

        Node* temp = tail;
        if(head == tail){
            head = tail = nullptr;
            _size = 0;
            _sizeValid = true;
        }
        else{
            Node* prev = head;
            while(prev->next != nullptr){
                prev = prev->next;
            }

            tail = prev;
            tail->next = nullptr;
            invalidateSize();
        }
        delete temp; 
    }

    Node* getNodeAt(size_t index) const{
        size_t listSize = size();
        if(index >= listSize){
            return nullptr;
        }
        Node* curr = head;
        for(size_t i = 0; i < index; i++){
            curr = curr->next;
        }
        return curr;
    }

    void remove(int value){
        if(head == nullptr){
            return;
        }

        if(head->val == value){
            Node* temp = head;
            if(head == tail){
                head = tail = nullptr;
                _size = 0;
                _sizeValid = true;
            }
            else{
                head = head->next;
                invalidateSize();
            }
            delete temp;
            return;
        }

        Node* curr = head;
        while (curr->next != nullptr){
            if(curr->next->val == value){
                Node* temp = curr->next;
                if(temp == tail){
                    tail = curr;
                    curr->next = nullptr;
                }
                else{
                    curr->next = temp->next;
                }
                delete temp;
                invalidateSize();
            }
            curr = curr->next;
        }
        
    }

    void removeAt(int index){
        if(index < 0 || empty()){
            return;
        }
        if(index == 0){
            popFront();
            return;
        }

        Node* prev = getNodeAt(index - 1);
        if(prev == tail || prev == nullptr){
            return;
        }
        if(prev->next == tail){
            popBack();
            return;
        }
        else{
            Node* temp = prev->next;
            prev->next = prev->next->next;
            delete temp;
            invalidateSize();
        }
    }

    void insertAt(int value, int index){
        if(index < 0){
            return;
        }
        if(index == 0){
            insertFront(value);
            return;
        }

        Node* prev = getNodeAt(index - 1);
        if(prev == tail || prev == nullptr){
            insertBack(value);
        }
        else{
            Node* newNode = new Node(value);
            newNode->next = prev->next;
            prev->next = newNode;
            invalidateSize();
        }
    }    

    void print() const{
        Node* curr = head;
        while(curr != nullptr){
            cout << curr->val << "->";
            curr = curr->next;
        }
        cout << "nullptr" << endl;
    }

    bool find(int value) const{
        if(head == nullptr){
            return false;
        }
        Node * curr = head;
        while (curr != nullptr)
        {
            if(curr->val == value){
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    int front() const{
        if(empty()){
            throw std::runtime_error("List is empty");
        }
        return head->val;
    }

    int back() const{
        if(empty()){
            throw std::runtime_error("List is empty");
        }
        return tail->val;
    }    
};