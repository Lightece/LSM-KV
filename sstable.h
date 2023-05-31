//
// Created by zyh on 23/5/24.
//

#ifndef LSMKV_SSTABLE_H
#define LSMKV_SSTABLE_H

#include <cstdint>
#include <cstring>
#include <bitset>
#include <vector>

typedef std::pair<uint64_t, unsigned int> index_t;


/*
 * Bloom Filter: 用于判断键是否可能存在于SSTable中
 */
class BloomFilter {
private:
    static constexpr uint32_t kFilterSize = 10240;  // Bloom Filter大小，单位为字节
    std::bitset<kFilterSize> filter;

    uint32_t HashFunction1(uint64_t key);
    uint32_t HashFunction2(uint64_t key);
    uint32_t HashFunction3(uint64_t key);
    uint32_t HashFunction4(uint64_t key);


public:
    BloomFilter() {
        // 初始化Bloom Filter为空
        filter.reset();
    }

    // 插入键到Bloom Filter
    void Insert(uint64_t key);

    // 判断键是否可能存在于Bloom Filter中
    bool MayContain(uint64_t key);
};


/*
 * cache: 用于存储SSTable的元数据
 */
class cache {
private:
    // header
    uint64_t timestamp;
    uint64_t key_count;
    uint64_t key_min = -1, key_max = 0;
    // bloom filter
    BloomFilter bloom_filter;
    // index block: key, offset
    std::vector<index_t> index;
public:
    cache() = default;
    ~cache() = default;

};


#endif //LSMKV_SSTABLE_H
