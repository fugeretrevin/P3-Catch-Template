#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>

#include <set>
using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?

    // map<from id, vector<pair<to id, weight>>>
    map<int, vector<pair<int, int>>> graph;
    set<pair<int, int>> closed_edges;

    //key: from "A", value: to <("B", 2), ("C", 3)>
    public:
    struct Class {
        string class_code;
        int location_id;
        Class(string code, int location) {
            class_code = code;
            location_id = location;
        }
        bool operator<(const Class& other) const {
                return class_code < other.class_code;
            }
        
    };
    struct Student {
        string student_name;
        string student_id;
        int residence_location_id;
        vector<Class> classes;
        Student(string name, string id, int residence_location_id, vector<Class> classes) {
            student_name = name;
            student_id = id;
            this->residence_location_id = residence_location_id;
            this->classes = classes;
        }
    };

    
    private:

    unordered_map<string, Student> students;
    unordered_map<string, Class> classes;


public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string& edges_filepath, const string& classes_filepath);
    bool ParseCommand(const string& command);
    bool insert(const string& student_name, const string& student_id, const int& residence_location_id, const vector<Class> classes);
    bool remove(const string& student_id);
    bool dropClass(const string& student_id, const string& classcode);
    bool replaceClass(const string& student_id, const string& classcode_1, const string& classcode_2);
    int removeClass(const string& classcode);
    bool toggleEdgesClosure(const vector<int>);
    string checkEdgeStatus(const int& location_id_x, const int& location_id_y);
    bool isConnected(const int& location_id_x, const int& location_id_y);
    string printShortestEdges(const string& student_id);
    string printStudentZone(const string& student_id);
    Student* findStudentById(const string &id);
    bool edgeExists(const int& x, const int& y);

    
    //EXTRA CREDIT
    int getShortestTime(const int& start_loc, const int& end_loc);
    string verifySchedule(const string& student_id);

};
