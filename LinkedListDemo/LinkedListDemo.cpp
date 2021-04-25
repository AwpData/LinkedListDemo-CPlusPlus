// Trevor Tang 4/25/21 PROG 113

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Student {
public:
	string firstname;
	string lastname;
	string major;
	string home;
	string year;

	Student() {
		firstname = "Not a real person...";
	}

	Student(string line) {
		int startPos = 0;
		int commaPos = line.find(',', startPos);
		firstname = line.substr(startPos, commaPos - startPos);
		startPos = commaPos + 1;

		commaPos = line.find(',', startPos);
		lastname = line.substr(startPos, commaPos - startPos);
		startPos = commaPos + 1;

		commaPos = line.find(',', startPos);
		major = line.substr(startPos, commaPos - startPos);
		startPos = commaPos + 1;

		commaPos = line.find(',', startPos);
		home = line.substr(startPos, commaPos - startPos);
		startPos = commaPos + 1;

		commaPos = line.find("\n", startPos);
		year = line.substr(startPos, commaPos - startPos);
	}

	void printStudent() {
		cout << firstname << " " << lastname << " " << major << " " << home << " " << year << endl;
	}
};

struct Listnode { // List node for our Linked List class 
	Student value;
	Listnode* next;
	Listnode(Student value, Listnode* next = nullptr) {
		this->value = value;
		this->next = next;
	}
};

class LinkedList {
public:
	Listnode* head; // front of the list
	Listnode* tail; // end of the list 

	LinkedList() {
		this->head = nullptr;
		this->tail = nullptr;
	}

	void add(Student value) { // Adds student node to the end of the LinkedList
		if (head == nullptr) { // This brings our linked list into existence, with the first added value being the head (or front) node 
			head = new Listnode(value);
			tail = head; // end of list (tail) is set to front of list (head) 
		}
		else {
			// I set current to tail and not to the head because then I won't have to loop through the linked list for time efficiency
			Listnode* current = tail; // current is our traversal pointer to not modify head pointers (which points to the actual linkedlist) 
			current->next = new Listnode(value); // If the current->next is a nullptr, then it makes sense to set that next value to be our new listnode 
			tail = current->next; // sets tail to the end of the list (current.next points to it) so adding the next node is faster 
		}
	}

	void displayList() { // Displays the student list from first to last node and deletes each student afterwards
		Listnode* current = head; // have to create a new pointer when traversing (because we can't reset where head is pointing!) 

		cout << "CURRENT STUDENT LIST" << endl;
		cout << "----------------------" << endl;

		while (current != nullptr) { // goes through the linked list and prints every value until it is nullptr 
			current->value.printStudent();
			current = current->next;
		}
		cout << "\n";
	}

	void remove(string firstname) { // Removes desired node based on student's first name 
		Listnode* current;
		Listnode* previous;
		if (!head) { // checks for empty list 
			cout << "Failure to remove: no one is in the list!" << endl;
			return;
		}
		if (head->value.firstname == firstname) { // checks if first node has our value 
			current = head; // Sets the temp pointer to head, changes head to the next value, and deletes the temp (and gets rid of the first value) 
			cout << "Deleting " << current->value.firstname << endl;
			head = head->next; // moves head over one to make it the new front of list
			if (head == NULL) { // This is to check if the front value was the ONLY value in the list; since we removed it, we can set head/tail to nullptr to refill the heap
				cout << "List is now empty!" << endl;
				head = nullptr;
				tail = nullptr;
			}
			delete current; // deletes the value 
		}
		else { // If the desired removed value is not the front node 
			current = head;
			previous = current;
			while (current != nullptr && current->value.firstname != firstname) { // Continuously loops until we get the desired value or hit a nullptr 
				previous = current; // I need the previous node so I can "jump over it" when deleting (or be a restore point for the tail if I delete the last node)
				current = current->next;
			}
			if (current) { // if current is NOT null 
				cout << "Deleting " << current->value.firstname << endl;
				if (tail->value.firstname == current->value.firstname) { // This checks if the last node was the deleted value 
					tail = previous; // Sets the tail (which points to the last node) to the previous node 
					delete current; // Deletes the last node
					tail->next = nullptr; // Sets the next last value to nullptr since it doesn't exist anymore 
				}
				else { // This checks if our deleted value is in-between the front and last node 
					previous->next = current->next; // First set the previous node's next to the deleted nodes next value ("Jump over") 
					delete current; // deletes the desired values
				}
			}
			else { // If the entered users name is invalid 
				cout << "Failure to remove: could not find " << firstname << endl;
			}
		}
		cout << "\n";
	}
};

int main()
{
	ifstream inputFile;
	inputFile.open("studentData.csv");
	Student* student;
	string input;
	LinkedList studentList;

	getline(inputFile, input); // Goes to the next line (The first line is the header) 
	while (getline(inputFile, input)) {
		student = new Student(input); // creates a new student by pointer reference 
		studentList.add(*student); // adds the student (dereferenced) to our linked list 
		cout << "Adding student " << student->firstname << endl; // DEBUG: says what student we just added 

		delete student; // delete that student so I can free up memory
		student = nullptr;
	}

	// LINKED LIST: [Kelsie, Cori, Lacey, Brett, Joesph, Bryson]
	cout << "\n";
	studentList.displayList(); // Displays (and consecutively deletes students in no sequential order) student linked list 

	studentList.remove("Bryson"); // POST: [Kelsie, Cori, Lacey, Brett, Joesph]
	studentList.displayList();
	studentList.remove("Joesph"); // POST: [Kelsie, Cori, Lacey, Brett]
	studentList.displayList();
	studentList.remove("Kelsie"); // POST: [Cori, Lacey, Brett]
	studentList.displayList();
	studentList.remove("Kelsie"); // POST: should say there is no student Kelsie 
	studentList.displayList();
	studentList.remove("Lacey"); // POST: [Cori, Brett]
	studentList.displayList();
	studentList.remove("Brett"); // POST: [Cori]
	studentList.displayList();
	studentList.remove("Cori"); // POST: Non-existent Linked List due to nullptr 
	studentList.displayList();
	studentList.remove("test"); // List should be empty 
	inputFile.close();
}