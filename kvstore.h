#pragma once

#include "kvstore_api.h"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

// one value, one node.
// a node can span over many levels, with a dynamic list of successor: _nextV
// directions: right

struct SkipListNode{
    // level: 0 for bottom, k-1 for ceiling, k levels in total
    // assume each node has a different value
    uint64_t _key = -1;
    int _level = 0;
    std::string _val;
    std::vector<SkipListNode*> _nextV;
};

class SkipList {
private:
    int size = 0, level = 0; // level = 0~n, number of the highest level
    int maxLevel = 48;
    double p = 0.5;
    SkipListNode *header, *trailer;
    SkipListNode* find(uint64_t x, int lowest = 0) const ;
    bool toss();

public:
    SkipList();
    void insert(uint64_t key, const std::string &s);
    void deleteNode(uint64_t key);
    int search(uint64_t x) const;
    void setP(double x){p = x;}
    int getSize(){return size;}
    void printList();   // for debug

};



class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
    SkipList memtab;

public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

//	void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list) override;
};
