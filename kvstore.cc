#include "kvstore.h"
#include <string>

SkipList::SkipList() {
    header = new SkipListNode;
    trailer = new SkipListNode;
    header->_nextV.push_back(trailer);
}

void SkipList::insert(uint64_t _key, const std::string& s) {
    SkipListNode* prev = find(_key, 0);
    if (prev->_key == _key){
        prev->_val = s;
        return;
    }
    SkipListNode* newNode = new SkipListNode;
    newNode->_key = _key;
    newNode->_val = s;

    int _level = 0;
    while (!_level || toss()) {
        if (_level > level) {
            ++level;
            header->_nextV.push_back(newNode);
            newNode->_nextV.push_back(trailer);
            ++header->_level;
            ++trailer->_level;
            ++_level;
        }
        else if (_level <= prev->_level) {
            newNode->_nextV.push_back(prev->_nextV[_level]);
            prev->_nextV[_level] = newNode;
            ++_level;
        }
        else {
            prev = find(_key, _level);
            newNode->_nextV.push_back(prev->_nextV[_level]);
            prev->_nextV[_level] = newNode;
            ++_level;
        }
    }
    newNode->_level = _level - 1;
}

void SkipList::deleteNode(uint64_t key) {
    SkipListNode* prev = header;
    std::vector<SkipListNode*> prevV(level + 1, nullptr); // Track previous nodes at each level
    // Find the node to be deleted and track previous nodes
    for (int i = level; i >= 0; --i) {
        while (prev->_nextV[i] != nullptr && prev->_nextV[i]->_key < key) {
            prev = prev->_nextV[i];
        }
        prevV[i] = prev;
    }
    SkipListNode* curr = prev->_nextV[0];
    if (curr != nullptr && curr->_key == key) {
        // Found the node, delete it
        for (int i = 0; i <= level; ++i) {
            if (prevV[i]->_nextV[i] == curr) {
                prevV[i]->_nextV[i] = curr->_nextV[i];
            }
        }
        delete curr;
        --size;
        // Adjust the level if necessary
        while (level > 0 && header->_nextV[level] == nullptr) {
            --level;
        }
    }
}

// return the number of comparisons
int SkipList::search(uint64_t x) const {
    int ans = 0;
    SkipListNode* tmp = find(x,0);
    if(tmp->_key == x) return ans;
    return -1;
}

bool SkipList::toss() {
    int x = rand() % 10000;
    if(x > p*10000) return true;
    return false;
}

// if x already exists, return x
// else return the probable prev node of x
SkipListNode *SkipList::find(uint64_t x, int lowest) const{
    SkipListNode *curr = header;
    int lev = level;
    while(curr!= nullptr && curr!= trailer){
        if(curr->_nextV[lev]->_key == x) return curr->_nextV[lev];
        if(curr->_nextV[lev]==trailer || curr->_nextV[lev]->_key > x){
            // search downwards
            if(lev>lowest)--lev;
            else return curr;
        }
        else if(curr->_nextV[lev]->_key < x){
            // search afterwards
            curr = curr->_nextV[lev];
        }
    }
    return curr;
}

void SkipList::printList() {
    SkipListNode* curr = header->_nextV[0];
    while (curr != nullptr && curr != trailer) {
        std::cout << "(" << curr->_key << ", " << curr->_val << ", " << curr->_level << ") ";
        curr = curr->_nextV[0];
    }
    std::cout << std::endl;
}


KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    if(get(key) == s) return;
    memtab.insert(key, s);
    if(memtab.getSize()>= 2 * 1024 * 1024){
        // flush memtab
        // TODO
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    // TODO
	return "";
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	// find in skiplist
    // if found, delete it
    // else insert (key, "_DELETED_") and return false
    if(memtab.search(key) != -1){ // found
        memtab.deleteNode(key);
        return true;
    }
    else {
        memtab.insert(key, "_DELETED_");
        return false;
    }
    return false;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    //TODO
}


///**
// * Return a list including all the key-value pair between key1 and key2.
// * keys in the list should be in an ascending order.
// * An empty string indicates not found.
// */
//void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list)
//{
//
//}