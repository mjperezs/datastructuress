/*Kira Cibak       U06950566
  Maria Jose Perez U77568172
  This program maintains a database that utilizes an AVL tree. We implemented many functions
  and also defined helper functions for the class methods. We created a comparator struct to use
  in the insert, search, and delete functions.*/

#include "AVL_Database.hpp"
#include <string>
#include <cmath>

struct comparator
{
    int tgKey;
    int mdVal;

    comparator(int tg, int md) : tgKey(tg), mdVal(md) {}
    bool operator()(const Record *x, const Record *y) const
    {
        int A = abs(x->value - tgKey);
        int B = abs(y->value - tgKey);

        if (A == B)
        {
            if (tgKey >= mdVal)
                return x->value < y->value;
            else
                return x->value > y->value;
        }

        return A < B;
    }
};

Record::Record(const std::string &k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record *r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr) {}

int AVLTree::height(AVLNode *node)
{
    return node ? node->height : 0; // return the height of the node, 0 if node is null
}

int AVLTree::balance(AVLNode *node)
{
    return node ? height(node->left) - height(node->right) : 0; // return the balance factor of the node, 0 if node is null
}

AVLNode *AVLTree::minValueNode(AVLNode *node)
{
    AVLNode *current = node;
    while (current->left != nullptr)
        current = current->left;

    return current;
}

AVLNode *AVLTree::deleter(AVLNode *root, const std::string &key, int value)
{
    if (root == nullptr)
        return root; // empty tree

    if (value < root->record->value)
    {
        root->left = deleter(root->left, key, value);
    }

    else if (value > root->record->value)
    {
        root->right = deleter(root->right, key, value);
    }

    else
    {
        if (root->left == nullptr || root->right == nullptr)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }

            else
            {
                *root = *temp; // Copy the contents of the non-empty child
            }

            delete temp;
        }

        else
        {
            AVLNode *temp = minValueNode(root->right);
            root->record = temp->record;
            root->right = deleter(root->right, temp->record->key, temp->record->value);
        }
    }

    if (root == nullptr)
        return root; // If the tree has only one node

    // Update height of the current node
    root->height = 1 + std::max(height(root->left), height(root->right));

    // Get the balance factor
    int balance = this->balance(root);

    // Left Left Case
    if (balance > 1 && this->balance(root->left) >= 0)
        return rotateRight(root);

    // Left Right Case
    if (balance > 1 && this->balance(root->left) < 0)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && this->balance(root->right) <= 0)
        return rotateLeft(root);

    // Right Left Case
    if (balance < -1 && this->balance(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

AVLNode *AVLTree::rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *z = x->right;

    x->right = y; // Perform rotation
    y->left = z;

    y->height = 1 + std::max(height(y->left), height(y->right)); // Update heights
    x->height = 1 + std::max(height(x->left), height(x->right));

    return x;
}

AVLNode *AVLTree::rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *z = y->left;

    y->left = x;
    x->right = z;

    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    return y;
}

AVLNode *AVLTree::InsertToTree(AVLNode *treeRoot, Record *record)
{
    if (treeRoot == nullptr)
    {
        return new AVLNode(record);
    }

    if (record->value < treeRoot->record->value)
    {
        treeRoot->left = InsertToTree(treeRoot->left, record);
    }

    else if (record->value > treeRoot->record->value)
    {
        treeRoot->right = InsertToTree(treeRoot->right, record);
    }

    else
    {
        return treeRoot;
    }

    treeRoot->height = 1 + std::max(height(treeRoot->left), height(treeRoot->right));

    int bal = balance(treeRoot);

    if (bal > 1 && record->value < treeRoot->left->record->value)
    {
        return rotateRight(treeRoot);
    }

    if (bal < -1 && record->value > treeRoot->right->record->value)
    {
        return rotateLeft(treeRoot);
    }

    if (bal > 1 && record->value > treeRoot->left->record->value)
    {
        treeRoot->left = rotateLeft(treeRoot->left);
        return rotateRight(treeRoot);
    }

    if (bal < -1 && record->value < treeRoot->right->record->value)
    {
        treeRoot->right = rotateRight(treeRoot->right);
        return rotateLeft(treeRoot);
    }

    return treeRoot;
}

void AVLTree::insert(Record *record)
{
    root = InsertToTree(root, record);
}

Record *AVLTree::search(const std::string &key, int value)
{
    AVLNode *temp = root;

    while (temp != nullptr) // Traverse the tree
    {
        if (temp->record->key == key && temp->record->value == value) // If the key and value match
        {
            return temp->record;
        }
        else if (value < temp->record->value) // If the value is less than the current node's value, go left
        {
            temp = temp->left;
        }
        else // Otherwise, go right
        {
            temp = temp->right;
        }
    }
    return new Record("", 0); // If the record is not found, return an empty record
}

void AVLTree::deleteNode(const std::string &key, int value)
{
    deleter(root, key, value);
}

void IndexedDatabase::insert(Record *record)
{
    Record *temp = index.search(record->key, record->value);
    if (temp != nullptr && temp->key != "") // If the record already exists
        return;

    index.insert(record); // Insert the record
    recCount++;           // Increment the record count
}

Record *IndexedDatabase::search(const std::string &key, int value)
{
    Record *temp = index.search(key, value); // Search for the record
    return temp;
}

void IndexedDatabase::deleteRecord(const std::string &key, int value)
{
    Record *temp = index.search(key, value); // Search for the record
    if (temp == nullptr)                     // If the record does not exist
        return;

    index.deleteNode(key, value); // Delete the record
    recCount--;                   // Decrement the record count
}

void IndexedDatabase::rqHelper(AVLNode *rt, int s, int e, std::vector<Record *> &res)
{
    if (rt == nullptr) // If tree is empty
        return;

    if (rt->record->value > s)         // If the value is greater than the start value
        rqHelper(rt->left, s, e, res); // Traverse left

    if (rt->record->value >= s && rt->record->value <= e) // If the value is within the range
        res.push_back(rt->record);                        // Add the record to the result

    if (rt->record->value < e)          // If the value is less than the end value
        rqHelper(rt->right, s, e, res); // Traverse right
}

std::vector<Record *> IndexedDatabase::rangeQuery(int start, int end)
{
    std::vector<Record *> res; // Create a vector to store the records
    rqHelper(index.getRoot(), start, end, res);
    return res;
}

void IndexedDatabase::getAllRecs(AVLNode *root, std::vector<Record *> &res)
{
    if (root == nullptr) // If the tree is empty
        return;

    getAllRecs(root->left, res);  // Traverse left
    res.push_back(root->record);  // Add the record to the result
    getAllRecs(root->right, res); // Traverse right
}

void IndexedDatabase::inorderer(AVLNode *root, std::vector<Record *> &res)
{
    if (root == nullptr) // If the tree is empty
        return;

    inorderer(root->left, res);  // Traverse left
    res.push_back(root->record); // Add the record to the result
    inorderer(root->right, res); // Traverse right
}

std::vector<Record *> IndexedDatabase::findKNearestKeys(int key, int k)
{
    Record *temp = index.search("", key);
    std::vector<Record *> res;                                                                         // Create a vector to store the records
    std::priority_queue<Record *, std::vector<Record *>, comparator> pq(comparator(key, temp->value)); // Create a priority queue

    inorderer(index.getRoot(), res); // Traverse the tree

    for (int i = 0; i < res.size(); i++) // Add the records to the priority queue
    {
        pq.push(res[i]);
        if (pq.size() > k)
            pq.pop();
    }

    std::vector<Record *> out; // Create a vector to store the output
    while (!pq.empty())        // Add the records to the output
    {
        out.push_back(pq.top());
        pq.pop();
    }

    return out;
}

std::vector<Record *> IndexedDatabase::inorderTraversal()
{
    std::vector<Record *> out;       // Create a vector to store the records
    inorderer(index.getRoot(), out); // Traverse the tree
    return out;
}

void IndexedDatabase::clearer(AVLNode *root)
{
    if (root == nullptr) // If the tree is empty
        return;

    clearer(root->left);
    clearer(root->right);

    delete root->record; // Delete the record
    delete root;         // Delete the node
}

void IndexedDatabase::clearDatabase()
{
    clearer(index.getRoot()); // Clear the database
    index.setRoot(nullptr);   // Set the root to null
    recCount = 0;             // Reset the record count
}

int IndexedDatabase::countRecords()
{
    return recCount;
}