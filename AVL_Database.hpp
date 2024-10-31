/**/

#ifndef AVL_DATABASE_HPP
#define AVL_DATABASE_HPP

#include <string>
#include <vector>
#include <queue>

class Record { 
public:
    std::string key;
    int value;

    Record(const std::string& k, int v);

    bool operator==(const Record& other) const {
        return key == other.key && value == other.value;
    }
};

class AVLNode {
public:
    Record* record;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Record* r);
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node);
    int balance(AVLNode* node);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* InsertToTree(AVLNode* treeRoot, Record* record);
    AVLNode* deleter(AVLNode* root, const std::string& key, int value);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);


public:
    AVLTree();
    void insert(Record* record);
    Record* search(const std::string& key, int value);
    void deleteNode(const std::string& key, int value);
    AVLNode* getRoot() const {return root;}
    void setRoot(AVLNode* newr) {root = newr;}

};

class IndexedDatabase {
private:
    AVLTree index;
    int recCount;
    void getAllRecs(AVLNode* root, std::vector<Record*>& res);
    void inorderer(AVLNode* root, std::vector<Record*>& res);
    void clearer(AVLNode* root);

public:
    void insert(Record* record);
    Record* search(const std::string& key, int value);
    void deleteRecord(const std::string& key, int value);
    std::vector<Record*> rangeQuery(int start, int end);
    std::vector<Record*> findKNearestKeys(int key, int k);
    std::vector<Record*> inorderTraversal();
    void clearDatabase();
    int countRecords();
    void rqHelper(AVLNode* root, int s, int e, std::vector<Record*>& res);
};
#endif // AVL_DATABASE_HPP