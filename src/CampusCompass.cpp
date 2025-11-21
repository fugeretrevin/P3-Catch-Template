#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not

    //Edges first
    ifstream edges_file(edges_filepath);
    string line;
    if(!edges_file.is_open()) {
      return false;
    }
    getline(edges_file, line); // reads first line to skip the header (just titles)
    
    //need from to, names, and weight
    while (getline(edges_file, line)) { // reads next line
      if (line.empty()) {
        continue;
        }
        try {

        stringstream ss(line);
        string elem;  //for skipping
        string from;
        string to;
        string weight;


        getline(ss, from, ','); // read to the next comma and skip it
        
        getline(ss, to, ',');
        getline(ss, elem, ','); // skip the names
        getline(ss, elem, ','); // skip the names
        getline(ss, weight, ',');
        graph[stoi(from)].push_back(make_pair(stoi(to), stoi(weight)));

        } catch (...) {
          continue;
        }
    }

    //Classes Next
    ifstream classes_file(classes_filepath);
    string line;
    if(!classes_file.is_open()) {
      return false;
    }
    getline(classes_file, line); // reads first line to skip the header (just titles)
    
    //need from to, names, and weight
    while (getline(classes_file, line)) { // reads next line
      if (line.empty()) {
        continue;
        }
        try {

        stringstream ss(line); //skip top line
        string elem;  //for skipping
        string classcode;
        string location;


        getline(ss, classcode, ','); // read to the next comma and skip it
        
        getline(ss, location, ',');
        Class new_class(classcode, location);
        classes[classcode] = new_class;

        } catch (...) {
          continue;
        }
    }
    return true;

}


bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}

bool CampusCompass::insert(const string &student_name, const string &student_id, const int &residence_location_id, const vector<Class> classes)
{
    Student new_student(student_name, student_id, residence_location_id, classes);
    students[student_id] = new_student;
    return true;
}

bool CampusCompass::remove(const string &student_id)
{
    students.erase(student_id);
    return true;
}

bool CampusCompass::dropClass(const string &student_id, const string &classcode)
{
    Student* stu = findStudentById(student_id);
    auto it = find(stu->classcodes.begin(), stu->classcodes.end(), classcode);

    if (it != stu->classcodes.end()) {
        stu->classcodes.erase(it);
        return true;
    }
    return false;
}

bool CampusCompass::replaceClass(const string &student_id, const string &classcode_1, const string &classcode_2)
{
    
 Student* stu = findStudentById(student_id);

for(int i = 0; i < stu->classcodes.size(); i++) {
    if (stu->classcodes[i] == classcode_1) {
        stu->classcodes[i] = classcode_2;
        return true;
    }
}
    return false;
}

int CampusCompass::removeClass(const string &classcode)
{
    classes.erase(classcode);
    for (auto s : students) {
        for (int i = 0; i < s.second.classcodes.size(); i++) {
            if (s.second.classcodes[i] == classcode) {
                s.second.classcodes.erase(s.second.classcodes.begin() + i);
                break;
            }
        }
    }
    //Remove from all students
    return 0;
}

bool CampusCompass::toggleEdgesClosure(const vector<int> edges) 
{
    if (edges.size() %2 != 0) {
        return false;
    }
    for(int i = 0; i < edges.size() - 1; i+=2) {
        if (closed_edges.count(make_pair(edges[i], edges[i+1])) == 0) {
            closed_edges.insert(make_pair(edges[i], edges[i+1]));
        }
        else {
            closed_edges.erase(make_pair(edges[i], edges[i+1]));
        }
    }

    return true;
}

string CampusCompass::checkEdgeStatus(const int &location_id_x, const int &location_id_y)
{
    string status = "";
        if (!edgeExists(location_id_x, location_id_y)) {
            status = "DNE";
        }
         else if (closed_edges.count(make_pair(location_id_x, location_id_y)) == 0) {
            status = "Open";
         }
         else {
 
            status = "Closed";
         }

    return status;
}

bool CampusCompass::isConnected(const int &location_id_x, const int &location_id_y)
{
    //BFS, if match end and say true
    //Find if theres a path between 2 locations
    return false;
}

string CampusCompass::printShortestEdges(const string &student_id)
{
    //Good ol Dijkstras for all classes from residence hall starting point
    Student* stu = findStudentById(student_id);
    for(auto course : stu->classcodes)
    unordered_map<int, int> dist;
    unordered_map<int, int> prev;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;


    
    return string();
}

string CampusCompass::printStudentZone(const string &student_id)
{

    //MST

    return string();
}

CampusCompass::Student* CampusCompass::findStudentById(const string &id) {
    auto it = students.find(id);
    if (it != students.end()) {
        return &it->second;
    }
    return nullptr;
}

bool CampusCompass::edgeExists(const int& x, const int& y) {
    for (auto& p : graph[x]) {
        if (p.first == y)  {
            return true;
        }
    }
    return false;
}


//EXTRA CREDIT
string CampusCompass::verifySchedule(const string &student_id)
{
    return string();
}

