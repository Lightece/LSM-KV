//
// Created by zyh on 23/5/24.
//

#include "sstable.h"
#include <cstdint>
#include <string>

// hash function 1：MurmurHash
uint32_t BloomFilter::HashFunction1(uint64_t key) {
    uint32_t seed = 0;
    uint32_t m = 0x5bd1e995;
    uint32_t r = 24;
    uint32_t h1 = static_cast<uint32_t>(key) ^ static_cast<uint32_t>(key >> 32);
    h1 *= m;
    h1 ^= h1 >> r;
    h1 *= m;
    return h1;
}

// hash function 2：xxHash
uint32_t BloomFilter::HashFunction2(uint64_t key) {
    uint32_t seed = 0;
    uint64_t prime1 = 11400714785074694791ULL;
    uint64_t prime2 = 14029467366897019727ULL;
    uint64_t prime3 = 1609587929392839161ULL;
    uint64_t prime4 = 9650029242287828579ULL;
    uint64_t prime5 = 2870177450012600261ULL;
    uint64_t hash = seed + prime5;
    hash += prime1 * (key & 0xFFFFFFFFULL);
    hash = ((hash << 23) | (hash >> 41)) * prime2 + prime3;
    hash ^= (hash >> 29) * prime4;
    hash *= prime1;
    hash ^= (hash >> 32);

    return static_cast<uint32_t>(hash);

}

// hash function 3：BKDRHash
uint32_t BloomFilter::HashFunction3(uint64_t key) {
    uint32_t seed = 131;
    uint32_t hash = 0;
    uint8_t* keyPtr = reinterpret_cast<uint8_t*>(&key);
    for (size_t i = 0; i < sizeof(key); i++) {
        hash = (hash * seed) + keyPtr[i];
    }
    return hash;
}

// hash function 4：SDBMHash
uint32_t BloomFilter::HashFunction4(uint64_t key) {
    uint32_t hash = 0;
    uint8_t* keyPtr = reinterpret_cast<uint8_t*>(&key);
    for (size_t i = 0; i < sizeof(key); i++) {
        hash = keyPtr[i] + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}


void BloomFilter::Insert(uint64_t key){
    uint32_t h1 = HashFunction1(key);
    uint32_t h2 = HashFunction2(key);
    uint32_t h3 = HashFunction3(key);
    uint32_t h4 = HashFunction4(key);
    filter.set(h1);
    filter.set(h2);
    filter.set(h3);
    filter.set(h4);
}

bool BloomFilter::MayContain(uint64_t key) {
    uint32_t h1 = HashFunction1(key);
    uint32_t h2 = HashFunction2(key);
    uint32_t h3 = HashFunction3(key);
    uint32_t h4 = HashFunction4(key);
    return filter.test(h1) && filter.test(h2) && filter.test(h3) && filter.test(h4);
}
