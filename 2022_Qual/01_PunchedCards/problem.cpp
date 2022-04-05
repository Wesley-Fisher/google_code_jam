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
  int R;
  int C;
};

struct CaseSolution {
  std::vector<std::string> vsLines;
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

    in >> p.R;
    in >> p.C;

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << std::endl;

    for(int i=0; i < solution.vsLines.size(); i++)
    {
      out << solution.vsLines[i];
    }
}

std::string CreateRow(int r, int C)
{
  std::string sRow;
  bool bDividerRow = (r % 2 == 0);
  
  int c0 = 0;
  if (r < 2)
  {
    c0 = 1;
    sRow += "..";
  }

  for(int c=c0; c<C; c++)
  {
    if(bDividerRow)
    {
      sRow += "+-";
    }
    else
    {
      sRow += "|.";
    }
  }

    if(bDividerRow)
    {
      sRow += "+\n";
    }
    else
    {
      sRow += "|\n";
    }

    return sRow;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    for(int r = 0; r < 2*details.R+1; r++)
    {
      std::string sLine = CreateRow(r, details.C);
      solution.vsLines.push_back(sLine);
    }

    return solution;
}

int countPeriods(CaseSolution solution)
{
  int iPeriods = 0;

  for(int i=0; i<solution.vsLines.size(); i++)
  {
    for(int j=0; j<solution.vsLines[i].size(); j++)
    {
      char c = solution.vsLines[i][j];
      if (c == '.')
      {
        iPeriods++;
      }
    }
  }
  return iPeriods;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Testing

#ifdef TEST
TEST(GTestTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}

TEST(GTestTest, NumRows) {
  CaseDetails details;
  CaseSolution solution;

  details.R = 3;
  details.C = 4;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines.size(), 7);

  details.R = 2;
  details.C = 2;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines.size(), 5);

  details.R = 2;
  details.C = 3;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines.size(), 5);
}

TEST(GTestTest, NumCols) {
  CaseDetails details;
  CaseSolution solution;

  details.R = 3;
  details.C = 4;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines[0].size(), 9+1);
  EXPECT_EQ(solution.vsLines[1].size(), 9+1);
  EXPECT_EQ(solution.vsLines[2].size(), 9+1);
  EXPECT_EQ(solution.vsLines[3].size(), 9+1);

  details.R = 2;
  details.C = 2;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines[0].size(), 5+1);
  EXPECT_EQ(solution.vsLines[1].size(), 5+1);
  EXPECT_EQ(solution.vsLines[2].size(), 5+1);
  EXPECT_EQ(solution.vsLines[3].size(), 5+1);

  details.R = 2;
  details.C = 3;
  solution = SolveCase(details);
  EXPECT_EQ(solution.vsLines[0].size(), 7+1);
  EXPECT_EQ(solution.vsLines[1].size(), 7+1);
  EXPECT_EQ(solution.vsLines[2].size(), 7+1);
  EXPECT_EQ(solution.vsLines[3].size(), 7+1);
}

TEST(GTestTest, NumPeriods) {
  CaseDetails details;
  CaseSolution solution;

  details.R = 3;
  details.C = 4;
  solution = SolveCase(details);
  EXPECT_EQ(countPeriods(solution), details.R*details.C+3);

  details.R = 2;
  details.C = 2;
  solution = SolveCase(details);
  EXPECT_EQ(countPeriods(solution), details.R*details.C+3);

  details.R = 2;
  details.C = 3;
  solution = SolveCase(details);
  EXPECT_EQ(countPeriods(solution), details.R*details.C+3);
}
#endif
