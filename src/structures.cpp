#include <iostream>
#include <stack>
#include "structures.h"

const int T_S = 0x0800;
using namespace std;


HashMapTable::HashMapTable() {
  ht = new HashTableEntry*[T_S];
  for (int i = 0; i < T_S; i++)
    ht[i] = NULL;
}

int HashMapTable::HashFunc(int key) {
  return key % T_S;
}

void HashMapTable::Insert(int k, int v) {
  int hash_v = HashFunc(k);
  HashTableEntry* p = NULL;
  HashTableEntry* en = ht[hash_v];
  while (en != NULL && en->k != k) {
    p = en;
    en = en->n;
  }
  if (en == NULL) {
    en = new HashTableEntry(k, v);
    if (p == NULL) {
      ht[hash_v] = en;
    } else {
      p->n = en;
    }
  } else {
    en->v = v;
  }
}

void HashMapTable::Remove(int k) {
  int hash_v = HashFunc(k);
  HashTableEntry* en = ht[hash_v];
  HashTableEntry* p = NULL;
  if (en == NULL) {
    cout<<"No Element found at key "<<k<<endl;
    return;
  }
  while(en != NULL && en->k != k) {
    p = en;
    en = en->n;
  }
  if (en == NULL) {
    cout<<"No Element with key "<<k<<endl;
    return;
  } else {
    if (p == NULL) {
      ht[hash_v] = NULL;
    } else {
      p->n = en->n;
      delete en;
    }
    delete en;
    cout<<"Element Deleted"<<endl;
  }
}

int HashMapTable::SearchKey(int k) {
  int hash_v = HashFunc(k);
  bool flag = false;
  HashTableEntry* en = ht[hash_v];
  if (en != NULL) {
    while (en != NULL) {
      if (en->k == k) {
        flag = true;
      }
      if (flag) {
        return en->v;
      }
      en = en->n;
    }
  }
  if (!flag)
    return -1;
}

HashMapTable::~HashMapTable() {
  delete [] ht;
}
