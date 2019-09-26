#ifndef STRUCTURES_H
#define STRUCTURES_H

#define C 0
#define Z 1
#define I 2
#define D 3
#define B 4
#define V 6
#define N 7

#include <stack>
#include <iostream>

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
	int sp;													//stack pointer
	int ps[8];											//p[NV-BDIZC]
	std::stack<int> memStack;

	CPU() {
		this->pc = 0;
		this->x = 0;
		this->y = 0;
		this->a = 0;
		this->sp = 0x0100;			//stack pointer goes from 0x0100 to 0x01ff
		for (int i = 0; i < 8; i++) {
			this->ps[i] = 0;
		}
	}
};

#endif
