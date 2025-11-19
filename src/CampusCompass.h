#pragma once
#include <string>
#include <vector>

using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string& edges_filepath, const string& classes_filepath);
    bool ParseCommand(const string& command);
    bool insert(const string& student_name, const string& student_id, const string& residence_location_id, const vector<string> classcodes);
    bool remove(const string& student_id);
    bool dropClass(const string& student_id, const string& classcode);
    bool replaceClass(const string& student_id, const string& classcode_1, const string& classcode_2);
    int removeClass(const string& classcode);
    bool toggleEdgesClosure(const vector<int>);
    string checkEdgeStatus(const string& location_id_x, const string& location_id_y);
    bool isConnected(const string& location_id_x, const string& location_id_y);
    string printShortestEdges(const string& student_id);
    string printStudentZone(const string& student_id);
    //EXTRA CREDIT
    string verifySchedule(const string& student_id);
    
};
