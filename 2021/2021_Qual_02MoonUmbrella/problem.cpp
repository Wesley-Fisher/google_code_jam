#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

// Includes for problem here



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
  int x;
  int y;
  std::string s;
};

struct CaseSolution {
  int cost;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main(int argc, char** argv)
{
  #ifdef LOCAL
    std::cout << "Starting" << std::endl;
    std::ifstream fin;

    std::string file = "tests";
    if(argc == 2)
    {
      file = std::string(argv[1]);
    }

    std::string filename = "tests/" + file + ".txt";
    fin.open(filename);
    if(!fin.is_open())
    {
      std::cout << "Create a tests.txt file" << std::endl;
      return 0;
    }

    std::ofstream fout;
    fout.open("tests/" + file + "_results.txt");

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
    in >> p.x;
    in >> p.y;
    in >> p.s;

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.cost << std::endl;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;
    solution.cost = 0;

    if(details.s.size() == 1)
    {
      return solution;
    }

    LOG("Processing [" << details.s << "]");

    std::string s = details.s;
    int N = s.size();
    int costWithC[N] = {0};
    int costWithJ[N] = {0};

    for(int i=1; i<N; i++)
    {
      if(s[i-1] == 'C')
      {
        if(s[i] == 'C')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1])  + 0;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + 0;
        }
        else if(s[i] == 'J')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1])+ details.x;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + details.x;
        }
        else if (s[i] == '?')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1]) + 0;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + details.x;
        }
      }
      else if(s[i-1] == 'J')
      {
        if(s[i] == 'C')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1]) + details.y;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + details.y;
        }
        else if(s[i] == 'J')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1]) + 0;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + 0;
        }
        else if (s[i] == '?')
        {
          costWithC[i] += std::min(costWithC[i-1],costWithJ[i-1]) + details.y;
          costWithJ[i] += std::min(costWithC[i-1],costWithJ[i-1]) + 0;
        }
      }
      else if (s[i-1] == '?')
      {
        if(s[i] == 'C')
        {
          costWithC[i] += costWithC[i-1] + 0;
          costWithJ[i] += costWithJ[i-1] + details.y;
        }
        else if(s[i] == 'J')
        {
          costWithC[i] += costWithC[i-1] + details.x;
          costWithJ[i] += costWithJ[i-1] + 0;
        }
        else if (s[i] == '?')
        {
          costWithC[i] += std::min(costWithC[i-1] + 0, costWithJ[i-1] + details.y);
          costWithJ[i] += std::min(costWithC[i-1] + details.x, costWithJ[i-1] + 0);
        }
      }
    }

    solution.cost = std::min(costWithC[N-1], costWithJ[N-1]);
    return solution;

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
