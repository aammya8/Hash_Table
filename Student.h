//header file

#ifndef STUDENT_H
#define STUDENT_H

#include <cstring>
#include <string>


//create Student (node) struct 
struct Student {

	//data members
	char* first_name = new char();
	char* last_name = new char();
	int id;
	float gpa;
	Student* next;

};

#endif