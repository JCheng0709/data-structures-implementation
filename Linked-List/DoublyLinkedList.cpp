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
        if(head == nullptr){
            return;
        }

        if(head->val == value){
            Node* temp = head;
            head = head->next;
            temp = head->prev;
            delete temp;
            return;
        }

        Node* curr = head;
        while(curr->next != nullptr){
            if(curr->next->val == value){
                Node* temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
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
        }
        newNode->next = head;
        head->prev = newNode;
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
        head = tail = nullptr;
    }

    void reverse(){
        Node* curr = head;
        Node* prev = nullptr;
        while(curr != nullptr){
            Node* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }
        head = prev;
    }

    ~DoublyLinkedList(){
        clear();
    }
};
