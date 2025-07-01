#include <iostream>
using namespace std;

struct Node
{
    int val;
    Node* next;
    Node() : val(0), next(nullptr) {}
    Node(int val) : val(val), next(nullptr) {}
    Node(int val, Node* next) : val(val), next(next) {}
};

class LinkedList
{
private:
    Node* head;
public:
    LinkedList() : head(nullptr) {};

    void insert(int value){
        Node* newNode = new Node(value);
        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        Node* curr = head;
        while(curr->next != nullptr){
            curr = curr->next;
        }
        curr->next = newNode;
    }

    void remove(int value){
        if(head == nullptr){
            return;
        }

        if(head->val == value){
            Node* temp = head;
            head = head->next;
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

    void deleteAll(){
        while(head != nullptr){
            Node* curr = head;
            head = head->next;
            delete curr;
        }
    }

    ~LinkedList(){
        while(head != nullptr){
            Node* curr = head;
            head = head->next;
            delete curr;
        }
    }
};