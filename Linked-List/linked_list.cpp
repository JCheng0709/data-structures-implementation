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

    static Node* deepcopy(Node* srcHead){
        if(srcHead == nullptr){
            return nullptr;
        }

        Node* newHead = new Node(srcHead->val);
        Node* src = srcHead->next;
        Node* dst = newHead;

        while (src != nullptr)
        {
            dst->next = new Node(src->val);
            dst = dst->next;
            src = src->next;
        }
        return newHead;
    }

public:
    // default constructor
    LinkedList() = default;

    // copy constructor
    LinkedList(const LinkedList& other) : head(deepcopy(other.head)) {}

    // move constructor
    LinkedList(LinkedList&& other)noexcept : head(std::exchange(other.head, nullptr)) {}

    // copy assignment
    LinkedList& operator=(LinkedList other){
        swap(other);
        return *this;
    }

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

    void print() const{
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

    void swap(LinkedList& other) noexcept { std::swap(head, other.head); }

    // destructor
    ~LinkedList(){
        deleteAll();
    }
};

int main() {
    LinkedList a;
    a.insert(1); a.insert(2); a.insert(3);
    cout << "A : "; a.print();

    LinkedList b = a;           // copy ctor
    cout << "B(copy A): "; b.print();

    LinkedList c(std::move(a)); // move ctor
    cout << "C(move A): "; c.print();
    cout << "A after move : "; a.print();

    LinkedList d;
    d.insert(9);
    d = b;                      // copy =
    cout << "D=b : "; d.print();

    d = std::move(c);           // move =
    cout << "D=move(C): "; d.print();
    cout << "C after move=: ";  c.print();
}