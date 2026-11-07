// You can edit anything you want in this file.
// However you need to implement all public StudentCourseManager function, as provided below as a template

#include "SegmentationResort26b1.h"
#include <stdexcept>

SegmentationResort::SegmentationResort() : current_meal(1){}

SegmentationResort::~SegmentationResort()
{
    guests_tree.applyToValues(deleteGuest);
    tables_tree.applyToValues(deleteTable);
}

StatusType SegmentationResort::checkIn(int guestId, int roomNum)
{
    if (guestId <= 0 || roomNum <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if(guests_tree.contains(guestId) || rooms_tree.contains(roomNum)) {
        return StatusType::FAILURE;
    }

    Guest *new_guest = nullptr;

    try {
        new_guest = new Guest(guestId, roomNum);
        rooms_tree.insert(roomNum, guestId);
        guests_tree.insert(guestId, new_guest);
    } catch (const std::bad_alloc&) {
        rooms_tree.remove(roomNum);
        guests_tree.remove(guestId);
        delete new_guest;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::checkOut(int guestId)
{
    if (guestId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Guest **guest_ptr_ptr = guests_tree.find(guestId);
    if (!guest_ptr_ptr) {
        return StatusType::FAILURE;
    }
    Guest *to_checkOut = *guest_ptr_ptr;
    if(!to_checkOut || to_checkOut->get_table()) {
        return StatusType::FAILURE;
    }
    rooms_tree.remove(to_checkOut->get_room_num());
    guests_tree.remove(guestId);
    delete to_checkOut;
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::addTable(int tableId, int capacity)
{
    if (tableId <= 0 || capacity <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (tables_tree.contains(tableId)) {
        return StatusType::FAILURE;
    }

    Table *new_table = nullptr;
    try {
        new_table = new Table(tableId, capacity);
        tables_tree.insert(tableId, new_table);
    } catch (const std::bad_alloc&) {
        tables_tree.remove(tableId);
        delete new_table;
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::removeTable(int tableId)
{
    if (tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!tables_tree.contains(tableId)) {
        return StatusType::FAILURE;
    }

    Table *to_remove = *tables_tree.find(tableId);
    if(to_remove->get_guests_count() > 0) {
        return StatusType::FAILURE;
    }

    tables_tree.remove(tableId);
    delete to_remove;
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::enterDiningRoom(int guestId, int tableId) {
    if (guestId <= 0 || tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Guest **guest_ptr_ptr = guests_tree.find(guestId);
    Table **table_ptr_ptr = tables_tree.find(tableId);
    if (!guest_ptr_ptr || !table_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Guest *guest = *guest_ptr_ptr;
    Table *table = *table_ptr_ptr;

    if(guest->get_table() != nullptr
        || guest->get_last_meal() >= current_meal) {
        return StatusType::FAILURE;
    }

    StatusType status;

    try {
        status = table->add_guest(guest);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (status == StatusType::SUCCESS) {
        guest->set_last_meal(current_meal);
    }
    return status;
}

StatusType SegmentationResort::leaveDiningRoom(int guestId, int tableId) {
    if (guestId <= 0 || tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Table **table_ptr_ptr = tables_tree.find(tableId);
    if (!table_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Table *table = *table_ptr_ptr;

    Guest **guest_ptr_ptr = table->get_guests_tree().find(guestId);
    if(!guest_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Guest *guest = *guest_ptr_ptr;

    StatusType status = table->remove_guest(guest);
    if (status == StatusType::SUCCESS) {
        guest->set_last_meal(current_meal);
    }

    return status;
}

StatusType SegmentationResort::reheatFood() {
    current_meal++;
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::joinTables(int tableId1, int tableId2){
    if (tableId1 <= 0 || tableId2 <= 0 || tableId1 == tableId2) {
        return StatusType::INVALID_INPUT;
    }

    Table **t1_ptr_ptr = tables_tree.find(tableId1);
    if(!t1_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Table **t2_ptr_ptr = tables_tree.find(tableId2);
    if(!t2_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Table *table1 = *t1_ptr_ptr;
    Table *table2 = *t2_ptr_ptr;
    try {
        table1->joinTables(table2);
        tables_tree.remove(tableId2);
        delete table2;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;

}

output_t<int> SegmentationResort::joinFriend(int guestId1, int guestId2){
    if (guestId1 <= 0 || guestId2 <= 0 || guestId1 == guestId2) {
        return StatusType::INVALID_INPUT;
    }

    Guest **guest1_ptr_ptr = guests_tree.find(guestId1);
    if (!guest1_ptr_ptr) {
        return StatusType::FAILURE;
    }

    Guest *guest1 = *guest1_ptr_ptr;
    if(guest1->get_table() || guest1->get_last_meal() == current_meal) {
        return StatusType::FAILURE;
    }

    Guest **guest2_ptr_ptr = guests_tree.find(guestId2);
    if (!guest2_ptr_ptr) {
        return StatusType::FAILURE;
    }
    Guest *guest2 = *guest2_ptr_ptr;
    Table *current_table = guest2->get_table();
    if (!current_table || current_table->get_guests_count()>=current_table->get_capacity()) {
        return StatusType::FAILURE;
    }


    StatusType status;

    try {
        status = current_table->add_guest(guest1);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (status == StatusType::SUCCESS) {
        guest1->set_last_meal(current_meal);
        return current_table->get_table_id();
    }
    return StatusType::FAILURE;

}

output_t<int> SegmentationResort::cleanNextRoom()
{
    if(rooms_tree.empty()) {
        return StatusType::FAILURE;
    }
    const int *room_ptr = rooms_tree.goToNextNode();
    if (room_ptr) {
       return *room_ptr;
    }
    return StatusType::FAILURE;

}