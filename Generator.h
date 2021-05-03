//header file

#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include "Student.h"
using namespace std;

class Generator {
public:
	Generator(); //constructor

	int id_num; 
	vector<char*> first_names; //holds all names in "first_names.txt"
	vector<char*> last_names; //holds all names in "last_names.txt"

	void read_first_names_file(); //reads file contents of "first_names.txt" and stores in first_names vector
	void read_last_names_file(); //reads file contents of "last_names.txt" and stores in last_names vector

	Student* generate_student(vector<int> &ids); //generates random student and adds to database

	bool id_available(int id, vector<int> ids); //checks if student with generated ID already exists

	int hash_function(char* first, int num_slots); //hash function
};


#endif