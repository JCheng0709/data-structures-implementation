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
    TreeNode(int val) : val(val), height(1), left(nullptr), right(nullptr){}
};

class AVL_Tree
{
private:
    std::unique_ptr<TreeNode> root;

    std::unique_ptr<TreeNode> deepCopy(const std::unique_ptr<TreeNode>& srcNode){
        if(srcNode == nullptr){
            return nullptr;
        }

        std::unique_ptr<TreeNode> newNode(new TreeNode(srcNode->val));
        newNode->height = srcNode->height;
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

    // destructor
    ~AVL_Tree() {}

    void clear(){
        root.reset();
    }

    void swap(AVL_Tree& other){
        std::swap(root, other.root);
    }

    static int height(const std::unique_ptr<TreeNode>& root){
        if(root == nullptr){
            return 0;
        }
        return root->height;
    }

    static void update(TreeNode* node){
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    static int getBalance(std::unique_ptr<TreeNode>& root){
        if(root == nullptr){
            return 0;
        }
        return (height(root->left) - height(root->right));
    }

    static std::unique_ptr<TreeNode> rotateRight(std::unique_ptr<TreeNode> node) {
        std::unique_ptr<TreeNode> newRoot = std::move(node->left);
        node->left = std::move(newRoot->right);
        newRoot->right = std::move(node);

        update(newRoot->right.get());
        update(newRoot.get());
        return newRoot;
    }

    static std::unique_ptr<TreeNode> rotateLeft(std::unique_ptr<TreeNode> node) {
        std::unique_ptr<TreeNode> newRoot = std::move(node->right);
        node->right = std::move(newRoot->left);
        newRoot->left = std::move(node);

        update(newRoot->left.get());
        update(newRoot.get());
        return newRoot;
    }

    static std::unique_ptr<TreeNode> rebalance(std::unique_ptr<TreeNode> node){
        update(node.get());
        int bf = getBalance(node);
        if(bf > 1){
            if(getBalance(node->left) < 0){
                node->left = rotateLeft(std::move(node->left));
            } 
            return rotateRight(std::move(node));
        }
        else if(bf < -1){
            if(getBalance(node->right) > 0){
                node->right = rotateRight(std::move(node->right));
            }
            return rotateLeft(std::move(node));
        }
        return node;
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

    // 修正：添加公開的 insert 方法
    void insert(int val){
        root = insert(std::move(root), val);
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

    // 修正：添加公開的 remove 方法
    void remove(int value){
        root = removeHelper(std::move(root), value);
    }

    // 修正：將你的 removeHelper 改為靜態方法，並修正參數類型
    static std::unique_ptr<TreeNode> removeHelper(std::unique_ptr<TreeNode> node, int value){
        if(node == nullptr){
            return nullptr;
        }
        if(node->val < value){
            node->right = removeHelper(std::move(node->right), value);
        }
        else if(node->val > value){
            node->left = removeHelper(std::move(node->left), value);
        }
        else{
            // 找到要刪除的節點
            if(node->left == nullptr && node->right == nullptr){
                return nullptr;
            }
            else if(node->left == nullptr){
                return std::move(node->right);
            }
            else if(node->right == nullptr){
                return std::move(node->left);
            }
            else{
                // 你的原始邏輯，但稍作修正
                TreeNode* succParent = node.get();
                TreeNode* succ = node->right.get();
                while(succ->left != nullptr){
                    succParent = succ;
                    succ = succ->left.get();
                }
                node->val = succ->val;
                if(succParent->left.get() == succ){
                    succParent->left = std::move(succ->right);
                }
                else{
                    succParent->right = std::move(succ->right);
                }
            }
        }
        return rebalance(std::move(node));
    }

    void inorder(){
        inorderHelper(root);
    }

    void inorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }

        inorderHelper(node->left);
        std::cout << node->val << " ";
        inorderHelper(node->right);
    }

    void preorder(){
        preorderHelper(root);
    }

    void preorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }

        std::cout << node->val << " ";
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorder(){
        postorderHelper(root);
    }

    void postorderHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr){
            return;
        }
        postorderHelper(node->left);
        postorderHelper(node->right);
        std::cout << node->val << " ";
    }

    std::vector<int> bfsTraverse(){
        return bfsTraverseHelper(root);
    }

    std::vector<int> bfsTraverseHelper(const std::unique_ptr<TreeNode>& node){
        std::vector<int> result;
        if(node == nullptr){
            return result;
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

    std::vector<std::vector<int> > bfsTraverseLevel(){
        return bfsTraverseLevelHelper(root);
    }

    std::vector<std::vector<int> > bfsTraverseLevelHelper(const std::unique_ptr<TreeNode>& node){
        std::vector<std::vector<int> > result;
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

    // 添加輔助方法來檢查 AVL 性質
    bool isAVL(){
        return isAVLHelper(root);
    }

    bool isAVLHelper(const std::unique_ptr<TreeNode>& node){
        if(node == nullptr) return true;
        
        int balance = height(node->left) - height(node->right);
        if(balance > 1 || balance < -1) return false;
        
        return isAVLHelper(node->left) && isAVLHelper(node->right);
    }

    void printTree(){
    printTreeHelper(root, "", true);
}

void printTreeHelper(const std::unique_ptr<TreeNode>& node, const std::string& prefix, bool isLast){
    if(node == nullptr) return;
    
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << node->val << " (h:" << node->height << ")" << std::endl;
    
    if(node->left != nullptr || node->right != nullptr){
        if(node->right != nullptr){
            printTreeHelper(node->right, prefix + (isLast ? "    " : "│   "), node->left == nullptr);
        }
        if(node->left != nullptr){
            printTreeHelper(node->left, prefix + (isLast ? "    " : "│   "), true);
        }
    }
}
};
