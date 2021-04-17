#include <fstream>
#include <iostream>

#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

// Includes for problem here


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Define common structures and functions
struct CaseDetails;
struct CaseSolution;

void SolveProblem(std::istream& in, std::ostream& out);
CaseDetails ReadCaseDetails(std::istream& in);
CaseSolution SolveCase(CaseDetails details);
void OutputSolution(CaseSolution solution, std::ostream& out, int case_num);



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specific Structs
struct CaseDetails {
};

struct CaseSolution {
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main()
{
  #ifdef LOCAL
    std::cout << "Starting" << std::endl;
    std::ifstream fin;
    fin.open("tests.txt");
    if(!fin.is_open())
    {
      std::cout << "Create a tests.txt file" << std::endl;
      return 0;
    }
    std::ofstream fout;
    fout.open("results.txt");
    SolveProblem(fin, fout);
    std::cout << "Finished" << std::endl;
  #else
    SolveProblem(std::cin, std::cout);
  #endif

  return 0;
}
#endif

void SolveProblem(std::istream& in, std::ostream& out) {
    int num_problems = 0;
    in >> num_problems;

    for(int i=0; i<num_problems; i++) {
        CaseDetails case_details = ReadCaseDetails(in);
        CaseSolution solution = SolveCase(case_details);
        OutputSolution(solution, out, i+1);
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

CaseDetails ReadCaseDetails(std::istream& in) {
    CaseDetails p;

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    //out << "Case #" << case_num << ": " << std::endl;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    return solution;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Testing

#ifdef TEST
TEST(GTestTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}
#endif
