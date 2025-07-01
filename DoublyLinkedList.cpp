#include <iostream>
using namespace std;

struct Node
{
    int val;
    Node* next;
    Node* prev;
    Node() : val(0), next(nullptr), prev(nullptr) {}
    Node(int val) : val(val), next(nullptr), prev(nullptr) {}
    Node(int val, Node* next, Node* prev) : val(val), next(next), prev(prev) {}
};

class DoublyLinkedList
{
private:
    Node* head;
    Node* tail;
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {};

    void insert(int value){
        Node* newNode = new Node(value);
        if (head == nullptr)
        {
            head = tail = newNode;
            return;
        }

        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
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
                    if(head != nullptr){
                        head->prev = nullptr;
                    }
                    else{
                        tail = nullptr;
                    }
                }
                else if(curr == tail){
                    tail = curr->prev;
                    if(tail != nullptr){
                        tail->next = nullptr;
                    }
                    else{
                        head = nullptr;
                    }
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

    void insertFront(int value){
        Node* newNode = new Node(value);
        if(head == nullptr){
            head = tail = newNode;
            return;
        }

        head->prev = newNode;
        newNode->next = head;
        head = newNode;
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

    void clear(){
        while(head != nullptr){
            Node* curr = head;
            head = head->next;
            delete curr;
        }
        tail = nullptr;
    }

    ~DoublyLinkedList(){
        clear();
    }
};