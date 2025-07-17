#include<iostream>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

struct TreeNode
{
    int val;
    int height;
    std::unique_ptr<TreeNode> left, right;
    TreeNode() : val(0), height(0), left(nullptr), right(nullptr) {}
    TreeNode(int val) : val(val), height(height), left(nullptr), right(nullptr){}
    // C++11 不能在建構子中使用 move，所以移除這個建構子
};

class AVL_Tree
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
    AVL_Tree() : root(nullptr) {}

    AVL_Tree(int val) : root(new TreeNode(val)) {}

    // copy ctor
    AVL_Tree(const AVL_Tree& other) : root(deepCopy(other.root)) {}

    // move ctor
    AVL_Tree(AVL_Tree&& other) noexcept : root(std::move(other.root)) {}

    // copy & move assignment
    AVL_Tree& operator=(AVL_Tree other) noexcept{
        swap(other);
        return *this;
    }

    // destructor - unique_ptr 會自動清理，所以可以使用預設
    ~AVL_Tree() {}

    void clear(){
        root.reset();  // 釋放整個樹
    }

    void swap(AVL_Tree& other){
        std::swap(root, other.root);
    }

    int height(const std::unique_ptr<TreeNode>& root) const{
        if(root == nullptr){
            return 0;
        }
        return root->height;
    }

    void update(TreeNode* node){
        
    }

    int getBalance(std::unique_ptr<TreeNode>& root){
        if(root == nullptr){
            return 0;
        }
        return (height(root->left) - height(root->right));
    }

    void rotateRight(std::unique_ptr<TreeNode>& root){
        std::unique_ptr<TreeNode> otherNode = std::move(root->left);
        root->left = std::move(otherNode->right);
        otherNode->right = std::move(root);
        root = std::move(otherNode);
    }

    void rotateLeft(std::unique_ptr<TreeNode>& root){
        std::unique_ptr<TreeNode> otherNode = std::move(root->right);
        root->left = std::move(otherNode->left);
        otherNode->left = std::move(root);
        root = std::move(otherNode);
    }

    static std::unique_ptr<TreeNode> rebalance(std::unique_ptr<TreeNode> node){

    }

    static std::unique_ptr<TreeNode> insert(std::unique_ptr<TreeNode> root, int val){
        if(root == nullptr){
            return std::unique_ptr<TreeNode>(new TreeNode(val));
        }
        if(val > root->val){
            root->right = insert(std::move(root->right), val);
        }
        else if(val < root->val){
            root->left = insert(std::move(root->left), val);
        }
        else{
            return root;
        }
        return rebalance(std::move(root));
    }

    bool find(int value){
        return findHelper(root, value);
    }

    bool findHelper(const std::unique_ptr<TreeNode>& node, int value){
        if(node == nullptr){
            return false;
        }

        const TreeNode* curr = node.get();
        while(curr != nullptr){
            if(curr->val > value){
                curr = curr->left.get();
            }
            else if(curr->val < value){
                curr = curr->right.get();
            }
            else{
                return true;
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
        if(node->val < value){
            return removeHelper(node->right, value);
        }
        if(node->val > value){
            return removeHelper(node->left, value);
        }
        if(node->left == nullptr && node->right == nullptr){
            node.reset();
        }
        else if(node->left == nullptr){
            node = std::move(node->right);
        }
        else if(node->right == nullptr){
            node = std::move(node->left);
        }
        else{
            TreeNode* succParent = node.get();
            TreeNode* succ = node->right.get();
            while(succ->left != nullptr){
                succParent = succ;
                succ = succ->left.get();
            }
            node->val = succ->val;
            if(succParent->left.get() == succ){
                succParent->left.reset(succ->right.release());
            }
            else{
                succParent->right.reset(succ->right.release());
            }
        }
        return true;
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

    std::size_t treeSize(){
        std::vector<int> tree = bfsTraverse();
        return tree.size();
    }

    bool isEmpty() const{
        return (root == nullptr);
    }

    
};