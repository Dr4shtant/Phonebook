#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a node in the AVL tree
struct Node {
    char name[100];
    char number[13];
    struct Node* left;
    struct Node* right;
    int height;
};

// Function to calculate the height of a node
int height(struct Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to calculate the maximum of two integers
int Max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to create a new node with the given values
struct Node* newNode(char name[], char number[]) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    strcpy(node->name, name);
    strcpy(node->number, number);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Function to perform a right rotation
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = Max(height(y->left), height(y->right)) + 1;
    x->height = Max(height(x->left), height(x->right)) + 1;

    return x;
}

// Function to perform a left rotation
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = Max(height(x->left), height(x->right)) + 1;
    y->height = Max(height(y->left), height(y->right)) + 1;

    return y;
}

// Function to get the balance factor of a node
int getBalance(struct Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Function to insert a node into the AVL tree
struct Node* insert(struct Node* node, char name[], char number[]) {
    // Perform the normal BST insertion
    if (node == NULL)
        return newNode(name, number);

    int cmp = strcmp(name, node->name);
    if (cmp < 0)
        node->left = insert(node->left, name, number);
    else if (cmp > 0)
        node->right = insert(node->right, name, number);
    else // Duplicate names are not allowed
        return node;

    // Update the height of the current node
    node->height = 1 + Max(height(node->left), height(node->right));

    // Check the balance factor and perform rotations if necessary
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && strcmp(name, node->left->name) < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && strcmp(name, node->right->name) > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(name, node->left->name) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(name, node->right->name) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Function to print the AVL tree in inorder traversal
void inorder(struct Node* node) {
    if (node != NULL) {
        inorder(node->left);
        printf("Name: %s\nNumber: %s\n", node->name, node->number);
        printf("--------------------------------------------------------------\n");
        inorder(node->right);
    }
}

void searchNumber(struct Node* node, char name[]) {
    if (node == NULL)
        printf("Contact does not exist\n"); // Name not found

    int cmp = strcmp(name, node->name);
    if (cmp < 0)
        return searchNumber(node->left, name);
    else if (cmp > 0)
        return searchNumber(node->right, name);
    else
        printf("Number: %s\n", node->number); // Name found
}

struct Node* findMin(struct Node* node) {
    if (node == NULL) {
        return NULL;
    }
    
    while (node->left != NULL) {
        node = node->left;
    }
    
    return node;
}


struct Node* deleteNodeByName(struct Node* root, char* name) {
    if (root == NULL) {
        return root;

    }
    
    // Find the node to delete
    int compare = strcmp(name, root->name);
    if (compare < 0) {
        root->left = deleteNodeByName(root->left, name);
    } else if (compare > 0) {
        root->right = deleteNodeByName(root->right, name);
    } else {
        // Node to delete found
        
        // Case 1: Node has no children
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            struct Node* temp = root;
            root = root->right;
            free(temp);
        } else if (root->right == NULL) {
            struct Node* temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Node has two children
        else {
            struct Node* temp = findMin(root->right);
            strcpy(root->name, temp->name);
            strcpy(root->number, temp->number);
            root->right = deleteNodeByName(root->right, temp->name);
        }
    }
    
    return root;
}


void deleteAVLTree(struct Node* root) {
    if (root == NULL) {
        return;
    }
    
    deleteAVLTree(root->left);
    deleteAVLTree(root->right);
    
    free(root);
}

// Driver program to test the AVL tree implementation
int main() {
    struct Node* root = NULL;
    int choice;
    char name[100],number[13];
    while (1) {
        printf("1.New Contact\t2.Display\t3.Search\t4.Delete\t5.Quit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                getchar();
                printf("Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printf("Number: ");
                fgets(number, sizeof(number), stdin);
                number[strcspn(number, "\n")] = '\0';
                root=insert(root,name,number);
                break;
            case 2:
                if(root!=NULL) {
                    printf("\033[4mContacts.\033[0m\n");
                    inorder(root);
                }
                else
                    printf("Phonebook is empty. Please add Contacts.\n");
                break;
            case 3:
                if(root!=NULL) {
                    getchar();
                    printf("Enter the name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    searchNumber(root, name);
                }
                else {
                    printf("Phonebook is empty. Please add Contacts.\n");
                }
                break;
            case 4:
                if(root!=NULL) {
                    getchar();
                    printf("Enter the name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    deleteNodeByName(root,name);
                }
                else {
                    printf("Phonebook is empty. Please add Contacts.\n");
                }
                break;
            case 5:
                printf("Exitting now.");
                deleteAVLTree(root);
                exit(0);
        }
    }
    return 0;
}