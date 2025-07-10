#include<iostream>
#include <utility>
#include <vector>
#include <queue>

struct TreeNode
{
    int val{};
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
    TreeNode() = default;
    TreeNode(int val) : val(val), left(nullptr), right(nullptr){}
    TreeNode(int val, TreeNode* left, TreeNode* right) : val(val), left(left), right(right){}
};

class binaryTree
{
private:
    TreeNode* root{nullptr};

    TreeNode* deepCopy(TreeNode* srcNode){
        if(srcNode == nullptr){
            return nullptr;
        }

        TreeNode* newNode = new TreeNode(srcNode->val);
        newNode->left = deepCopy(srcNode->left);
        newNode->right = deepCopy(srcNode->right);
        
        return newNode;
    }

public:
    // ctor
    binaryTree() = default;

    binaryTree(int val) : root(new TreeNode(val)) {}

    // copy ctor
    binaryTree(const binaryTree& other) : root(deepCopy(other.root)) {}

    // move ctor
    binaryTree(binaryTree&& other)noexcept : root(std::exchange(other.root, nullptr)) {}

    // copy & move assignment
    binaryTree& operator=(binaryTree other){
        swap(other);
        return *this;
    }

    // destructor
    ~binaryTree();

    void swap(binaryTree& other) noexcept{
        std::swap(root, other.root);
    }

    void insert(int val){
        TreeNode* newNode = new TreeNode(val);
        if(root == nullptr){
            root = newNode;
            return;
        }

        std::queue<TreeNode*> queue;
        queue.push(root);
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            if(curr->left == nullptr){
                curr->left = newNode;
                return;
            }
            else if(curr->right == nullptr){
                curr->right = newNode;
                return;
            }
            queue.push(curr->left);
            queue.push(curr->right);
        }
    }

    bool findDFS(TreeNode* root, int value){
        if(root == nullptr){
            return false;
        }

        if(root->val == value){
            return true;
        }
        bool left = findDFS(root->left, value);
        bool right = findDFS(root->right, value);

        return (left || right);
    }

    bool findBFS(TreeNode* root, int value){
        if(root == nullptr){
            return false;
        }
        std::queue<TreeNode*> queue;
        queue.push(root);
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            if(curr->val == value){
                return true;
            }
            if(curr->left != nullptr){
                queue.push(curr->left);
            }
            if(curr->right != nullptr){
                queue.push(curr->right);
            }
        }
        return false;
    }

    bool remove(int value){
        return removeHelper(root, value);
    }


    bool removeHelper(TreeNode*& root, int value){
        if(root == nullptr){
            return false;
        }
        if(root->val == value){
            deleteNode(root);
            return true;
        }

        bool left = removeHelper(root->left, value);
        bool right = removeHelper(root->right, value);

        return left || right;
    }

    void deleteNode(TreeNode*& root){
        // leaf node
        if(root->left == nullptr && root->right == nullptr){
            delete root;
            root = nullptr;
        }
        else if(root->left == nullptr){
            TreeNode* temp = root;
            root = root->right;
            delete temp;
        }
        else if(root->right == nullptr){
            TreeNode* temp = root;
            root = root->left;
            delete temp;
        }
        else{
            twoChildrenDelete(root);
        }
    }

    void twoChildrenDelete(TreeNode*& node){
        TreeNode* leftTree = node->left;
        TreeNode* rightTree = node->right;
        TreeNode* rightMost = leftTree;
        while(rightMost->right != nullptr){
            rightMost = rightMost->right;
        }
        rightMost->right = rightTree;
        delete node;
        node = leftTree;
    }

    void inorder(TreeNode* root){ // left root right
        if(root == nullptr){
            return;
        }

        inorder(root->left);
        std::cout << root->val << std::endl;
        inorder(root->right);
        return;
    }

    void preorder(TreeNode* root){ // root left right
        if(root == nullptr){
            return;
        }

        std::cout << root->val << std::endl;
        preorder(root->left);
        preorder(root->right);
        return;
    }

    void postorder(TreeNode* root){ // left right root
        if(root == nullptr){
            return;
        }
        postorder(root->left);
        postorder(root->right);
        std::cout << root->val << std::endl;
        return;
    }

    std::vector<int> bfsTraverse(TreeNode* root){
        std::vector<int> result;
        if(root == nullptr){
            return {};
        }

        std::queue<TreeNode*> queue;
        queue.push(root);
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            result.push_back(curr->val);
            if(curr->left != nullptr){
                queue.push(curr->left);
            }
            if(curr->right != nullptr){
                queue.push(curr->right);
            }
        }
        return result;
    }

    std::vector<std::vector<int>> bfsTraverseLevel(TreeNode* root){
        std::vector<std::vector<int>> result;
        if(root == nullptr){
            return result;
        }

        std::queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()){
            std::vector<int> currLevel;
            int nodeInCurrLevel = q.size();
            
            for(int i = 0; i < nodeInCurrLevel; i++){
                TreeNode* curr = q.front();
                q.pop();
                currLevel.push_back(curr->val);

                if(curr->left != nullptr){
                    q.push(curr->left);
                }
                if(curr->right != nullptr){
                    q.push(curr->right);
                }
                
            }
            result.push_back(currLevel);
        }
        return result;
    }
};
