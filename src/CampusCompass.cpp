#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
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
        Class new_class(classcode, stoi(location));
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



    string cmd; //get first line

    stringstream ss(command);
    ss >> cmd; // first word before space

    if (cmd == "insert") {

        string name;
        string student_id;
        string residence_id;
        ss >> name;
        ss >> student_id;
        ss >> residence_id;
        string classcode;
        vector<Class> classcodes;
        for (ss >> classcode) {
            Class course(classcode, classes[classcode].location_id);
            classcodes.push_back(course);
        }
        insert(name, student_id, stoi(residence_id), classcodes)

        //insert
    } 
    else if (cmd == "remove") {
        string student_id;
        ss >> student_id;
        remove(student_id);
        //remove
    } 
    else if (cmd == "dropClass") {
        string student_id;
        string classcode;
        ss >> student_id;
        ss >> classcode;
        dropClass(student_id, classcode);
        //drop class
    } 
    else if (cmd == "replaceClass") {
        string student_id;
        string classcode1;
        string classcode2;
        ss >> student_id;
        ss >> classcode1;
        ss >> classcode2;

        replaceClass(student_id, classcode1, classcode2);

        
    }
     else if (cmd == "removeClass") {
        string classcode;
        ss >> classcode;
        removeClass(classcode);


    }
     else if (cmd == "toggleEdgesClosure") {
        string location_id;
        vector<int> ids;
        for (ss >> location_id) {
            ids.push_back(stoi(location_id));
        }
        toggleEdgesClosure(ids);

    }
     else if (cmd == "checkEdgeStatus") {
        string location_id_x;
        string location_id_y;
        ss >> location_id_x;
        ss >> location_id_y;
        checkEdgeStatus(stoi(location_id_x), stoi(location_id_y));
    }
     else if (cmd == "isConnected") {
        string location_id_x;
        string location_id_y;
        ss >> location_id_x;
        ss >> location_id_y;
        isConnected(stoi(location_id_x), stoi(location_id_y));
    }
    else if (cmd == "printShortestEdges") {
         string student_id;
        ss >> student_id;
        printShortestEdges(student_id);

    }
    else if (cmd == "printStudentZone") {
        string student_id;
        ss >> student_id;
        printStudentZone(student_id);
    }
    else if (cmd == "verifySchedule") {
        string student_id;
        ss >> student_id;
        verifySchedule(student_id);
    }
    else {
        return false;
    }



    return is_valid;
}

bool CampusCompass::insert(const string &student_name, const string &student_id, const int &residence_location_id, const vector<CampusCompass::Class> classes)
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
    for (auto it = stu->classcodes.begin(); it != stu->classcodes.end(); it++) {
        if (it->class_code == classcode) {
            stu->classcodes.erase(it);
            return true;
        }
    }

    return false;
}

bool CampusCompass::replaceClass(const string &student_id, const string &classcode_1, const string &classcode_2)
{
    
 Student* stu = findStudentById(student_id);

for(int i = 0; i < stu->classcodes.size(); i++) {
    if (stu->classcodes[i].class_code == classcode_1) {
        stu->classcodes[i].class_code = classcode_2;
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
            if (s.second.classcodes[i].class_code == classcode) {
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
    if (location_id_x == location_id_y) {
        return true;
    }
    set<int> visited;
    queue<int> q;
    q.push(location_id_x);
    visited.insert(location_id_x);

    while (!q.empty()) {
        int u = q.front();
        
        q.pop();
      for (auto& p : graph[u]) {
        int v = p.first;
            if (closed_edges.count({u, v})) {
               continue;
            }
            if (v == location_id_y) {
                return true;
            }
            if (visited.count(v) == 0) {

                visited.insert(v);
                q.push(v);
            }
        }

    }
    return false;

    //BFS, if match end and say true
    //Find if theres a path between 2 locations
}

string CampusCompass::printShortestEdges(const string &student_id)
{
    //Good ol Dijkstras for all classes from residence hall starting point

    Student* stu = findStudentById(student_id);
    if (!stu) {
        return "";
    }
    int start = stu->residence_location_id;
    string result = "Name:" + stu->student_name + "\n";

    unordered_map<int, int> dist;
    unordered_map<int, int> prev;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    for (auto &g : graph) {
        dist[g.first] = INT_MAX;
    }
    dist[start] = 0;
    pq.push({0, start});

   

    
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) {
            continue;
        }
        for (auto [v, weight] : graph[u]) {
            if (closed_edges.count({u, v})) {
                continue;
            }

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    vector<Class> sortedClasses = stu->classcodes;
    sort(sortedClasses.begin(), sortedClasses.end());

    for (auto& c : sortedClasses) {
        int loc = c.location_id;
        int time = dist[loc];
        result += c.class_code + "| Total Time:" + to_string(time) + "\n";
    }

    return result;

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

