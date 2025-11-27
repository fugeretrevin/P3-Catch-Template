#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;
/*Submit a file containing the following five tests using the Catch Framework:

Test at least five incorrect commands. Ex: insert "A11y" 45679999 1 1 COP3530 [1 points]
Test at least three edge cases for various functions. Ex: removing a student that doesn’t exist [1 points]
Test your dropClass, removeClass, remove, and replaceClass commands [1 point]
Test your printShortestEdges command in a case where a student can reach a class, then one or more edges turn off, and then they cannot reach that class. (Hint: the visualizer tool can help with this a lot). [2 points]*/
// the syntax for defining a test is below. It is important for the name to be
// unique, but you can group multiple tests with [tags]. A test can have
// [multiple][tags] using that syntax.
TEST_CASE("Shortest Edges Case", "test") {
  // instantiate any class members that you need to test here
  int one = 1;

  // anything that evaluates to false in a REQUIRE block will result in a
  // failing test
  REQUIRE(one == 0); // fix me!

  // all REQUIRE blocks must evaluate to true for the whole test to pass
  REQUIRE(false); // also fix me!
}

TEST_CASE("Incorrect Commands", "test") {
  CampusCompass c;

  SECTION("Incorrect Insert Name") {
  string input = R"(insert "Stud3nt" 10000001 1 1 COP3502)";
  bool expectedOutput = false;
  bool actualOutput = c.parseCommand(input);
  REQUIRE(actualOutput == expectedOutput);
  };

  SECTION("Incorrect Insert ID") {
  string input = R"(insert "Student" 100000001 1 1 COP3502)"; //Too long
  string input2 = R"(insert "Student" 1000d001 1 1 COP3502)"; //Has letter

  bool expectedOutput = false;
  bool actualOutput1 = c.parseCommand(input);
  bool actualOutput2 = c.parseCommand(input2);

  REQUIRE(actualOutput1 == expectedOutput);
  REQUIRE(actualOutput2 == expectedOutput);

  };
}

TEST_CASE("Edge Cases", "test") {
  CampusCompass c;

  SECTION("Remove nonexistent student") {
    string input = R"(remove 10000001)"; //nonexistent student

    bool actualOutput c.parseCommand(input);
    bool expectedOutput = false;
    REQUIRE(actualOutput == expectedOutput);
  };

  SECTION("Remove nonexistent class") {
    string input = R"(remove COP4622)"; //nonexistent class

    bool actualOutput c.parseCommand(input);
    bool expectedOutput = false;
    REQUIRE(actualOutput == expectedOutput);
  };
    SECTION("isConnected with incorrect ID") {
    string input = R"(isConnected 901 100)"; //nonexistent location ids

    bool actualOutput c.parseCommand(input);
    bool expectedOutput = false;
    REQUIRE(actualOutput == expectedOutput);
  };

  // each section runs the setup code independently to ensure that they don't
  // affect each other
}
TEST_CASE("Function Tests", "test") {
  // you can also use "sections" to share setup code between tests, for example:
  CampusCompass c;
  c.parseCommand(R"(insert "Student1" 10000001 1 3 COP3502 MAC2311 CNT4007)"); //create student
  c.parseCommand(R"(insert "Student2" 10000002 1 3 COP3502 MAC2311 CNT4007)"); //create student


  SECTION("Drop Class") {
    bool actualOutput =   c.parseCommand(R"(dropClass 10000001 MAC2311)"); 
    REQUIRE(actualOutput == true);
  };

  SECTION("Remove Class") {
   bool actualOutput =   c.parseCommand(R"(removeClass COP3502)"); 
    REQUIRE(actualOutput == true);
  };

  SECTION("Remove") {
    bool actualOutput =   c.parseCommand(R"(removeStudent 10000001)"); 

    REQUIRE(actualOutput == true);
  };

  SECTION("Replace Class") {
    bool actualOutput =   c.parseCommand(R"(replaceClass 10000001 CNT4007 MAC2312)"); 

    REQUIRE(actualOutput == true);
  };

  // each section runs the setup code independently to ensure that they don't
  // affect each other
}

// You must write 5 unique, meaningful tests for credit on the testing section
// of this project!

// See the following for an example of how to easily test your output.
// Note that while this works, I recommend also creating plenty of unit tests for particular functions within your code.
// This pattern should only be used for final, end-to-end testing.

// This uses C++ "raw strings" and assumes your CampusCompass outputs a string with
//   the same thing you print.
TEST_CASE("Example CampusCompass Output Test", "[flag]") {
  // the following is a "raw string" - you can write the exact input (without
  //   any indentation!) and it should work as expected
  // this is based on the input and output of the first public test case
  string input = R"(6
insert "Student A" 10000001 1 1 COP3502
insert "Student B" 10000002 1 1 COP3502
insert "Student C" 10000003 1 2 COP3502 MAC2311
dropClass 10000001 COP3502
remove 10000001
removeClass COP3502
)";

  string expectedOutput = R"(successful
successful
successful
successful
unsuccessful
2
)";

  string actualOutput;

  // somehow pass your input into your CampusCompass and parse it to call the
  // correct functions, for example:
  /*
  CampusCompass c;
  c.parseInput(input)
  // this would be some function that sends the output from your class into a string for use in testing
  actualOutput = c.getStringRepresentation()
  */

  REQUIRE(actualOutput == expectedOutput);
}
