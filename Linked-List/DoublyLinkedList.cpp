#include <iostream>
#include <utility>
using namespace std;

struct Node
{
    int val{};
    Node* next{nullptr};
    Node* prev{nullptr};
    Node() = default;
    Node(int val) : val(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node* next, Node* prev) : val(val), next(next), prev(prev) {}
};

class DoublyLinkedList
{
private:
    Node* head{nullptr};
    Node* tail{nullptr};

    mutable size_t _size{0};
    mutable bool _sizeValid{true};

    size_t calculateSize() const{
        Node* curr = head;
        size_t length = 0;
        while(curr != nullptr){
            curr = curr->next;
            length++;
        }
        return length;
    }

    void invalidateSize() const{
        _sizeValid = false;
    }

    static std::pair<Node*, Node*> deepCopy(Node* srcHead){
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

            while(src != nullptr){
                dst->next = new Node(src->val);
                dst->next->prev = dst;
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
    // default ctor
    DoublyLinkedList() = default;

    // copy ctor
    DoublyLinkedList(const DoublyLinkedList& other){
        std::pair<Node*, Node*> result = deepCopy(other.head);
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

    // move ctor
    DoublyLinkedList(DoublyLinkedList&& other)noexcept
     : head(std::exchange(other.head, nullptr)),
       tail(std::exchange(other.tail, nullptr)),
       _size(std::exchange(other._size, 0)),
       _sizeValid(std::exchange(other._sizeValid, true)) {}


    // copy & move assignment
    DoublyLinkedList& operator=(DoublyLinkedList other){
        swap(other);
        return *this;
    }

    // destructor
    ~DoublyLinkedList(){
        clear();
    }

    void swap(DoublyLinkedList& other) noexcept{
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
            return;
        }

        newNode->next = head;
        head->prev = newNode;
        head = newNode;
        invalidateSize();
    }

    void pushBack(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
            _size = 1;
            _sizeValid = true;
        }
        else{
            tail->next = newNode;
            newNode->prev = tail;
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
            head->prev = nullptr;
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
            tail = tail->prev;
            tail->next = nullptr;
            invalidateSize();
        }
        delete temp;
    }

    
    Node* getNodeAt(int index) const{
        if(index < 0){
            return nullptr;
        }

        size_t listSize = size();
        if(static_cast<size_t>(index) >= listSize){
            return nullptr;
        }
        if(static_cast<size_t>(index) < listSize / 2){
            Node* curr = head;
            for(int i = 0; i < index; i++){
                curr = curr->next;
            }
            return curr;
        }
        else{
            Node* curr = tail;
            for(size_t i = listSize - 1; i > static_cast<size_t>(index); i--){
                curr = curr->prev;
            }
            return curr;
        }
    }

    void insert(int value, int index){
        if(index < 0){
            return;
        }
        if(index == 0){
            insertFront(value);
            return;
        }

        Node* curr = getNodeAt(index);

        if(curr == nullptr){
            pushBack(value);
        }
        else{
            Node* newNode = new Node(value);
            Node* prevNode = curr->prev;
            newNode->next = curr;
            newNode->prev = prevNode;
            curr->prev = newNode;
            prevNode->next = newNode;
            invalidateSize();
        }
    }

    void remove(int value){
        Node* curr = head;
        while(curr != nullptr){
            if(curr->val == value){
                if(curr == head && curr == tail){
                    head = tail = nullptr;
                }
                else if(curr == head){
                    head = curr->next;
                    head->prev = nullptr;
                }
                else if(curr == tail){
                    tail = curr->prev;
                    tail->next = nullptr;
                }
                else{
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                }
                delete curr;
                invalidateSize();
                return;
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

        Node* curr = getNodeAt(index);
        if(curr != nullptr){
            if(curr == tail){
                popBack();
            }
            else{
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
                invalidateSize();
            }
        }
    }

    void printList() const{
        Node* curr = head;
        while(curr != nullptr){
            cout << curr->val << "->";
            curr = curr->next;
        }
        cout << "nullptr" << endl;
    }

    void printReverse() const{
        Node* curr = tail;
        while(curr != nullptr){
            cout << curr->val << "<-";
            curr = curr->prev;
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