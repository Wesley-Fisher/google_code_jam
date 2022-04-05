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
struct ink
{
  int c;
  int m;
  int y;
  int k;
};

struct CaseDetails {
  ink a;
  ink b;
  ink c;
};

struct CaseSolution {
  bool bPossible;
  ink choice;
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

    in >> p.a.c;
    in >> p.a.m;
    in >> p.a.y;
    in >> p.a.k;

    in >> p.b.c;
    in >> p.b.m;
    in >> p.b.y;
    in >> p.b.k;

    in >> p.c.c;
    in >> p.c.m;
    in >> p.c.y;
    in >> p.c.k;

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": ";

    if(solution.bPossible)
    {
      out << solution.choice.c << " " << solution.choice.m << " "
          << solution.choice.y << " " << solution.choice.k << std::endl;
    }
    else
    {
      out << "IMPOSSIBLE" << std::endl;
    }
}

void subtractSurplus(int& source, int& surplus)
{
  if(source == 0 || surplus <= 0)
  {
    return;
  }

  if (source >= surplus)
  {
    source -= surplus;
    surplus = 0;
  }
  else
  {
    surplus -= source;
    source = 0;
  }
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    int c = std::min(std::min(details.a.c, details.b.c), details.c.c);
    int m = std::min(std::min(details.a.m, details.b.m), details.c.m);
    int y = std::min(std::min(details.a.y, details.b.y), details.c.y);
    int k = std::min(std::min(details.a.k, details.b.k), details.c.k);

    int surplus = c+m+y+k - 1000000;
    LOG("Min Ink: " << c << ", " << m << ", " << y << ", " << k);
    LOG("Surplus: " << surplus);

    if (surplus < 0)
    {
      solution.bPossible = false;
      return solution;
    }
    solution.bPossible = true;

    
    subtractSurplus(c, surplus);
    subtractSurplus(m, surplus);
    subtractSurplus(y, surplus);
    subtractSurplus(k, surplus);

    solution.choice.c = c;
    solution.choice.m = m;
    solution.choice.y = y;
    solution.choice.k = k;

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

TEST(GTestTest, SubtractSurplus) {
  int source;
  int surplus;

  source = 10;
  surplus = 2;
  subtractSurplus(source, surplus);
  EXPECT_EQ(source, 8);
  EXPECT_EQ(surplus, 0);

  source = 10;
  surplus = 12;
  subtractSurplus(source, surplus);
  EXPECT_EQ(source, 0);
  EXPECT_EQ(surplus, 2);

  source = 0;
  surplus = 12;
  subtractSurplus(source, surplus);
  EXPECT_EQ(source, 0);
  EXPECT_EQ(surplus, 12);

  source = 10;
  surplus = 0;
  subtractSurplus(source, surplus);
  EXPECT_EQ(source, 10);
  EXPECT_EQ(surplus, 0);
}
#endif
