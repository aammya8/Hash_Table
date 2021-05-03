/*
  Project Description: Student database program using hash table. User can add, print, and delete Students. Students can be inputted manually or using random student generator.
  Name: Aammya Sapra
  Date: April 29, 2021
*/

#include <iostream>
#include <cstring>
#include <string>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <vector>
#include "Student.h"
#include "Generator.h"

using namespace std;


//function prototypes
void add_student(Student** hash_table, int num_slots, Student* s, Generator* g);
bool id_exists(Student* s, vector<int> id_numbers);
void print_students(Student** hash_table, int num_slots);
void delete_student(Student** hash_table, int num_slots, int id);
void rehash(Student** &hash_table, int &num_slots, Generator* g);



//------------------------------------------------------MAIN FUNCTION----------------------------------------------------------

int main() {

	//print out instructions
	cout << '\n' << "To add a student type \'ADD\'." << endl;
	cout << "To print current students type \'PRINT\'." << endl;
	cout << "To delete a student type \'DELETE\'." << endl;
	cout << "To quit type \'QUIT\'." << endl << endl;


	bool inputting = true;
	char command[10];  //stores the command (ADD, PRINT, DELETE, QUIT) inputted by the user

	int num_slots = 100; //hash table size
	Student** hash_table = new Student* [num_slots];
	int key;

	vector<int> id_numbers; //stores all id numbers; will be used in student_exists()

	Generator* generator = new Generator();
	

	while (inputting) {
		cin >> command;

		//ADD NEW STUDENT(S)------------------------------------
		if (strcmp(command, "ADD") == 0) {

			string option; //stores option (manual/random) chosen by user
			bool exists; //stores whether or not student with given ID number already exists

			cout << "To add a new student manually, enter \'M\'." << endl;
			cout << "To add a new Student using the random student generator, enter \'R\'." << endl;
			cin >> option; //store response

			//manually input Student data
			if (option == "M" || option == "m") {
				//create new Student pointer
				Student *st = new Student();

				//prompt user to enter information
				cout << "First name: ";
				cin >> st->first_name;
				cout << "Last name: ";
				cin >> st->last_name;
				cout << "ID number: ";
				cin >> st->id;
				cout << "GPA: ";
				cin >> st->gpa;

				//check to make sure this ID number does not already exist
				exists = id_exists(st, id_numbers);
				if (exists == false) {
					add_student(hash_table, num_slots, st, generator);
				}
				else {
					cout << "A student with this ID number already exists. Please check that you have entered the ID correctly." << endl;
				}
			} 

			//randomly generate student(s)
			else if (option == "R" || option == "r") {

				int num_to_add; //stores number of students user wants to randomly generate
				cout << "How many students would you like to generate and add? Please enter a number." << endl;
				cin >> num_to_add;

				while (num_to_add > 0) {
					add_student(hash_table, num_slots, generator->generate_student(id_numbers), generator);
					num_to_add--;
				}
				cout << "Students added." << endl;
			}

			cout << "\nEnter \'ADD\', \'PRINT\', \'DELETE\' to continue editing or \'QUIT\' to exit." << endl << endl;
		} 

		//PRINT STUDENT(S)-------------------------------------
		else if (strcmp(command, "PRINT") == 0) {
			print_students(hash_table, num_slots);
			cout << "\nEnter \'ADD\', \'PRINT\', \'DELETE\' to continue editing or \'QUIT\' to exit." << endl << endl;
		}

		//DELETE STUDENT--------------------------------------
		else if (strcmp(command, "DELETE") == 0) {
			int i;
			cout << "Enter student's ID number: ";
			cin >> i;
			delete_student(hash_table, num_slots, i);
			cout << "\nEnter \'ADD\', \'PRINT\', \'DELETE\' to continue editing or \'QUIT\' to exit." << endl << endl;
		}

		//QUIT------------------------------------------------
		else if (strcmp(command, "QUIT") == 0) {
			inputting = false;
			break;
		}

		//INVALID INPUT---------------------------------------
		else { //prompt user to re-enter command if something invalid was entered
			cout << "\nPlease enter either \'ADD\', \'PRINT\', \'DELETE\', or \'QUIT\'." << endl;
		}
	} //end of while loop 

	return 0;
} //end of main




//--------------------------------------------------------FUNCTIONS------------------------------------------------------------



//function to add new student
//if more than 3 collisions when chaining, create table to have double the number of slots and rehash students into new table
void add_student(Student** hash_table, int num_slots, Student* s, Generator* g) {
	int collisions = 0;
	int hash_index = g->hash_function(s->first_name, num_slots);
	Student* current = hash_table[hash_index];

	if (current == NULL) { //if index is empty
		hash_table[hash_index] = s;
	}
	else {
		collisions++;
		while (current->next != NULL) { //if slot is not empty, chain to end
			collisions++;
			current = current->next;
		} //end of linked list found
		current->next = s; //add new Student s to end
		s->next = NULL;
		collisions++;

		//when collisions = 3 (3 students at index), rehash
		if (collisions == 3) {
			rehash(hash_table, num_slots, g);
		}
	}

	return;
}



//function to make sure ID number is not already taken
bool id_exists(Student* s, vector<int> id_numbers) {
	for (vector<int>::iterator it = id_numbers.begin(); it < id_numbers.end(); it++) {
		if (*it == s->id) {
			return true; //ID already taken
		}
	}
	return false; //ID available
}



//function to print out all the students currently stored
void print_students(Student** hash_table, int num_slots) {
	cout << "Students will be printed in the following format: first_name last_name, student_id, GPA" << endl;
	
	for (int i = 0; i < num_slots; i++) { //iterate through hash table
		Student* current = hash_table[i];
		while (current != NULL) { //print out student(s) at hash index
			//cout << "Index: " << i << endl;
			cout << current->first_name << ' ' << current->last_name << ", " << current->id << ", " << fixed << setprecision(2) << current->gpa << endl;
			current = current->next; //move to next student in linked list (if chaining)
		}
	}
	return;
}



//function to delete a student 
void delete_student(Student** hash_table, int num_slots, int id) {
	Student* current = NULL;
	Student* prev = NULL;

	for (int i = 0; i < num_slots; i++) { //iterate through hash table
		if (hash_table[i] != NULL) {
			current = hash_table[i];
			while (current != NULL) { //iterate through linked list
				if (current->id == id) { //ID match found
					if (prev != NULL) { //Node is not head of linked list
						prev->next = current->next;
						delete current;
						current = NULL;
					} else { //Node is head of linked list
						hash_table[i] = current->next;
						delete current;
						current = NULL;
					}
				}
				else { //ID match not found
					prev = current;
					current = current->next;
				}
			}
		}
		//reset for next hash index
		current = NULL;
		prev = NULL;
	}

	return;
}



//function to rehash table 
//create table to have double the number of slots and rehash students into new table
void rehash(Student** &hash_table, int &num_slots, Generator* g) {

	//create temporary copy of hash table
	Student** temp = new Student* [num_slots];
	for (int i = 0; i < num_slots; i++) {
		temp[i] = hash_table[i];
	}

	//double the size
	num_slots = num_slots * 2;

	//reset hash table to NULL
	hash_table = new Student* [num_slots];
	for (int i = 0; i < num_slots; i++) {
		hash_table[i] = NULL;
	}

	//copy information into new table
	for (int i = 0; i < (num_slots/2); i++) {
		if (temp[i] != NULL) {
			add_student(hash_table, num_slots, temp[i], g);
		}
	}

	//delete copy of original table
	delete[] temp; 

	return;
}