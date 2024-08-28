#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Node {
    int value;
    Node* left;
    Node* right;
    int height;

    Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int height(Node* node) {
        // if node exists, return height, else return 0
        if (node){
            return node->height;
        } else {
            return 0;
        }       
    }

    int balance(Node* node) {
        // if node exists, return balance, else return 0
        if (node){
            return height(node->left) - height(node->right);
        } else {
            return 0;
        }  
    }

    Node* rotate_right(Node* y) {
        Node* temp1 = y->left;
        Node* temp2 = temp1->right;
        temp1->right = y;
        y->left = temp2;
        // recalculate height recursively
        y->height = std::max(height(y->left), height(y->right)) + 1;
        temp1->height = std::max(height(temp1->left), height(temp1->right)) + 1;
        return temp1;
    }

    Node* rotate_left(Node* x) {
        Node* temp1 = x->right;
        Node* temp2 = temp1->left;
        temp1->left = x;
        x->right = temp2;
        // recalculate height recursively
        x->height = std::max(height(x->left), height(x->right)) + 1;
        temp1->height = std::max(height(temp1->left), height(temp1->right)) + 1;
        return temp1;
    }

    Node* insert(Node* node, int value) {
        if (!node) {
            return new Node(value);
        }
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node;
        }

        node->height = std::max(height(node->left), height(node->right)) + 1;
        int bal = balance(node);

        // left left
        if (bal > 1 && value < node->left->value) {
            return rotate_right(node);
        }

        // right right
        if (bal < -1 && value > node->right->value) {
            return rotate_left(node);
        }

        // left right
        if (bal > 1 && value > node->left->value) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        // right left
        if (bal < -1 && value < node->right->value) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    Node* min_node(Node* node) {
        Node* current = node;
        // keep going to the left side of the tree to find minimum value
        while (current->left) {
            current = current->left;
        }
        return current;
    }

    Node* delete_node(Node* root, int value) {
        if (!root) {
            return root;
        }

        if (value < root->value) {
            // recursively call on left side of tree if value searching for is smaller than root
            root->left = delete_node(root->left, value); 
        } else if (value > root->value) {
            // recursively call on right side of tree if value searching for is greater than root
            root->right = delete_node(root->right, value);
        } else {
            if ((!root->left) || (!root->right)) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else *root = *temp;
                delete temp;
            } else {
                Node* temp = min_node(root->right);
                root->value = temp->value;
                root->right = delete_node(root->right, temp->value);
            }
        }

        if (!root) {
            return root;
        }

        root->height = std::max(height(root->left), height(root->right)) + 1;
        int bal = balance(root);

        // left left
        if (bal > 1 && balance(root->left) >= 0) {
            return rotate_right(root);
        }

        // left right
        if (bal > 1 && balance(root->left) < 0) {
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }

        // right right
        if (bal < -1 && balance(root->right) <= 0) {
            return rotate_left(root);
        }

        // right left
        if (bal < -1 && balance(root->right) > 0) {
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }

        return root;
    }

    void pre_order(Node* node) {
        if (!node) {
            return;
        }
        std::cout << node->value << " ";
        pre_order(node->left);
        pre_order(node->right);
    }

    void in_order(Node* node) {
        if (!node) {
            return;
        }
        in_order(node->left);
        std::cout << node->value << " ";
        in_order(node->right);
    }

    void post_order(Node* node) {
        if (!node) {
            return;
        }
        post_order(node->left);
        post_order(node->right);
        std::cout << node->value << " ";
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int value) {
        root = insert(root, value);
    }

    void remove(int value) {
        root = delete_node(root, value);
    }

    void print_pre_order() {
        if (!root) {
            std::cout << "EMPTY";
        } else {
            pre_order(root);
        }
        std::cout << std::endl;
    }

    void print_in_order() {
        if (!root) {
            std::cout << "EMPTY";
        } else {
            in_order(root);
        }
        std::cout << std::endl;
    }

    void print_post_order() {
        if (!root) {
            std::cout << "EMPTY";
        } else {
            post_order(root);
        }
        std::cout << std::endl;
    }
};

int main() {
    AVLTree avlTree;
    std::string command;
    
    while (std::cin >> command) {
        if (command == "PRE" || command == "IN" || command == "POST") {
            if (command == "PRE") {
                avlTree.print_pre_order();
            } else if (command == "IN"){ 
                avlTree.print_in_order();
            } else if (command == "POST"){
                avlTree.print_post_order();
            } else {
                std::cout << "Invalid Finishing Move" << std::endl;
            }
            break;
        } else {
            char action = command[0];
            int value = std::stoi(command.substr(1));
            if (action == 'A') {
                avlTree.insert(value);
            } else if (action == 'D') {
                avlTree.remove(value);
            } else {
                std::cout << "Invalid Move" << std::endl;
            }
        }
    }

    return 0;
}