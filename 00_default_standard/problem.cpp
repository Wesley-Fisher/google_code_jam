#ifdef TEST
#define BOOST_TEST_MODULE MyTests
#include <boost/test/included/unit_test.hpp>
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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

struct CaseDetails {
};

struct CaseSolution {
};

CaseDetails* ReadCaseDetails(std::istream& in)
{
  CaseDetails* details = new CaseDetails();
  return details;
}

std::string CaseOutput(CaseSolution* solution, int case_num)
{
  std::stringstream ss;
  //ss << "Case #" << case_num << ": " << std::endl;
  return std::string(ss.str());
}

CaseSolution* SolveCase(CaseDetails* details, std::string sCase) {
    LOG("SolveCase: " << sCase);
    CaseSolution* solution = new CaseSolution();

    return solution;
}

bool OutputCompare(std::string mine, std::string ans)
{
  return false;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// For Incomplete Type

void delete_impl(CaseDetails* details, CaseSolution* solution)
{
  delete details;
  delete solution;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Testing

#ifdef TEST
BOOST_AUTO_TEST_CASE(BasicAssertions) {
  BOOST_CHECK_EQUAL("hello", "world");
  BOOST_CHECK_EQUAL(7 * 6, 42);
}

BOOST_AUTO_TEST_CASE(SolveCaseDummy) {
  CaseDetails* det = new CaseDetails();
  CaseSolution* sol = SolveCase(det, "dummy");
  delete det;
  delete sol;
}
#endif
