# Hotel Management System (Segmentation Resort)

This project provides a C++ implementation of a Hotel Management System developed for the Data Structures 1 course (234218) at the Technion. The system efficiently tracks hotel guest check-ins, check-outs, room assignments, and dining room seating arrangements under strict time complexity constraints.

## Data Structures & Core Architecture

The core of the system relies on generic **AVL Trees** to manage guests, rooms, and dining tables efficiently:

* `guests_tree`: Maps guest IDs to guest records.  
* `tables_tree`: Maps table IDs to active dining room table records.  
* `rooms_tree`: Maps room numbers to registered guest IDs.

## Supported Operations & Complexity

* **`checkIn` / `checkOut`**: Registers new guests or processes guest departures in O(log n) time, where n is the number of active guests in the hotel.  
* **`addTable` / `removeTable`**: Manages active dining room tables in O(log m) time, where m is the number of active dining room tables.  
* **`enterDiningRoom`**: Assigns a guest to a specific dining table in O(log n \+ log m) time.  
* **`leaveDiningRoom`**: Removes a guest from a specific dining table in O(log m \+ log k) time, where k is the number of guests seated at that specific table.  
* **`reheatFood`**: Advances the meal index in O(1) time to manage dining room entry permissions efficiently, ensuring guests can only eat once per meal cycle.  
* **`joinTables`**: Merges two tables into one by extracting sorted arrays from both tables' AVL trees and constructing a new balanced tree in O(log m \+ k1 \+ k2) time, where k1 is the number of guests seated at the first table and k2 is the number of guests seated at the second table.  
* **`cleanNextRoom`**: Returns the next occupied room to be cleaned in O(1) time using in-order successor pointers maintained within the rooms AVL tree. 

