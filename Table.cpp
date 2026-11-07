#include "Table.h"

#include "Guest.h"
#include "wet1util.h"

#include <stdexcept>


Table::Table(int id, int max_guests) : table_id(id),
capacity(max_guests),guests_count(0){
	if (id <=0 || max_guests <= 0) {
		throw std::invalid_argument("invalid id/capacity");
	}
}

int Table::get_table_id() const {
	return table_id;
}

int Table::get_capacity() const {
	return capacity;
}

int Table::get_guests_count() const {
	return guests_count;
}

AVLTree<int, Guest *> &Table::get_guests_tree() {
	return guests_tree;
}

const AVLTree<int, Guest *> &Table::get_guests_tree() const {
	return guests_tree;
}



StatusType Table::add_guest(Guest* guest_to_add) {
	if (get_guests_count() >= get_capacity()) {
		return StatusType::FAILURE;
	}
	if(guests_tree.insert(guest_to_add->get_guest_id(), guest_to_add)) {
		guests_count++;
		guest_to_add->set_table(this);
		return StatusType::SUCCESS;
	}
	return StatusType::FAILURE;
	
}

StatusType Table::remove_guest(Guest* guest_to_remove) {
	if (guests_tree.remove(guest_to_remove->get_guest_id())) {
		guests_count--;
		guest_to_remove->set_table(nullptr);
		return StatusType::SUCCESS;
	}
	return StatusType::FAILURE;
}

StatusType Table::joinTables(Table *other) {
	if (!other || this == other) {
		return StatusType::FAILURE;
	}

	int other_count = other->get_guests_count();
	Guest** other_guests = nullptr;

	if (other_count > 0) {
		other_guests = other->get_guests_tree().getValuesArray();
	}

	try {
		this->guests_tree.mergeWith(other->get_guests_tree());
	} catch (std::bad_alloc&) {
		delete[] other_guests;
		throw;
	}

	this->capacity += other->get_capacity();
	this->guests_count += other_count;

	if (other_guests) {
		for (int i = 0; i < other_count; ++i) {
			other_guests[i]->set_table(this);
		}
		delete[] other_guests;
	}

	return StatusType::SUCCESS;
}












