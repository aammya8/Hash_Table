//cpp file

#include "Generator.h"
#include "Student.h"
#include <cstring>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>


Generator::Generator() {
	read_first_names_file();
	read_last_names_file();
}


//reads file contents of "first_names.txt" and stores in first_names vector
void Generator::read_first_names_file() {
	FILE* pfile = NULL; //file pointer
	char line[50]; //holds contents of single line in "first_names.txt" (one name per line)
	pfile = fopen("first_names.txt", "r"); //read file

	while (fgets(line, 50, pfile)) { //read line by line (50 characters per line)
		char* pch; //char pointer
		pch = strchr(line, '\n');

		if (pch != NULL) { //add null terminating character at the end of each line
			*pch = '\0'; //dereference and assign value
		}

		char* firstname = new char[strlen(line) + 1];
		memset(firstname, '\0', strlen(firstname));
		strcpy(firstname, line);

		first_names.push_back(firstname); //add name to first_names vector
	}

	return;
}




//reads file contents of "last_names.txt" and stores in last_names vector
void Generator::read_last_names_file() {
	FILE* pfile = NULL; //file pointer
	char line[50]; //holds contents of single line in "last_names.txt" (one name per line)
	pfile = fopen("last_names.txt", "r"); //read file

	while (fgets(line, 50, pfile)) { //read line by line (50 characters per line)
		char* pch; //char pointer
		pch = strchr(line, '\n');

		if (pch != NULL) { //add null terminating character at the end of each line
			*pch = '\0'; //dereference and assign value
		}

		char* lastname = new char[strlen(line) + 1];
		memset(lastname, '\0', strlen(lastname));
		strcpy(lastname, line);

		last_names.push_back(lastname); //add name to last_names vector
	}

	return;
}




//function to generate random student and adds to Student database
Student* Generator::generate_student(vector<int> &ids) {

	Student* s = new Student(); //create new Student

	int f = rand() % 25; //random number corresponding to line in "first_names.txt"
	s->first_name = first_names.at(f);
	cout << "First name: " << s->first_name << endl;

	int l = rand() % 25; //random number corresponding to line in "last_names.txt"
	s->last_name = last_names.at(l);

	id_num = 0; //initialize ID number to zero
	while (id_available(id_num, ids) == false) {
		id_num++; //keep incrementing ID number until available ID found
	}
	s->id = id_num;
	ids.push_back(id_num); //add new ID to vector of student IDs

	s->gpa = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 4.00));

	return s;
}





//function to check that ID number does not already exist in Student database
bool Generator::id_available(int id, vector<int> ids) {
	for (vector<int>::iterator it = ids.begin(); it < ids.end(); it++) {
		if (*it == id) {
			return false; //ID already taken
		}
	}
	return true; //ID available
}




//hash function
//hash alters based on size of table
int Generator::hash_function(char* first, int num_slots) {
	int x = 0;
	int len = strlen(first);
	for (int i = 0; i < len; i++) {
		x = x + (int)(*first + i);
	}
	int hash_index = x % num_slots;
	return hash_index;
}