#ifndef STRUCTURES_H
#define STRUCTURES_H

struct HashTableEntry {
  int v, k;
  HashTableEntry *n;
  HashTableEntry *p;
  HashTableEntry(int k, int v);
};
class HashMapTable {
	HashTableEntry **ht, **top;
	public:
	HashMapTable();
		int HashFunc(int key);
		void Insert(int k, int v);
		void Remove(int k);
		void SearchKey(int k);
		~HashMapTable();
};

#endif