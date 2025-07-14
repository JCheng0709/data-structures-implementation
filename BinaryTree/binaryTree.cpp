#include<iostream>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

struct TreeNode
{
    int val;
    std::unique_ptr<TreeNode> left, right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int val) : val(val), left(nullptr), right(nullptr){}
    // C++11 不能在建構子中使用 move，所以移除這個建構子
};

class binaryTree
{
private:
    std::unique_ptr<TreeNode> root;

    std::unique_ptr<TreeNode> deepCopy(const std::unique_ptr<TreeNode>& srcNode){
        if(srcNode == nullptr){
            return nullptr;  // C++11 可以使用 nullptr
        }

        // C++11 沒有 make_unique，需要手動建立
        std::unique_ptr<TreeNode> newNode(new TreeNode(srcNode->val));
        newNode->left = deepCopy(srcNode->left);
        newNode->right = deepCopy(srcNode->right);
        
        return newNode;
    }

public:
    // ctor
    binaryTree() : root(nullptr) {}

    binaryTree(int val) : root(new TreeNode(val)) {}

    // copy ctor
    binaryTree(const binaryTree& other) : root(deepCopy(other.root)) {}

    // move ctor
    binaryTree(binaryTree&& other) : root(std::move(other.root)) {
        other.root = nullptr;
    }

    // copy & move assignment
    binaryTree& operator=(binaryTree other){
        swap(other);
        return *this;
    }

    // destructor - unique_ptr 會自動清理，所以可以使用預設
    ~binaryTree() {}

    void clear(){
        root.reset();  // 釋放整個樹
    }

    void swap(binaryTree& other){
        std::swap(root, other.root);
    }

    void insert(int val){
        std::unique_ptr<TreeNode> newNode(new TreeNode(val));
        if(root == nullptr){
            root = std::move(newNode);
            return;
        }

        std::queue<TreeNode*> queue;
        queue.push(root.get());
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            if(curr->left == nullptr){
                curr->left = std::move(newNode);
                return;
            }
            else if(curr->right == nullptr){
                curr->right = std::move(newNode);
                return;
            }
            queue.push(curr->left.get());
            queue.push(curr->right.get());
        }
    }

    bool findDFS(int value){
        return findDFSHelper(root, value);
    }

    bool findDFSHelper(const std::unique_ptr<TreeNode>& node, int value){
        if(node == nullptr){
            return false;
        }

        if(node->val == value){
            return true;
        }
        bool left = findDFSHelper(node->left, value);
        bool right = findDFSHelper(node->right, value);

        return (left || right);
    }

    bool findBFS(int value){
        return findBFSHelper(root, value);
    }

    bool findBFSHelper(const std::unique_ptr<TreeNode>& node, int value){
        if(node == nullptr){
            return false;
        }
        std::queue<TreeNode*> queue;
        queue.push(node.get());
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            if(curr->val == value){
                return true;
            }
            if(curr->left != nullptr){
                queue.push(curr->left.get());
            }
            if(curr->right != nullptr){
                queue.push(curr->right.get());
            }
        }
        return false;
    }

    bool remove(int value){
        return removeHelper(root, value);
    }

    bool removeHelper(std::unique_ptr<TreeNode>& node, int value){
        if(node == nullptr){
            return false;
        }
        if(node->val == value){
            deleteNode(node);
            return true;
        }

        bool left = removeHelper(node->left, value);
        bool right = removeHelper(node->right, value);

        return left || right;
    }

    void deleteNode(std::unique_ptr<TreeNode>& node){
        // leaf node
        if(node->left == nullptr && node->right == nullptr){
            node.reset();  // 自動釋放記憶體
        }
        else if(node->left == nullptr){
            node = std::move(node->right);
        }
        else if(node->right == nullptr){
            node = std::move(node->left);
        }
        else{
            twoChildrenDelete(node);
        }
    }

    void twoChildrenDelete(std::unique_ptr<TreeNode>& node){
        std::unique_ptr<TreeNode> leftTree = std::move(node->left);
        std::unique_ptr<TreeNode> rightTree = std::move(node->right);
        
        TreeNode* rightMost = leftTree.get();
        while(rightMost->right != nullptr){
            rightMost = rightMost->right.get();
        }
        rightMost->right = std::move(rightTree);
        node = std::move(leftTree);
    }

    void inorder(){ // left root right
        inorderHelper(root);
    }

    void inorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }

        inorderHelper(node->left);
        std::cout << node->val << std::endl;
        inorderHelper(node->right);
    }

    void preorder(){ // root left right
        preorderHelper(root);
    }

    void preorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }

        std::cout << node->val << std::endl;
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorder(){ // left right root
        postorderHelper(root);
    }

    void postorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }
        postorderHelper(node->left);
        postorderHelper(node->right);
        std::cout << node->val << std::endl;
    }

    std::vector<int> bfsTraverse(){
        return bfsTraverseHelper(root);
    }

    std::vector<int> bfsTraverseHelper(const std::unique_ptr<TreeNode>& node){
        std::vector<int> result;
        if(node == nullptr){
            return result;  // 或者可以直接 return {};
        }

        std::queue<TreeNode*> queue;
        queue.push(node.get());
        while(!queue.empty()){
            TreeNode* curr = queue.front();
            queue.pop();
            result.push_back(curr->val);
            if(curr->left != nullptr){
                queue.push(curr->left.get());
            }
            if(curr->right != nullptr){
                queue.push(curr->right.get());
            }
        }
        return result;
    }

    std::vector<std::vector<int> > bfsTraverseLevel(){  // C++11 需要空格
        return bfsTraverseLevelHelper(root);
    }

    std::vector<std::vector<int> > bfsTraverseLevelHelper(const std::unique_ptr<TreeNode>& node){
        std::vector<std::vector<int> > result;  // C++11 需要空格
        if(node == nullptr){
            return result;
        }

        std::queue<TreeNode*> q;
        q.push(node.get());
        while(!q.empty()){
            std::vector<int> currLevel;
            int nodeInCurrLevel = q.size();
            
            for(int i = 0; i < nodeInCurrLevel; i++){
                TreeNode* curr = q.front();
                q.pop();
                currLevel.push_back(curr->val);

                if(curr->left != nullptr){
                    q.push(curr->left.get());
                }
                if(curr->right != nullptr){
                    q.push(curr->right.get());
                }
                
            }
            result.push_back(currLevel);
        }
        return result;
    }

    // 提供 getter 以便外部測試
    TreeNode* getRoot() const {
        return root.get();
    }

    size_t height(std::unique_ptr<TreeNode>& root){
        if(root == nullptr){
            return 0;
        }

        size_t left = height(root->left);
        size_t right = height(root->right);

        return (left >= right) ? left+1 : right+1;
    }

    size_t treeSize(){
        std::vector<int> tree = bfsTraverse();
        return tree.size();
    }

    bool isEmpty(std::unique_ptr<TreeNode> root){
        return (root == nullptr);
    }
};
