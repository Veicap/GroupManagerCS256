#ifndef __PRINTABLE_H__
#define __PRINTABLE_H__
#include <iostream>
#include <iomanip>
#include <string>

const char corner_char = '+';
const char row_char = '-';
const char column_char = '|';
const int group_width = 9;
const int nameofStudent_width = 18;
const int project_width = 20;
const std::string group = "Group";
const std::string studentName = "Student's name";

// Table of Group
void printRowOfTableGroup(std::string nameOfStudent) {
	std::cout << "\t"
		<< std::left << std::setfill(' ')
		<< std::setw(group_width) << column_char
		<< column_char << std::setw(nameofStudent_width - 1) << nameOfStudent
		<< column_char << "\n";
}

void printRowOfTableGroup(int order, std::string nameOfStudent) {
	std::cout << "\t"
		<< std::left << std::setfill(' ')
		<< column_char << std::setw(group_width - 1) << order
		<< column_char << std::setw(nameofStudent_width - 1) << nameOfStudent << column_char << "\n";
}
void printTopicOfTableGroup() {
	std::cout << "\t"
		<< std::left << std::setfill(' ')
		<< column_char << std::setw(group_width - 1) << group
		<< column_char << std::setw(nameofStudent_width - 1) << studentName << column_char << "\n";
}
void printBorderOfTableGroup() {
	std::cout << "\t"
		<< std::left << std::setfill(row_char)
		<< std::setw(group_width) << corner_char
		<< std::setw(nameofStudent_width) << corner_char << corner_char << "\n";
}
void printBorderForOption4() {
	std::cout << "\t"
		<< std::left << std::setfill(row_char)
		<< std::setw(10) << corner_char
		<< std::setw(24) << corner_char << corner_char << "\n";
}
void printBorderForOption5(int i) {
	std::cout << "\t"
		<< std::left << std::setfill(row_char)
		<< std::setw(10) << corner_char;
	for (int j = 0; j < i; j++) {
		std::cout << std::setw(24) << corner_char;
	}
	std::cout << corner_char << "\n";
}
void printBorderForMenu() {
	std::cout << "\t\t\t\t\t"
		<< std::left << std::setfill(row_char)
		<< std::setw(40) << corner_char
		<< corner_char << "\n";
}
void printBorderForOption2() {
	std::cout << "\t"
		<< std::left << std::setfill(row_char)
		<< std::setw(10) << corner_char
		<< std::setw(40) << corner_char
		<< std::setw(30) << corner_char
		<< corner_char << "\n";
}

#endif // !__PRINTABLE_H__


