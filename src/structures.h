#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stack>
#include <iostream>

#define N  0
#define V  1
#define B  2
#define D  3
#define I  4
#define Z  5
#define C  6

struct HashTableEntry {
	int v, k;
	HashTableEntry *n;
  	HashTableEntry *p;
	HashTableEntry(int k, int v) {
		this->k = k;
		this->v = v;
		this->n = NULL;
	}
};

class HashMapTable {
	HashTableEntry **ht, **top;
	public:
		HashMapTable();
		int HashFunc(int key);
		void Insert(int k, int v);
		void Remove(int k);
		int SearchKey(int k);
		~HashMapTable();
};

struct CPU {
	int pc;													//program counter
	char x;			
	char y;
	char a;
	char sp;													//stack pointer
	int ps[7];											//p[NV-BDIZC]
	std::stack<int> memStack;
	CPU() {
		this->pc = 0;
		this->x = 0;
		this->y = 0;
		this->a = 0;
		this->sp = 0;
		for (int i = 0; i < 7; i++) {
			this->ps[i] = 0;
		}
	}
};

#endif