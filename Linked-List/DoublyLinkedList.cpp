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

    static std::pair<Node*, Node*> deepCopy(Node* srcHead){
        if(srcHead == nullptr){
            return {nullptr, nullptr};
        }

        Node* newHead = new Node(srcHead->val);
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

    void clear(){
        while(head != nullptr){
            Node* curr = head;
            head = head->next;
            delete curr;
        }
        tail = nullptr;
    }

public:
    // default ctor
    DoublyLinkedList() = default;

    // copy ctor
    DoublyLinkedList(const DoublyLinkedList& other){
        std::pair<Node*, Node*> result = deepCopy(other.head);
        head = result.first;
        tail = result.second;
    }

    // move ctor
    DoublyLinkedList(DoublyLinkedList&& other)noexcept : head(std::exchange(other.head, nullptr)), tail(std::exchange(other.tail, nullptr)) {}

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
    }

    int size() const{
        Node* curr = head;
        int length = 0;
        while (curr != nullptr)
        {
            length++;
            curr = curr->next;
        }
        return length;
    }

    bool empty() const{
        return (head == nullptr);
    }

    void pushFront(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
            return;
        }

        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    void pushBack(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
        }
        else{
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void popFront(){
        if(empty()){
            return;
        }

        Node* temp = head;
        if(head == tail){
            head = tail = nullptr;
        }
        else{
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
    }

    void popBack(){
        if(empty()){
            return;
        }

        Node* temp = tail;
        if(head == tail){
            head = tail = nullptr;
        }
        else{
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
    }

    /*
    Node* getNodeAt(int index) const{
        if(index < 0){
            return nullptr;
        }
        if(index < size() / 2){

        }
    }*/

    void insert(int value, int index){
        if(index < 0){
            return;
        }
        if(index == 0){
            pushFront(value);
            return;
        }

        Node* curr = head;
        int steps = 0;
        while(curr != nullptr){
            if(steps < index){
                curr = curr->next;
                steps++;
            }
            else{
                break;
            }
        }

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

        Node* curr = head;
        int steps = 0;
        while(curr != nullptr){
            if(steps < index){
                curr = curr->next;
                steps++;
            }
            else{
                break;
            }
        }

        if(curr != nullptr){
            if(curr == tail){
                popBack();
            }
            else{
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
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