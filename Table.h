#ifndef TABLE_H
#define TABLE_H
#include "AVL.h"

enum struct StatusType;
class Guest;

class Table {
private:
	const int table_id;
	int capacity;
	int guests_count;
	AVLTree<int, Guest*> guests_tree;

public:
	Table(int id, int max_guests);

	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;

	int get_table_id() const;
	int get_capacity() const;
	int get_guests_count() const;
	const AVLTree<int, Guest*>& get_guests_tree() const;
	AVLTree<int, Guest*>& get_guests_tree();

	StatusType add_guest(Guest*);
	StatusType remove_guest(Guest*);

	StatusType joinTables(Table* other);

};



#endif //TABLE_H
