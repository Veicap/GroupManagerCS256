#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "printTable.h"
#include <ctime>
#include <thread>
#pragma warning(disable:4996)
const std::string FILE_NAME = "group.txt";
const std::string FILE_NAMESTAT = "statistic.txt";
std::string transformDateFormat(std::string inputDate) {
    if (inputDate.length() != 8) {
        std::cerr << "Invalid input date format. It should be YYYYMMDD." << std::endl;
        return "";
    }

    std::string yearStr = inputDate.substr(0, 4);
    std::string monthStr = inputDate.substr(4, 2);
    std::string dayStr = inputDate.substr(6, 2);

    std::string transformedDate = dayStr + "/" + monthStr + "/" + yearStr;

    return transformedDate;
}
struct ProjectSubmission {
    int projectNumber;
    std::string submissionDate;
    std::string statement;
};
struct Group {
    int groupNumber;
    std::string nameOfGroup;
    std::vector<std::string> nameOfStudentList;
    std::vector<ProjectSubmission*> submissions;
};
struct Project {
    int projectNumber;
    std::string shortDescriptionOfProject;
    std::string dayOfDeadLine;
    std::string monthOfDeadLine;
    std::string yearOfDeadLine;
    std::string deadline;
};
struct HandleData {
    // Option 1:
    std::vector<Group*> listOfGroup;
    // Read file text group.txt
    void ReadFile() {
        std::ifstream groupFile(FILE_NAME);
        if (!groupFile.is_open()) {
            std::cerr << "Error opening file: " << FILE_NAME << std::endl;
            return;
        }

        std::string groupInformation;
        Group* group = new Group();
        // Read file
        while (std::getline(groupFile, groupInformation)) {
            if (groupInformation.find("Group") != std::string::npos) {
                if (group->groupNumber != NULL) {
                    listOfGroup.push_back(group);
                }
                group = new Group(); // Create new group;
                int pos = groupInformation.find(" ") + 1;
                int groupNumber = std::stoi(groupInformation.erase(0, pos));
                group->groupNumber = groupNumber;
            }
            else {
                group->nameOfStudentList.push_back(groupInformation);
            }
        }
        if (group->groupNumber != NULL) {
            listOfGroup.push_back(group);
        }        
        groupFile.close();
        
    }
    // Input information and push data into vector listOfGroup
    // Compare 2 name
    bool IsIdentical(const std::string name1, const std::string name2) {
   
        std::string ten1Lower, ten2Lower;
        for (char c : name1) {
            ten1Lower += std::tolower(c);
        }
        for (char c : name2) {
            ten2Lower += std::tolower(c);
        }

        return ten1Lower == ten2Lower;
    }
    std::string capitalizeName(const std::string& name) {
        std::string capitalizedName = name; 
        capitalizedName[0] = std::toupper(capitalizedName[0]);

        for (int i = 1; i < capitalizedName.length(); ++i) {
            if (std::isspace(capitalizedName[i - 1])) {
                capitalizedName[i] = std::toupper(capitalizedName[i]);
            }
            else {
                capitalizedName[i] = std::tolower(capitalizedName[i]);
            }
        }

        return capitalizedName;
    }
    void InputInformation() {
        Group* group = new Group();
        std::vector<std::string> nameOfStudentEnteredList;
        int numberOfGroupAdd = 0;
        std::cout << "(!)Class already have " << listOfGroup.size() << " groups." << "\n";
        // Enter number of group need to add until greater than 0
        while (numberOfGroupAdd <= 0) {
            std::cout << "(-)Enter number of groups in the class you want to add: "; std::cin >> numberOfGroupAdd;
            std::cout << "-------------------------------------------------------------\n";
            if (numberOfGroupAdd <= 0) {
                std::cout << "(!) You need to enter the number of added groups greater than 0 . Please re-enter\n";
            }
        }
        int numberOfGroups = numberOfGroupAdd + listOfGroup.size();
        for (int i = listOfGroup.size(); i < numberOfGroups; i++) {
            group->groupNumber = i + 1;
            int numberOfStudentInAGroup = 0;
            std::cout << "Enter information of group " << i + 1 << "\n";
            while (numberOfStudentInAGroup <= 0) {
                std::cout << "(-)Enter number of students: "; std::cin >> numberOfStudentInAGroup;
                if (numberOfStudentInAGroup <= 0) {
                    std::cout << "(!) You need to enter the number of added students greater than 0. Please re-enter\n";
                }
            }
            std::cin.ignore();
            for (int j = 0; j < numberOfStudentInAGroup; j++) {
                std::string nameOfStudent = "";
                bool isDuplicate = false;
                // enter number of student until not duplicate
                do {
                    isDuplicate = false;
                    std::cout << "(-)Enter name of student number " << j + 1 << ": ";
                    std::getline(std::cin, nameOfStudent);
                    nameOfStudent = capitalizeName(nameOfStudent);
                    for (auto group : listOfGroup) {
                        for (auto& nameOfStudentInList : group->nameOfStudentList) {
                            if (IsIdentical(nameOfStudentInList, nameOfStudent)) {
                                isDuplicate = true;
                                std::cout << "(!)Student already have in another group. Please re-enter\n";
                                break;
                            }
                        }
                        if (isDuplicate) {
                            break;
                        }

                    }
                    for (auto& nameOfStudentEntered : nameOfStudentEnteredList) {
                        if (IsIdentical(nameOfStudentEntered, nameOfStudent)) {
                            isDuplicate = true;
                            std::cout << "(!)Student already have in another group. Please re-enter\n";
                        }
                    }
                    if (!isDuplicate) {
                        group->nameOfStudentList.push_back(nameOfStudent);
                        nameOfStudentEnteredList.push_back(nameOfStudent);
                    }
                } while (isDuplicate);
            }
            std::cout << "-------------------------------------------------\n";
            listOfGroup.push_back(group);
            group = new Group();
            nameOfStudentEnteredList.clear();
        }
        std::cout << "(!)You have successfully entered the information\n";
        //delete group;
    }
    void SaveInformation() {
        std::ofstream groupFile(FILE_NAME);
        if (!groupFile.is_open()) {
            std::cerr << "Error opening file for writing: " << FILE_NAME << std::endl;
            return;
        }

        for (auto group : listOfGroup) {
            groupFile << "Group " + group->groupNumber << "\n";
            for (auto& nameOfStudent : group->nameOfStudentList) {
                groupFile << nameOfStudent << "\n";
            }
        }
        std::cout << "(!)Save information successfully!\n";
        groupFile.close();
    }
    void DisplayDataAllGroup() {
        printBorderOfTableGroup();
        printTopicOfTableGroup();
        printBorderOfTableGroup();

        int orderOfGroup = 1;

        for (auto group : listOfGroup) {
            int orderOfStudentInGroup = 1;
            for (auto& nameOfStudent : group->nameOfStudentList) {
                if (orderOfStudentInGroup == 1) {
                    printRowOfTableGroup(orderOfGroup, nameOfStudent);
                }
                else {
                    printRowOfTableGroup(nameOfStudent);
                }
                orderOfStudentInGroup++;
            }
            printBorderOfTableGroup();
            orderOfGroup++;

        }
    }
    void DisplayDataSingleGroup() {
       
        int groupNumber;
        std::string nameOfGroup;
        std::cout << "Enter number of groups: ";
        std::cin >> groupNumber;
        
        printBorderOfTableGroup();
        printTopicOfTableGroup();
        printBorderOfTableGroup();
        int orderOfGroup = 1;
        for (auto group : listOfGroup) {
            int orderOfStudentInGroup = 1;
            if (group->groupNumber == groupNumber) {
                for (auto& nameOfStudent : group->nameOfStudentList) {

                    if (orderOfStudentInGroup == 1) {
                        printRowOfTableGroup(orderOfGroup, nameOfStudent);
                    }
                    else {
                        printRowOfTableGroup(nameOfStudent);
                    }
                    orderOfStudentInGroup++;
                }
                printBorderOfTableGroup();
            }
            orderOfGroup++;
        }
        std::cin.ignore();
    }

    //Option 2
    void TransformDateMonth(std::string& dateIndex) {
        if (dateIndex.length() <= 1) {
            dateIndex = '0' + dateIndex;
        }
    }
    std::vector<Project*> projects;
    void InputProjectInformation() {
        int dayIndex, monthIndex, yearIndex;
        std::string day, month, year;
        if (projects.empty()) {
            Project* project = new Project();
            int numberOfProject;
            std::cout << "Enter number of project: "; std::cin >> numberOfProject;
            for (int i = 1; i <= numberOfProject; i++) {
                std::cin.ignore();
                std::cout << "(*) Enter information of project " << i << "\n";
                project->projectNumber = i;
                std::cout << "(+) Enter a short description about project: ";
                std::getline(std::cin, project->shortDescriptionOfProject);
                std::cout << "(-) Enter a submission deadline of project(day month year): ";
                std::cin >> dayIndex; std::cin >> monthIndex; std::cin >> yearIndex;
                day = std::to_string(dayIndex); month = std::to_string(monthIndex); year = std::to_string(yearIndex);
                TransformDateMonth(day); project->dayOfDeadLine = day;
                TransformDateMonth(month); project->monthOfDeadLine = month;
                project->yearOfDeadLine = year;
                project->deadline = year + month + day;
                addProject(project);
                project = new Project();
            }
            delete project;
        }
        std::cout << "(!)You have successfully entered the information\n";
    }
    
    void displayProjects() const {
        if (projects.empty()) {
            std::cout << "No projects available.\n";
            return;
        }

        printBorderForOption2();
        std::cout << "\t"
            << std::left << std::setfill(' ')
            << std::setw(2) << "|" << std::setw(8) << "Project"
            << std::setw(13) << "|" << std::setw(27) << "Description"
            << std::setw(10) << "|" << std::setw(20) << "Deadline"
            << "|" << "\n";
        printBorderForOption2();
        for (const auto project : projects) {
            std::string deadline = transformDateFormat(project->deadline);
            std::cout << "\t"
                << std::left << std::setfill(' ')
                << std::setw(5) << "|" << std::setw(5) << project->projectNumber
                << std::setw(13) << "|" << std::setw(27) << project->shortDescriptionOfProject
                << std::setw(10) << "|" << std::setw(20) << deadline
                << "|" << "\n";
            printBorderForOption2();
        }
    }
    void addProject(Project* project) {
        projects.push_back(project);
    }
    //Option 3
    void SubmitProject() {
        int groupIndex, projectIndex, dayIndex, monthIndex, yearIndex;
        std::string day, month, year;
        std::cout << "Enter group number for project submission: ";
        std::cin >> groupIndex;

        if (groupIndex < 1 || groupIndex > listOfGroup.size()) {
            std::cout << "Invalid group number. Please try again.\n";
            return;
        }

        std::cout << "Enter project number: ";
        std::cin >> projectIndex;

        if (projectIndex < 1 || projectIndex > projects.size()) {
            std::cout << "Invalid project number. Please try again.\n";
            return;
        }
        std::cout << "Enter submission date (day month year): ";
        std::cin >> dayIndex;
        std::cin >> monthIndex;
        std::cin >> yearIndex;
        day = std::to_string(dayIndex); month = std::to_string(monthIndex); year = std::to_string(yearIndex);
        TransformDateMonth(day);
        TransformDateMonth(month);
        ProjectSubmission* submission = nullptr;


        for (auto& existingSubmission : listOfGroup[groupIndex - 1]->submissions) {
            if (existingSubmission->projectNumber == projectIndex) {
                submission = existingSubmission;
                break;
            }
        }

        if (!submission) {
            submission = new ProjectSubmission;
            submission->projectNumber = projectIndex;
            listOfGroup[groupIndex - 1]->submissions.push_back(submission);
        }
        submission->submissionDate = year + month + day;

        std::cout << "Project submitted successfully!\n";
    }
    // Option 4
    std::vector<ProjectSubmission*> submissions;
    std::string stateOnTime = "On Time";
    std::string stateLate = "Late";
    std::string stateNotSubmit = "Not submited yet";
    void SetStatement() {
        for (const auto& group : listOfGroup) {
            // Initialize submission status as "Not yet submitted";
            for (const auto& project : projects) {
                bool submitted = false;
                for (auto& submission : group->submissions) {
                    if (project->projectNumber == submission->projectNumber) {
                        if (submission->submissionDate.empty()) {
                            submission->statement = stateNotSubmit;
                        }
                        else if (submission->submissionDate <= project->deadline) {
                            submission->statement = stateOnTime;
                        }
                        else if (submission->submissionDate >= project->deadline) {
                            submission->statement = stateLate;
                        }

                        submitted = true;
                        break;
                    }

                }
                if (!submitted) {
                    ProjectSubmission* submission = new ProjectSubmission();
                    submission->projectNumber = project->projectNumber;
                    submission->statement = stateNotSubmit;
                    group->submissions.push_back(submission);
                }
            }

        }
    }
    void displaySubmissionStatus() {
        std::string optionOf4;
        std::cout << "(1) Display Submission Status By Project\n"
            << "(2) Display Submission Status By Group\n";
        std::cout << "Enter your option: ";
        std::getline(std::cin, optionOf4);

        if (optionOf4 == "1") {
            int project_number;
            std::cout << "Enter the project number: ";
            std::cin >> project_number;
            std::cin.ignore();
            // Display table header
            printBorderForOption4();
            std::cout << "\t" << std::setfill(' ')
                << "|" << std::setw(9) << "Group"
                << "|" << std::setw(19) << "\tStatement" << column_char << "\n";
            printBorderForOption4();

            for (const auto& group : listOfGroup) {
                // Initialize submission status as "Not yet submitted"
                for (const auto& submission : group->submissions) {
                    // Check if the group has a submission for the specified project
                    if (submission->projectNumber == project_number) {

                        std::cout << "\t" << std::setfill(' ')
                            << "|" << std::setw(9) << group->groupNumber
                            << "|" << std::setw(23) << submission->statement << column_char << "\n";
                        printBorderForOption4();
                        break;
                    }
                }
            }
        }
        else if (optionOf4 == "2") {

            int group_Number;
            std::cout << "Enter group number: ";
            std::cin >> group_Number;
            // Display table header
            printBorderForOption4();
            std::cout << "\t" << std::setfill(' ')
                << "|" << std::setw(9) << "Project"
                << "|" << std::setw(19) << "\tStatement" << column_char << "\n";
            printBorderForOption4();
            for (const auto& project : projects) {

                for (const auto& submission : listOfGroup[group_Number - 1]->submissions) {
                    if (submission->projectNumber == project->projectNumber) {

                        std::cout << "\t" << std::setfill(' ')
                            << "|" << std::setw(9) << project->projectNumber
                            << "|" << std::setw(23) << submission->statement << column_char << "\n";
                        printBorderForOption4();
                        break;
                    }
                }
            }
            std::cin.ignore();
        }

    }
    // Option 5
    void exportResult(std::string statDate) {
        std::ofstream stat(FILE_NAMESTAT);
        if (!stat.is_open()) {
            std::cerr << "Error opening file for writing: " << FILE_NAMESTAT << std::endl;
            return;
        }
        stat << std::setw(29);
        for (const auto& project : projects) {
            if (project->deadline <= statDate) {
                stat << "Project " << project->projectNumber << std::setw(22);
            }
        }

        stat << std::setw(0) << std::endl;
        for (const auto& group : listOfGroup) {
            stat << "Group " << group->groupNumber;
            for (const auto& project : projects) {
                for (const auto& submission : group->submissions) {
                    if (project->deadline <= statDate && project->projectNumber == submission->projectNumber)
                        stat << std::setw(23) << submission->statement;
                }
            }
            stat << "\n";
        }
        stat.close();
    }

    void overallStat() {
        int i = 0;
        int groupSize = listOfGroup.size();
        std::string optionOf5;
        std::cout << "(1) Show the statistic from beginning to a specific date\n"
            << "(2) Show the statistic from beginning to current time\n";
        std::cout << "Enter your option: ";
        std::getline(std::cin, optionOf5);

        if (optionOf5 == "1") {
            int statDay, statMonth, statYear;
            std::string dD, mM, yY;
            std::string statDate;
            std::cout << "Enter the date (day month year): ";
            std::cin >> statDay >> statMonth >> statYear;
            dD = std::to_string(statDay); mM = std::to_string(statMonth); yY = std::to_string(statYear);
            TransformDateMonth(dD);
            TransformDateMonth(mM);
            statDate = yY + mM + dD;
            //std::cin.ignore();
            for (const auto& project : projects) {
                if (project->deadline <= statDate) i++;
            }
            if (i == 0) std::cout << "No project before this date.\n";
            else {
                std::cout << "Now showing the statistic of projects before " << statDay << "/" << statMonth << "/" << statYear << std::endl << std::endl;
                printBorderForOption5(i);
                std::cout << "\t" << std::setfill(' ')
                    << "|" << std::setw(9) << "Group";
                for (const auto& project : projects) {
                    if (project->deadline <= statDate) std::cout << "|" << "Project " << std::setw(15) << project->projectNumber;
                }
                std::cout << column_char << "\n";
                printBorderForOption5(i);

                for (const auto& group : listOfGroup) {
                    std::cout << "\t" << std::setfill(' ')
                        << "|" << std::setw(9) << group->groupNumber << "|";
                    for (const auto& project : projects) {
                        for (const auto& submission : group->submissions) {
                            if (project->deadline <= statDate && project->projectNumber == submission->projectNumber)
                                std::cout << std::setw(23) << submission->statement << "|";
                        }
                    }
                    std::cout << "\n";
                    printBorderForOption5(i);
                }
                exportResult(statDate);
                std::cin.ignore();
                std::cout << "\n" << "The result is exported to the statistic.txt file\n";
            }
        }



        else if (optionOf5 == "2") {
            std::string statDate;
            std::string day, month, year;
            std::time_t currentTime = std::time(nullptr);
            std::tm* currentDateTime = std::localtime(&currentTime);
            year = std::to_string(currentDateTime->tm_year + 1900);
            month = std::to_string(currentDateTime->tm_mon + 1);
            day = std::to_string(currentDateTime->tm_mday);
            std::cout << "Current Date: " << day << "-" << month << "-" << year << std::endl;
            TransformDateMonth(day);
            TransformDateMonth(month);
            statDate = year + month + day;

            for (const auto& project : projects) {
                if (project->deadline <= statDate) i++;
            }
            if (i == 0) std::cout << "No project before this date.\n";
            else {
                std::cout << "Now showing stastics before today's date\n";
                printBorderForOption5(i);
                std::cout << "\t" << std::setfill(' ')
                    << "|" << std::setw(9) << "Group";
                for (const auto& project : projects) {
                    if (project->deadline <= statDate) std::cout << "|" << "Project " << std::setw(15) << project->projectNumber;
                }
                std::cout << column_char << "\n";
                printBorderForOption5(i);

                for (const auto& group : listOfGroup) {
                    std::cout << "\t" << std::setfill(' ')
                        << "|" << std::setw(9) << group->groupNumber << "|";
                    for (const auto& project : projects) {
                        for (const auto& submission : group->submissions) {
                            if (project->deadline <= statDate && project->projectNumber == submission->projectNumber)
                                std::cout << std::setw(23) << submission->statement << "|";
                        }
                    }
                    std::cout << "\n";
                    printBorderForOption5(i);
                }
                exportResult(statDate);
                std::cout << "\n" << "The result is exported to the statistic.txt file\n";
            }
        }

    }


    // Option 6
    //Display group submit on time;
    void DisplaySubmitOnTimeOrLate() {
        std::cout << "(1) Group submit on time\n"
            << "(2) Group do not complete\n";    
        std::string optionOf6;
        std::cout << "Enter your option: "; std::getline(std::cin, optionOf6);
        if (optionOf6 == "1") {
            std::cout << "(*)Group submit on time is: \n";
            
            for (const auto& project : projects) {
                std::cout << "(-)Project " << project->projectNumber << ": ";
                int numberofGroupOntime = 0;
                for (const auto& group : listOfGroup) {
                    
                    for (const auto& submission : group->submissions) {
                        if (project->projectNumber == submission->projectNumber) {
                            if (submission->statement == stateOnTime) {
                                std::cout << "Group " << group->groupNumber << ", ";
                                numberofGroupOntime++;
                            }                           
                            break;
                        }
                    }
                }
                if (numberofGroupOntime == 0) {
                    std::cout << "No group submitted on time";
                }
                std::cout << std::endl;
            }
        }
        else if (optionOf6 == "2") {
            std::cout << "(*)Group do not complete is: \n";

            for (const auto& project : projects) {
                std::cout << "(-)Project " << project->projectNumber << ": ";
                int numberofGroupLateOrNotSumnit = 0;
                for (const auto& group : listOfGroup) {
                    for (const auto& submission : group->submissions) {
                        if (project->projectNumber == submission->projectNumber) {
                            if (submission->statement == stateLate || submission->statement == stateNotSubmit) {
                                std::cout << "Group " << group->groupNumber << ", ";
                                numberofGroupLateOrNotSumnit++;
                            }
                            break;
                        }
                        
                    }
                }
                if (numberofGroupLateOrNotSumnit == 0) {
                    std::cout << "No group submitted late or not submit";
                }
                std::cout << std::endl;
            }
        }
       
    }
    //option 7
    
};

int main()
{
    HandleData* handleData = new HandleData();
    handleData->ReadFile();
    std::string option;
    while (true) {
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(19) << "|"
            << std::setw(21) << "MENU"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(1) Group Management"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(2) Project Management"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(3) Project Submission"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(4) Submission Status"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(5) Overall Statistic"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(6) Project Status of Group"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\t\t\t\t\t"
            << std::left << std::setfill(' ')
            << std::setw(8) << "|"
            << std::setw(32) << "(7) Quit"
            << "|" << "\n";
        printBorderForMenu();
        std::cout << "\nEnter your option: ";
        std::getline(std::cin, option);
        if (option == "1") {
            std::string optionOf1 = "";
            std::cout << "(1) Input Information of Groups\n"
                << "(2) Display Information\n"
                << "(3) Save Groups Information\n";
            std::cout << "Enter your option: ";
            std::getline(std::cin, optionOf1);
            if (optionOf1 == "1") {
                handleData->InputInformation();
            }
            else if (optionOf1 == "2") {
                std::string optionOf1_2 = "";
                std::cout << "(1) Display information for all group\n"
                    << "(2) Display information for specific group\n";
                std::cout << "Enter your option: "; std::getline(std::cin, optionOf1_2);
                if (optionOf1_2 == "1") {
                    handleData->DisplayDataAllGroup();
                }
                else if (optionOf1_2 == "2") {
                    handleData->DisplayDataSingleGroup();
                }
            }
            else if (optionOf1 == "3") {
                handleData->SaveInformation();
            }
        }
        else if (option == "2") {
            std::string optionOf2;
            std::cout << "(1) Input Projects Information\n"
                << "(2) Display Information of Groups\n";
            std::cout << "Enter your option: ";
            std::getline(std::cin, optionOf2);
            if (optionOf2 == "1") {
                handleData->InputProjectInformation();
                handleData->SetStatement();
                std::cin.ignore();
            }
            else if (optionOf2 == "2") {
                handleData->displayProjects();
            }
        }
        else if (option == "3") {

            handleData->SubmitProject();
            handleData->SetStatement();
            std::cin.ignore();
        }
        else if (option == "4") {
            handleData->displaySubmissionStatus();         
        }
        else if (option == "5") {
            handleData->overallStat();
        }
        else if (option == "6") {
            handleData->DisplaySubmitOnTimeOrLate();
        }
        else if (option == "7") {
            std::string optionOf7, optionOf7_1;
            std::cout << "Do you want to quit the program (Y/N): ";
            std::getline(std::cin, optionOf7);
            
            if (optionOf7 == "Y" || optionOf7 == "y") {
                std::cout << "Do you want to save information before exit the program (Y/N): ";
                std::getline(std::cin, optionOf7_1);
                if (optionOf7_1 == "Y" || optionOf7_1 == "y") {
                    handleData->SaveInformation();
                }
                std::cout << "(*)Exiting the program....." << std::endl;

                // Sleep for one second (1000 milliseconds)
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "(*)You've exited the program!";
                break;
            }

        }
        std::cout << "(*)Press enter to back to menu...";
        //std::cin.ignore();
        std::getline(std::cin, option);
        system("cls");
    }



    delete handleData;
}