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
  long int N;
  std::vector<unsigned long int> vFun;
  std::vector<int> pointsTo;
  std::vector<std::vector<int> > pointedToBy;
};

struct CaseSolution {
  unsigned long int fun;
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

    in >> p.N;
    p.vFun.resize(p.N+1);
    p.pointsTo.resize(p.N+1);
    p.pointedToBy.resize(p.N+1);

    p.vFun[0] = 0;

    for (int i=1;i<=p.N; i++)
    {
      unsigned long int fun;
      in >> fun;
      p.vFun[i] = fun;
    }

    for (int i=1;i<=p.N; i++)
    {
      int pt;
      in >> pt;
      p.pointsTo[i] = pt;
      p.pointedToBy[pt].push_back(i);
    }

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.fun << std::endl;
}

void findMinIndexValue(const std::vector<unsigned long int> viValues, int& idx, unsigned long int& value)
{
  idx = 0;
  value = viValues[0];

  for(int i=1; i< viValues.size(); i++)
  {
    if(viValues[i] < value)
    {
      idx = i;
      value = viValues[i];
    }
  }
}

std::string vec2string(const std::vector<unsigned long int> vValues)
{
  #ifdef LOCAL
    std::stringstream ss;
    ss << "{" << std::to_string(vValues[0]);
    for(int i=1; i<vValues.size(); i++)
    {
      ss << "," << std::to_string(vValues[i]);
    }
    ss << "}";
    return ss.str();
  #else
    return "";
  #endif
}

std::vector<unsigned long int> TreeRecursion(int i0, const CaseDetails& details)
{
  LOG("Looking at node: " << i0);
  // Leaves
  if(details.pointedToBy[i0].size() ==0)
  {
    unsigned long int f = details.vFun[i0];
    LOG("  Node " << i0 << " is leaf with " << f);
    std::vector<unsigned long int> ret = {f, 0};
    return ret;
  }

  // Straight Chains
  if(details.pointedToBy[i0].size() == 1)
  {
    unsigned long int f = details.vFun[i0];
    int iNext = details.pointedToBy[i0][0];
    std::vector<unsigned long int> prevFun = TreeRecursion(iNext, details);

    prevFun[0] = std::max(f, prevFun[0]);

    LOG("  Node " << i0 << " is " << f << " -> " << vec2string(prevFun));
    return prevFun;
  }

  // Choice -- Choose Lowest
  std::vector<int> choices = details.pointedToBy[i0];
  std::vector<unsigned long int> funs;

  unsigned long fixed = 0;

  for(int i=0; i<choices.size(); i++)
  {
    int iNext = choices[i];
    std::vector<unsigned long int> f = TreeRecursion(iNext, details);
    funs.push_back(f[0]);
    fixed += f[1];
  }

  unsigned long int fMin = 0;
  int iMin = -1;
  findMinIndexValue(funs, iMin, fMin);


  unsigned long open;
  std::vector<unsigned long int> ret = {0, fixed};
  for(int i=0; i<funs.size(); i++)
  {
    if (i == iMin)
    {
      ret[0] = std::max(details.vFun[i0], fMin);
    }
    else
    {
      ret[1] += funs[i];
    }
  }

  #ifdef LOCAL
    LOG("  Node " << i0 << " is " << details.vFun[i0] << " with " << vec2string(funs) << "->" << vec2string(ret));
  #endif

  return ret;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    LOG(std::endl << std::endl << std::endl);
    std::vector<unsigned long int> vals = TreeRecursion(0, details);
    solution.fun = 0;
    for(int i=0; i<vals.size(); i++)
    {
      solution.fun += vals[i];
    }

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
