#include "Guest.h"
#include <stdexcept>


Guest::Guest(int guest_id, int room_num) : guest_id(guest_id), room_num(room_num),
last_meal_index(NOT_ATTENDED_MEAL), table(nullptr)
{
	if (guest_id <= 0 || room_num <= 0) {
		throw std::invalid_argument("invalid guest id or room num");
	}
}

int Guest::get_guest_id() const {
	return guest_id;
}

int Guest::get_last_meal() const {
	return last_meal_index;
}

const Table *Guest::get_table() const {
	return table;
}

Table *Guest::get_table() {
	return table;
}

int Guest::get_room_num() const {
	return room_num;
}

void Guest::set_last_meal(int updated_last_meal) {
	if (updated_last_meal < last_meal_index) {
		throw std::invalid_argument("invalid last meal");
	}
	last_meal_index = updated_last_meal;
}

void Guest::set_table(Table *new_table) {
	table = new_table;
}






