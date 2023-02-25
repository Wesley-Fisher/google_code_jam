#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

#include <fstream>
#include <iostream>
#include <iomanip>

#include <vector>
#include <deque>
#include <string>
#include <sstream>

#include "main_standard.h"

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
int main(int argc, char** argv)
{
  #ifdef ANALYSIS
    return main_analysis(argc, argv);
  #elif defined LOCAL
    return main_local(argc, argv);
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
        CaseSolution solution = SolveCase(case_details, std::to_string(i+1));
        OutputSolution(solution, out, i+1);
    }
}

void AnalyzeProblem(std::istream& in, std::istream& answers) {
    int num_problems = 0;
    in >> num_problems;

    LOG("ANALYZING " << num_problems << " problems");

    for(int i=0; i<num_problems; i++) {
        CaseDetails case_details = ReadCaseDetails(in);
        CaseSolution solution = SolveCase(case_details, std::to_string(i+1));
        
        std::string ans = CaseOutput(solution, i+1);
        ans.pop_back(); // Remove newline for logging.comparing
        
        std::string key;
        std::getline(answers, key);
        
        if(!OutputCompare(ans, key))
        {
          LOG("MISMATCH (mine, key)");
          LOG(ans);
          LOG(key);
          LOG("");
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

std::string CaseOutput(CaseSolution solution, int case_num)
{
  std::stringstream ss;
  //ss << "Case #" << case_num << ": " << std::endl;
  return std::string(ss.str());
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << CaseOutput(solution, case_num);
}

CaseSolution SolveCase(CaseDetails details, std::string sCase) {
    LOG("SolveCase: " << sCase);
    CaseSolution solution;

    return solution;
}

bool OutputCompare(std::string mine, std::string ans)
{
  return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Testing

#ifdef TEST
TEST(GTestTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}

TEST(GTestTest, SolveCaseDummy) {
  CaseDetails det;
  CaseSolution sol = SolveCase(det, "dummy");
}
#endif
