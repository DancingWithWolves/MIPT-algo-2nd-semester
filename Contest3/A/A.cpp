#include <cstddef>
#include <iostream>
#include <fstream>
#include <istream>
#include <iterator>
#include <ostream>
#include <type_traits>


template <typename key_t> 

class AVLTree {

private:
    class AVLTreeNode {

        private:
            key_t key;

        public:
            size_t sub_tree_size, height;
            AVLTreeNode *left, *right;

            AVLTreeNode(key_t key) : key(key), left(nullptr), right(nullptr), sub_tree_size(1), height(1){}
            ~AVLTreeNode(){}

            key_t getKey(){ return key; }

            
    };
    void deleteNodes (AVLTreeNode *cur)
    {
        if( !cur ) return;
        deleteNodes(cur->left);
        deleteNodes(cur->right);
        delete cur;
    }
    AVLTreeNode *root;
    size_t size;
    

public:
    
    AVLTree() : size(0), root(nullptr){}
    AVLTree(key_t key) : size(1) { root = new AVLTreeNode(key); }
    
    ~AVLTree() 
    {
        deleteNodes(root);
        size = 0;
        root = nullptr;
    }
    
    size_t getHeight (AVLTreeNode *cur) { return !cur ? 0 : cur->height; }
    size_t getSubTreeSize (AVLTreeNode *cur) { return !cur ? 0 : cur->sub_tree_size; }
    int getBalance (AVLTreeNode *cur) { return getHeight(cur->right) - getHeight(cur->left); }

    void fixHeightAndSize (AVLTreeNode *cur) 
    {  
        int height_left = getHeight(cur->left);
        int height_right = getHeight(cur->right);
        cur->height = ( (height_left > height_right) ? height_left : height_right) + 1;
        cur->sub_tree_size = getSubTreeSize(cur->left) + getSubTreeSize(cur->right) + 1;
    }




    AVLTreeNode *rotateRight (AVLTreeNode *cur)
    {
        AVLTreeNode *tmp = cur->left;
        cur->left = tmp->right;
        tmp->right = cur;
        fixHeightAndSize(cur);
        fixHeightAndSize(tmp);
        if(cur == root) root = tmp;
        return tmp;
    }

    AVLTreeNode *rotateLeft (AVLTreeNode *cur)
    {
        AVLTreeNode *tmp = cur->right;
        cur->right = tmp->left;
        tmp->left = cur;
        fixHeightAndSize(cur);
        fixHeightAndSize(tmp);
        if(cur == root) root = tmp;
        return tmp;
    }

    AVLTreeNode *balance (AVLTreeNode *cur)
    {
        fixHeightAndSize(cur);

        if (getBalance(cur) == 2) {
            if (getBalance(cur->right) < 0)
                cur->right = rotateRight(cur->right);
            return rotateLeft(cur);
        }

        if( getBalance(cur) == -2 ) {
            if (getBalance(cur->left) > 0  )
                cur->left = rotateLeft(cur->left);
            return rotateRight(cur);
        }

        return cur;
    }

    AVLTreeNode *insertIter (AVLTreeNode *cur, key_t key) 
    {
        if (!cur)
            return new AVLTreeNode(key);
        cur->sub_tree_size++;
        if (key < cur->getKey())
            cur->left = insertIter(cur->left, key);
        else if (key > cur->getKey())
            cur->right = insertIter(cur->right, key);
        return balance(cur);
    }

    AVLTreeNode *insert(key_t key) 
    {
        if (!root) {
            root = new AVLTreeNode(key);
            return root;
        }
        return insertIter(root, key); 
    }
    
    
    const char* vizFileName = "tree.gpvz";
    void showTreeIter (AVLTreeNode *cur, std::ofstream &out)
    {
        if (!cur) return;

        out << reinterpret_cast <long> (cur) << " [ label = \"" 
        << "key = "
        << cur->getKey()
        << "\n height = "
        << cur->height
        << "\n size = "
        << cur->sub_tree_size
        << "\n balance = "
        << getBalance(cur)
        << "\" ]\n";
        if (cur->left) out << reinterpret_cast <long> (cur) << " -> " << reinterpret_cast <long> (cur->left) << "\n";
        if (cur->right) out << reinterpret_cast <long> (cur) << " -> " << reinterpret_cast <long> (cur->right) << "\n";

        showTreeIter(cur->left, out);

        showTreeIter(cur->right, out);

    }

    void showTree ()
    {
        std::ofstream out;
        out.open( vizFileName );
        out << "digraph {\n";

        showTreeIter(root, out);

        out << "}\n";
        out.close();
        system( "dot tree.gpvz -T png -o tree_image.png" );
        system( "eog tree_image.png" );
    }

    key_t next (key_t key)
    {
        if (!root) return -1;
        AVLTreeNode *cur = root, *prev = root;
        key_t ret = -1;

        while (cur != nullptr) {
            if ( cur->getKey() >= key && (cur->getKey() < ret || ret == -1) ) ret = cur->getKey();
            prev = cur;

            if (key == cur->getKey())
                return key;

            if (key < cur->getKey())
                cur = cur->left;
            else if (key > cur->getKey())
                cur = cur->right;
        }
        
        return ret;
    }

};



int main()
{
    const int magic_constant = 1000000000;
    AVLTree <int> tree;
    
    int n = 0, tmp = 0, add = 0;
    char format = 0;
    std::cin >> n;
    bool needToAdd = false;
    for (int i = 0; i < n; ++i) {
        std::cin >> format;
        std::cin >> tmp;
        if (format == '+') {
            if (needToAdd) {
                tmp = (add+tmp) % magic_constant;
                needToAdd = false;
            }
            tree.insert(tmp);
        }        
        if (format == '?') {
            needToAdd = true;
            add = tree.next(tmp);
            std::cout << add << std::endl;
        }
    }
    
    return 0;
}
