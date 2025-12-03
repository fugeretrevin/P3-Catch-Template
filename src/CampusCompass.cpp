#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <climits>
#include <regex>
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
            if (!weight.empty() && weight.back() == '\r') {
                weight.pop_back();
            }
        graph[stoi(from)].push_back(make_pair(stoi(to), stoi(weight)));
        graph[stoi(to)].push_back(make_pair(stoi(from), stoi(weight)));

        } catch (...) {
          continue;
        }
    }

    //Classes Next
    ifstream classes_file(classes_filepath);
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
            string start_time;
            string end_time;


        getline(ss, classcode, ','); // read to the next comma and skip it

        getline(ss, location, ',');
            getline(ss, start_time, ','); // read to the next comma and skip it
            getline(ss, end_time, ','); // read to the next comma and skip it
        if (!end_time.empty() && end_time.back() == '\r') {
            end_time.pop_back();
        }
        classcode.erase(0, classcode.find_first_not_of(" \n\r\t "));
            classcode.erase(classcode.find_last_not_of(" \n\r\t ") + 1);
        Class new_class(classcode, stoi(location), start_time, end_time);
        classes[classcode] = new_class;

        } catch (...) {
            cerr <<"skipping invalid class line" << line << endl;
          continue;
        }
    }
    return true;

}


bool CampusCompass::ParseCommand(const string &command) {
    string cmd = command;

    while(!cmd.empty() && (cmd.back() == '\r' || cmd.back() == '\n' || isspace(cmd.back()))) {
        cmd.pop_back();
    }
    if (cmd.empty()) {
        return true;
    }


    stringstream ss(cmd);
    string func;
    ss >> func; // first word before space

    vector<string> args;
    string curr_arg;
    bool in_quotes = false;
    size_t pos = cmd.find(func);
    if (pos != string::npos) {
        pos += func.length();
        while (pos < cmd.length() && isspace(cmd[pos])) {
            pos++;
        }
    }
    for (size_t i = pos; i < cmd.length(); i++) {
        char c = cmd[i];
        if (c == '"') {
            in_quotes = !in_quotes;
            curr_arg += c;
        }
        else if (isspace(c) && !in_quotes) {
            if (!curr_arg.empty()) {
                args.push_back(curr_arg);
                curr_arg.clear();
            }
        }
        else {
            curr_arg += c;
        }
    }
    if (!curr_arg.empty()) {
        args.push_back(curr_arg);
    }

    if (func == "insert") {
        size_t min_args = 5;
        if (args.size() < min_args) {
            return false;
        }
        string name = args[0];
        if (name.size() >= 2 && name.front() == '"' && name.back() == '"') {
            name = name.substr(1, name.size() - 2);
            regex name_regex("^[A-Za-z ]+$");
            if (!regex_match(name, name_regex)) {
                return false;
            }
        }
        else {
            return false;
        }
        string student_id = args[1];
        regex id_regex("^[0-9]{8}$");
        if (!regex_match(student_id, id_regex)) {
            return false;
        }

        int residence_id = 0;
        int numClasses = 0;

        try {
            residence_id = stoi(args[2]);
        }
        catch (...) {
            cerr << "Invalid Residence ID";
            return false;
        }
        try {
            numClasses = stoi(args[3]);
        }
        catch (...) {
            cerr << "Invalid num classes";
            return false;
        }
        vector<CampusCompass::Class> student_classes;

        for (int i = 4; i < numClasses + 4; i++) {
            if ((size_t)i >= args.size()) {
                return false;
            }
            string classcode = args[i];
            auto iter_find = this->classes.find(classcode);
            if (iter_find == this->classes.end()) {
                cerr << "Class code not found" << endl;
                return false;
            }
            student_classes.push_back(iter_find->second);
        }
        insert(name, student_id, residence_id, student_classes);
        return true;

    }
    else if (func == "remove") {
        size_t required_args = 1;
        if (args.size() != required_args) {
            return false;
        }
        string student_id = args[0];
        remove(student_id);
        return true;
        //remove
    }
    else if (func == "dropClass") {
        size_t required_args = 2;
        if (args.size() != required_args) {
            cerr << "requires 2 arguments";
            return false;
        }
        string student_id = args[0];
        string classcode = args[1];
        dropClass(student_id, classcode);
        return true;
        //drop class
    }
    else if (func == "replaceClass") {
        size_t required_args = 3;
        if (args.size() != required_args) {
            cerr << "requires 3 arguments";
            return false;
        }
        string student_id = args[0];
        string classcode1 = args[1];
        string classcode2 = args[2];


       replaceClass(student_id, classcode1, classcode2);
        return true;


    }
     else if (func == "removeClass") {
          size_t required_args = 1;
        if (args.size() != required_args) {
            cerr << "requires 1 argument";
            return false;
        }
        string classcode = args[0];

        int affected = removeClass(classcode);
         cout << affected << endl;
         return true;


    }
     else if (func == "toggleEdgesClosure") {
        size_t min_args = 2;
        if (args.size() < min_args || args.size()%2 == 0) {
            cerr << "requires at least 2 arguments and pairs of ids";
            return false;
        }
        try {
            int count = 0;
            try {
                count = stoi(args[0]);

            }
            catch (...) {
                return false;
            }
            if (args.size() != (1+2*count)) {
                return false;
            }
            vector<int> ids;
            try {
                for (size_t i = 1; i < args.size(); i++) {
                    ids.push_back(stoi(args[i]));
                }
                bool toggled = toggleEdgesClosure(ids);
                if (toggled) {
                    cout << "successful" << endl;
                }
                else {
                    return false;
                }
                return true;
            }
            catch (...) {
                return false;
            }
        }
        catch (...) {
            cerr << "Invalid ints for ids";
            return false;
        }

    }
     else if (func == "checkEdgeStatus") {
        size_t required_args = 2;
        if (args.size() != required_args) {
            cerr << "requires 2 arguments";
            return false;
        }
        string location_id_x = args[0];
        string location_id_y = args[1];
         int int_loc_x = 0;
         int int_loc_y = 0;
        try {
             int_loc_x = stoi(location_id_x);
             int_loc_y = stoi(location_id_y);
            cout << checkEdgeStatus(int_loc_x, int_loc_y) << endl;

        }
        catch (...) {
            cerr << "invalid int ids";
            return false;
        }

    }
     else if (func == "isConnected") {
        size_t required_args = 2;
        if (args.size() != required_args) {
            cerr << "requires 2 arguments";
            return false;
        }
        string location_id_x = args[0];
        string location_id_y = args[1];
         int int_loc_x;
         int int_loc_y ;
        try {
             int_loc_x = stoi(location_id_x);
             int_loc_y = stoi(location_id_y);

             if (isConnected(int_loc_x, int_loc_y)) {
                 cout << "successful" << endl;
             }
            else {
                cout << "unsuccessful" << endl;
            }
            return true;
        }
        catch (...) {
            cerr << "invalid int ids";
            return false;
        }
    }
    else if (func == "printShortestEdges") {
        size_t required_args = 1;
        if (args.size() != required_args) {
            cerr << "requires 1 argument";
            return false;
        }
         string student_id = args[0];
        cout << printShortestEdges(student_id) << endl;
        return true;

    }
    else if (func == "printStudentZone") {
        size_t required_args = 1;
        if (args.size() != required_args) {
            cerr << "requires 1 argument";
            return false;
        }
         string student_id = args[0];
       cout << printStudentZone(student_id) << endl;
        return true;

    }
    else if (func == "verifySchedule") {
        size_t required_args = 1;
        if (args.size() != required_args) {
            cerr << "requires 1 argument";
            return false;
        }
         string student_id = args[0];
        cout << verifySchedule(student_id) << endl;
        return true;

    }
    else {
        return false;
    }



    return true;
}

bool CampusCompass::insert(const string &student_name, const string &student_id, const int &residence_location_id, const vector<CampusCompass::Class> student_classes)
{
    Student new_student(student_name, student_id, residence_location_id, student_classes);
    students[student_id] = new_student;
    cout << "successful" << endl;

    return true;
}

bool CampusCompass::remove(const string &student_id)
{
   size_t num = students.erase(student_id);
   if (num == 1) {
    cout << "successful" << endl;
    return true;
   }
   cout << "unsuccessful" << endl;
   return false;
}

bool CampusCompass::dropClass(const string &student_id, const string &classcode)
{
    Student* stu = findStudentById(student_id);
    if (!stu) {
        cout << "unsuccessful" << endl;
            return false;
        }
    for (auto it = stu->classes.begin(); it != stu->classes.end(); it++) {
        if (it->class_code == classcode) {
            stu->classes.erase(it);
            if (stu->classes.size() == 0) {
                students.erase(student_id);
            }
            cout << "successful" << endl;
            return true;
        }
    }
        cout << "unsuccessful" << endl;
    return false;
}

bool CampusCompass::replaceClass(const string &student_id, const string &classcode_1, const string &classcode_2)
{

 Student* stu = findStudentById(student_id);
if (!stu) {
        cout << "unsuccessful" << endl;
    return false;
}
auto iter_find = classes.find(classcode_2);
//If new class doesnt exist
if (iter_find == classes.end()) {
        cout << "unsuccessful" << endl;
    return false;
}
int replacement_id = iter_find->second.location_id;
for(size_t i = 0; i < stu->classes.size(); i++) {
    if (stu->classes[i].class_code == classcode_1) {
        stu->classes[i].class_code = classcode_2;
        stu->classes[i].location_id = replacement_id;
        cout << "successful" << endl;
        return true;
    }
}
        cout << "unsuccessful" << endl;
    return false;
}

int CampusCompass::removeClass(const string &classcode)
{
    classes.erase(classcode);
    int students_affected = 0;
    for (auto& s : students) {
        for (size_t i = 0; i < s.second.classes.size(); i++) {
            if (s.second.classes[i].class_code == classcode) {
                s.second.classes.erase(s.second.classes.begin() + i);
                students_affected++;
                i--;
            }
        }
    }
    //Remove from all students
    return students_affected;
}

bool CampusCompass::toggleEdgesClosure(const vector<int> edges)
{
    if (edges.size() %2 != 0) {
        return false;
    }
    for(size_t i = 0; i < edges.size() - 1; i+=2) {
        if (closed_edges.count(make_pair(edges[i], edges[i+1])) == 0) {
            closed_edges.insert(make_pair(edges[i], edges[i+1]));
            closed_edges.insert(make_pair(edges[i+1], edges[i]));

        }
        else {
            closed_edges.erase(make_pair(edges[i], edges[i+1]));
            closed_edges.erase(make_pair(edges[i+1], edges[i]));

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
            status = "open";
         }
         else {

            status = "closed";
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
    string result = "Name: " + stu->student_name + "\n";

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

    vector<Class> sortedClasses = stu->classes;
    sort(sortedClasses.begin(), sortedClasses.end());

    for (auto& c : sortedClasses) {
        int loc = c.location_id;
        int time = INT_MAX;
        if (dist.count(loc)) {
            time = dist[loc];
        }
        if (time == INT_MAX) {
        result += c.class_code + " | Total Time: -1" + "\n";
        }
        else {
        result += c.class_code + " | Total Time: " + to_string(time) + "\n";

        }
    }

    return result;
}



string CampusCompass::printStudentZone(const string &student_id)
{
    Student* stu = findStudentById(student_id);
    if (!stu || stu->classes.empty()) {
        return "Student not found/no classes";
    }

    set<int> locations;
    locations.insert(stu->residence_location_id);
    for (auto& c : stu->classes) {
        locations.insert(c.location_id);
    }
    if (locations.size() <= 1) {
        return "Student Zone Cost For " + stu->student_name + ": 0";
    }
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    unordered_map<int, int> min_weight;
    set<int> in_zone;
    int total_weight = 0;
    size_t amt_connected = 0;
    for (int l : locations) {
        min_weight[l] = INT_MAX;
    }
    int start_node = stu->residence_location_id;
    min_weight[start_node] = 0;
    pq.push({0, start_node});
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();
        if (in_zone.count(u)) {
            continue;
        }

        if (w != min_weight[u]) {
            continue;
        }


        in_zone.insert(u);
        total_weight += w;
        if (locations.count(u)) {
            amt_connected++;
            if (amt_connected == locations.size()) {
                break;
            }
        }

        if (graph.count(u)) {
            for (auto [v, weight] : graph.at(u)) {
                if (in_zone.count(v)) {
                    continue;
                }
                if (closed_edges.count({u, v})) {
                    continue;
                }
                if (!min_weight.count(v) || weight < min_weight[v]) {
                    min_weight[v] = weight;
                    pq.push({weight, v});
                }


            }
        }
    }

    if (amt_connected < locations.size()) {
        return "Student Zone Cost For " + stu->student_name + ": disconnected";

    }
        string result = "Student Zone Cost For " + stu->student_name + ": " + to_string(total_weight);
    return result;

    //MST, Prims

}

CampusCompass::Student* CampusCompass::findStudentById(const string &id) {
    auto it = students.find(id);
    if (it != students.end()) {
        return &it->second;
    }
    return nullptr;
}

bool CampusCompass::edgeExists(const int& x, const int& y) {
    if (!graph.count(x) || !graph.count(y)) {
        return false;
    }
    for (auto& p : graph[x]) {
        if (p.first == y)  {
            return true;
        }
    }
    return false;
}

int CampusCompass::getShortestTime(const int &start_loc, const int &end_loc)
{
    if (start_loc == end_loc) {
        return 0;
    }
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    unordered_map<int,int> dist;
    for (auto& [id, adj] : graph) {
        dist[id] = INT_MAX;
    }
    dist[start_loc] = 0;
    dist[end_loc] = INT_MAX;
    pq.push({0, start_loc});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (u == end_loc) {
            return d;
        }
        if (d > dist[u]) {
            continue;
        }
        if (graph.count(u)) {
            for (auto [v, weight] : graph.at(u)) {
                if (closed_edges.count({u, v})) {
                    continue;
                }
                if (dist.count(v) && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }
    return INT_MAX;
}

// EXTRA CREDIT
int getTimeGap(const string& start_next, const string& end_curr) {
    //00:00
   try {
       int next_hr = stoi(start_next.substr(0,2));
       int next_min = stoi(start_next.substr(3,2));

       int curr_hr = stoi(end_curr.substr(0,2));
       int curr_min = stoi(end_curr.substr(3,2));

       int next_total = (next_hr * 60) + next_min;
       int curr_total = (curr_hr * 60) + curr_min;
       return next_total - curr_total;
   }
    catch (...) {
        return 0;
    }
}
bool compByTime(const CampusCompass::Class &c1, const CampusCompass::Class &c2) {
    return c1.start_time < c2.start_time;
}
string CampusCompass::verifySchedule(const string &student_id)
{
    Student* stu = findStudentById(student_id);
        if (!stu || stu->classes.size() <= 1) {
            return "unsuccessful";
        }
    vector<Class> schedule = stu->classes;
    sort(schedule.begin(), schedule.end(), compByTime);

        string result = "Schedule Check for " + stu->student_name + "\n";


        for (size_t i = 0; i < schedule.size() - 1; i++) {

            int start_loc = schedule[i].location_id;
            int end_loc = schedule[i+1].location_id;
            int travel_time = getShortestTime(start_loc, end_loc);
            string start_next = schedule[i+1].start_time;
            string end_curr = schedule[i].end_time;
            int time_gap = getTimeGap(start_next, end_curr);

            string trip_desc = schedule[i].class_code + " - " + schedule[i+1].class_code;
            string status_message;



            if (travel_time == INT_MAX) {
                status_message = "Unreachable";
            }
            else if (travel_time <= time_gap) {
                status_message = "Can make it!";
            }
            else {
                status_message = "Cannot make it!";
            }
            result += trip_desc + " " + status_message + "\n";
        }

    return result;
}

