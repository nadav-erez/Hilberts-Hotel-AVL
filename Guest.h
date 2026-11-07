#ifndef GUEST_H
#define GUEST_H

class Table;

class Guest {
private:
	const int guest_id;
	const int room_num;
	int last_meal_index;
	Table* table;

public:
	Guest(int guest_id, int room_num);

	Guest(const Guest&) = delete;
	Guest& operator=(const Guest&) = delete;

	int get_guest_id() const;
	int get_last_meal() const;
	int get_room_num() const;
	const Table* get_table() const;
	Table* get_table();

	void set_last_meal(int updated_last_meal);
	void set_table(Table* new_table);

	static const int NOT_ATTENDED_MEAL = -1;

};


#endif //GUEST_H
