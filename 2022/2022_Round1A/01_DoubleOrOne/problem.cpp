#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

#include <fstream>
#include <iostream>
#include <iomanip>

#include <vector>
#include <string>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Define Utilities
#define iow(w) std::setw(w)
#define iop(p) std::setprecision(p)
#define iowp(w,p) iow(w) << iop(p)

#ifdef LOCAL
  #define LOG(...) std::cout << __VA_ARGS__ << std::endl
#else
  #define LOG(...) 
#endif

bool OpenTest(std::string& name, std::ifstream& fin)
{
  std::string filename = "../tests/" + name + ".txt";
  fin.open(filename);
  return fin.is_open();
}


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
  std::string s;
};

struct CaseSolution {
  std::string s;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main(int argc, char** argv)
{
  #ifdef LOCAL
    std::cout << "Starting" << std::endl;

    std::string file = "tests";
    if(argc == 2)
    {
      file = std::string(argv[1]);
    }

    std::ifstream fin;
    if(!OpenTest(file, fin))
    {
      std::cout << "Create a " << file << ".txt file" << std::endl;
      return 0;
    }

    std::string sOut = "../tests/" + file + "_results.txt";
    std::cout << "Writing to: " << sOut << std::endl;
    std::ofstream fout;
    fout.open(sOut);

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
    in >> p.s;

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.s << std::endl;
}

int getLastEqualChar(std::string s, int i)
{
  if (i == s.length() - 1)
  {
    return i;
  }

  int j = i;

  while (s[j] == s[i] && j < s.length()-1)
  {
    j++;
  }
  j--;
  return j;
}

int getMultiplier(std::string s, int j)
{
  if (j == s.length() - 1)
  {
    return 1;
  }

  if (s[j] < s[j+1])
  {
    return 2;
  }

  return 1;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    std::stringstream ss;
    int i = 0;

    while(i < details.s.length() - 1)
    {
      int j = getLastEqualChar(details.s, i);

      int k = getMultiplier(details.s, j);
      
      for(int m=0; m<j-i+1; m++)
      {
        for (int n=0; n<k; n++)
        {
          ss << details.s[i];
        }
      }

      i = j + 1;
    }

    ss << details.s[details.s.length()-1];
    solution.s = ss.str();

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

TEST(GTestTest, TestGiven1) {
  CaseDetails details;
  details.s = "PEEL";
  CaseSolution solution = SolveCase(details);

  EXPECT_EQ(solution.s, "PEEEEL");
}

TEST(GTestTest, TestGiven2) {
  CaseDetails details;
  details.s = "AAAAAAAAAA";
  CaseSolution solution = SolveCase(details);

  EXPECT_EQ(solution.s, "AAAAAAAAAA");
}

TEST(GTestTest, TestGiven3) {
  CaseDetails details;
  details.s = "CODEJAMDAY";
  CaseSolution solution = SolveCase(details);

  EXPECT_EQ(solution.s, "CCODDEEJAAMDAAY");
}

TEST(GTestTest, TestLastEq) {
  EXPECT_EQ(getLastEqualChar("CODEJAMDAY", 0), 0);
  EXPECT_EQ(getLastEqualChar("CODEJAMDAY", 1), 1);
  EXPECT_EQ(getLastEqualChar("CODEJAMDAY", 8), 8);
  EXPECT_EQ(getLastEqualChar("CODEJAMDAY", 9), 9);

  EXPECT_EQ(getLastEqualChar("ABBCCCDDDDE", 0), 0);
  EXPECT_EQ(getLastEqualChar("ABBCCCDDDDE", 1), 2);
  EXPECT_EQ(getLastEqualChar("ABBCCCDDDDE", 3), 5);
  EXPECT_EQ(getLastEqualChar("ABBCCCDDDDE", 6), 9);
  EXPECT_EQ(getLastEqualChar("ABBCCCDDDDE", 10), 10);
}

TEST(GTestTest, TestMult) {
  EXPECT_EQ(getMultiplier("ABCDCBA", 0), 2);
  EXPECT_EQ(getMultiplier("ABCDCBA", 1), 2);
  EXPECT_EQ(getMultiplier("ABCDCBA", 2), 2);
  EXPECT_EQ(getMultiplier("ABCDCBA", 3), 1);
  EXPECT_EQ(getMultiplier("ABCDCBA", 4), 1);
  EXPECT_EQ(getMultiplier("ABCDCBA", 5), 1);
  EXPECT_EQ(getMultiplier("ABCDCBA", 6), 1);
}

TEST(GTestTest, TestAnotherA) {
  CaseDetails details;
  details.s = "CODEEJAMMDAAAY";
  CaseSolution solution = SolveCase(details);

  EXPECT_EQ(solution.s, "CCODDEEEEJAAMMDAAAAAAY");
}

TEST(GTestTest, TestAnotherB) {
  CaseDetails details;
  details.s = "C";
  CaseSolution solution = SolveCase(details);

  EXPECT_EQ(solution.s, "C");
}
#endif
