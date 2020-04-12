#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
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
    

public:
    
    int k;
    AVLTree() : root(nullptr), k(0){}
    AVLTree(key_t key) : k(0){ root = new AVLTreeNode(key); }
    
    ~AVLTree() 
    {
        deleteNodes(root);
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

    AVLTreeNode *findMin (AVLTreeNode *cur)
    {
        return cur->left ? findMin(cur->left) : cur;
    }

    AVLTreeNode *removeMin (AVLTreeNode *cur)
    {
        if (!cur->left)
            return cur->right;
        cur->left = removeMin(cur->left);
        return balance(cur);
    }


    AVLTreeNode *removeIter (AVLTreeNode *cur, key_t key)
    {
        if (!cur) return nullptr;
        if (key < cur->getKey())
            cur->left = removeIter(cur->left, key);
        else if (key > cur->getKey())
            cur->right = removeIter(cur->right, key);
        //когда нашли ключ
        else {
            AVLTreeNode *left = cur->left, *right= cur->right;
            delete cur;

            if (!right)
                return left;
            AVLTreeNode *min = findMin(right);
            min->right = removeMin(right);
            min->left = left;
            
            return balance(min);
        }
        return balance(cur);
    }

    void remove (key_t key) {
        if (!root) return;
        if (root->getKey() == key && root->sub_tree_size == 1) {
            delete root;
            root = nullptr;
            return;
        }
        else if (root->getKey() == key) 
            root = removeIter(root, key);
        else removeIter(root, key);

    }

    
    AVLTreeNode *findMaxIter (int k, AVLTreeNode *cur)
    {
        
 
        int curK = 1;
        if (cur->right != nullptr) {
            curK += cur->right->sub_tree_size;
        }
    
        if (k == curK) {
            return cur;
        } else if (k < curK) {
            return findMaxIter(k, cur->right);
        } else {
            return findMaxIter(k - curK, cur->left);
        }
        /*if (!cur) 
            return nullptr;
        AVLTreeNode *right = findMaxIter(cur->right);
        if (right)
            return right;

        this->k--;
        if (k == 0)
            return cur;
        
        return findMaxIter(cur->left);*/

    }

    key_t findKthMax (int k) { this->k = k; return findMaxIter(k, root)->getKey(); }
    

};

int main()
{
    AVLTree <long> tree;

    /*long *arr = new long [100000];
    int qty = 0;
    for (int i = 0; i < 100000; ++i) {
        int coin = random()%3;
        if (coin == 0) { //insert
            long tmp = random()%1000;
            tree.insert(tmp);
            arr[qty++] = tmp;
            std::sort(arr, arr+qty);
            std::cout << "insert " << tmp << "\n";
        }
        if (coin == 1) { //remove
            if (qty > 0){
                long tmp = random()%qty;
                std::cout << "remove " << arr[tmp] << "\n";
                tree.remove(arr[tmp]);
                for (int j = tmp; j < qty-1; j++) {
                    arr[j] = arr[j+1];
                }
                qty--;
            }
        }
        if (coin == 2) { //find max
            if (qty > 0) {
                long tmp = random()%qty;
                long tmp2 = tree.findKthMax(tmp+1);
                std::cout << "find " << tmp << "'s maximum\n";
                if (tmp2 != arr[qty-tmp-1]) {
                    std::cout << "ERROR! must be: " << arr[qty-tmp-1] << " is: " << tmp2 << "\n";
                    for (int j = 0; j < qty; ++j ) {
                        std::cout << arr[j] << " ";
                    }
                    std::cout << "\n";
                    tree.showTree();
                    break;
                }
            }
        }
        //tree.showTree();
    }*/

    

    int n = 0;
    std::cin >> n;
    char format = 0;
    long tmp = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> format;
        switch (format) {
            case '+':
                std::cin >> format >> tmp;
                tree.insert(tmp);
                break;
            case '1':
                std:: cin >> tmp;
                tree.insert(tmp);
                break;
            case '0':
                std::cin >> tmp;
                std::cout << tree.findKthMax(tmp) << "\n";
                break;
            case '-':
                std::cin >> format >> tmp;
                tree.remove(tmp);
                break;
        }
        //tree.showTree();
    }
    return 0;
}