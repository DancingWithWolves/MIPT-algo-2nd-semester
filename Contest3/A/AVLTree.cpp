#include <cstddef>
#include <iostream>
#include <fstream>
#include <istream>
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
    
    AVLTree() : size(0){}
    AVLTree(key_t key) : size(1) { root = new AVLTreeNode(key); }
    
    ~AVLTree() 
    {
        deleteNodes(root);
        size = 0;
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
        else return nullptr;
        return balance(cur);
    }
    AVLTreeNode *insert(key_t key) { return insertIter(root, key); }
    
    
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

};



int main()
{
    AVLTree <int> tree (1);
    tree.insert(2);
    tree.insert(2);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(-3);
    tree.showTree();
    
    
    return 0;
}
