/*

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "CampusCompass.h"

using namespace std;
const string edges_file = "../data/edges.csv";
const string classes_file = "../data/classes.csv";


TEST_CASE("Shortest Edges Case", "[graph]") {
  CampusCompass c;
  c.ParseCSV(edges_file, classes_file);
  c.ParseCommand(R"(insert "Student" 12345678 22 1 COP3502)");
  string output = c.printShortestEdges("12345678");
  REQUIRE(output.find("Total Time: 1") != string::npos);

  c.ParseCommand("toggleEdgesClosure 22 23");
  string output2 = c.printShortestEdges("12345678");
  REQUIRE(output2.find("Total Time: 1") == string::npos);

  c.ParseCommand("toggleEdgesClosure 22 21");
  c.ParseCommand("toggleEdgesClosure 22 11");
  string output3 = c.printShortestEdges("12345678");
  REQUIRE(output3.find("Total Time: -1") != string::npos);
}

TEST_CASE("Incorrect Commands", "[incorrect]") {
  CampusCompass c;
  bool loaded = c.ParseCSV(edges_file, classes_file);
  REQUIRE(loaded == true);

  SECTION("Incorrect Insert Name") {
  string input = R"(insert "Stud3nt" 10000001 1 1 COP3502)"; //Wrong Command #1
  bool expectedOutput = false;
  bool actualOutput = c.ParseCommand(input);
  REQUIRE(actualOutput == expectedOutput);
  };

  SECTION("Incorrect Insert ID") {
  string input = R"(insert "Student" 100000001 1 1 COP3502)"; //Too long | Wrong Command #2
  string input2 = R"(insert "Student" 1000d001 1 1 COP3502)"; //Has letter | Wrong Command #3

  

  REQUIRE(c.ParseCommand(input) == false);
  REQUIRE(c.ParseCommand(input2) == false);

  };

  SECTION("Incorrect Insert Name Without Quotes") {
      string input = R"(insert Named 10000001 1 1 COP3502)"; //Wrong Command #4
      REQUIRE(c.ParseCommand(input) == false);

  }

  SECTION("Toggle Edges, Wrong Number Arguments") {
    string input = R"(toggleEdgesClosure 1 2 3)"; //Wrong Command #5
    REQUIRE(c.ParseCommand(input) == false);

  }
}

TEST_CASE("Edge Cases", "[edge]") {
  CampusCompass c;
  c.ParseCSV(edges_file, classes_file);


  SECTION("Remove nonexistent student") {
    string input = R"(remove 10000001)"; //nonexistent student | Edge Case #1
    REQUIRE(c.ParseCommand(input) == false);

  };

  SECTION("Remove nonexistent class") {
    string input = R"(remove COP4622)"; //nonexistent class | Edge Case #2

    
    REQUIRE(c.ParseCommand(input) == false);
  };
    SECTION("isConnected with incorrect ID") {
    string input = R"(isConnected 901 100)"; //nonexistent location ids | Edge Case #3

    REQUIRE(c.ParseCommand(input) == false);

  };

 
}
TEST_CASE("Function Tests", "[function]") {
  CampusCompass c;
  bool loaded = c.ParseCSV(edges_file, classes_file);
  REQUIRE(loaded == true);
  c.ParseCommand(R"(insert "StudentOne" 10000001 1 3 COP3502 MAC2311 CNT4007)"); //create student
  c.ParseCommand(R"(insert "StudentTwo" 10000002 1 3 COP3502 MAC2311 CNT4007)"); //create student


  SECTION("Drop Class") {
    bool actualOutput = c.ParseCommand(R"(dropClass 10000001 MAC2311)"); 
    REQUIRE(actualOutput == true);
  };

  SECTION("Remove Class") {
   bool actualOutput = c.ParseCommand(R"(removeClass COP3502)"); 
    REQUIRE(actualOutput == true);
  };

  SECTION("Remove Student") {
    bool actualOutput = c.ParseCommand(R"(remove 10000001)"); 

    REQUIRE(actualOutput == true);
  };

  SECTION("Replace Class") {
    bool actualOutput = c.ParseCommand(R"(replaceClass 10000001 CNT4007 MAC2312)"); 

    REQUIRE(actualOutput == true);
  };

  
}

*/