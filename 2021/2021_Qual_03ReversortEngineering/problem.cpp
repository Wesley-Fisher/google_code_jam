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
#include <set>
#include <map>


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
  std::string filename = "tests/" + name + ".txt";
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
  int N;
  int C;
};

struct CaseSolution {
  std::vector<int> nums;
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
    in >> p.N;
    in >> p.C;
    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": ";

    if(solution.nums.size() == 0)
    {
      out << "IMPOSSIBLE" << std::endl;
    }
    else
    {
      for (int i=0; i<solution.nums.size(); i++)
      {
        out << solution.nums[i];
        if(i != solution.nums.size()-1)
        {
          out << " ";
        }
      }
      out << std::endl;
    }
}

int GetMaxCost(int nLeft)
{
  if(nLeft == 1)
  {
    return 0;
  }

  int cost = 0;
  for(int i=nLeft; i>1; i--)
  {
    cost += i;
  }
  return cost;
}

std::set<std::pair< int, int > > setFailed;
std::map<std::pair<int,int>, std::vector<int>> mapSuccess;

// Possible Improvements
// - Just find one cost that will work
// -- Can always predict the max and min costs possible given nLeft
std::vector<int> GetCostList(int nLeft, int costToGo)
{
  std::vector<int> list;
  if( nLeft == 1)
  {
    if(costToGo == 0)
    {
      list.push_back(0);
    }
    return list;
  }

  std::pair<int,int> p = std::pair<int,int>(nLeft, costToGo);
  if(setFailed.find(p) != setFailed.end())
  {
    return list;
  }
  if(mapSuccess.find(p) != mapSuccess.end())
  {
    return mapSuccess.at(p);
  }

  if(costToGo == nLeft - 1)
  {
    for(int i=nLeft; i>1; i--)
    {
      list.push_back(1);
    }
    list.push_back(0);
    return list;
  }

  int maxCostAfter = GetMaxCost(nLeft-1);
  for(int i=nLeft; i>=1; i--)
  {
    int costLeft = costToGo - i;
    if (costLeft <= maxCostAfter)
    {
      std::vector<int> remCosts = GetCostList(nLeft-1, costToGo-i);
      if(remCosts.size() > 0)
      {
        list.push_back(i);
        for(int j=0; j<remCosts.size(); j++)
        {
          list.push_back(remCosts[j]);
        }

        if(mapSuccess.find(p) == mapSuccess.end())
        {
          mapSuccess.insert(std::pair<std::pair<int,int>, std::vector<int> >(p, list));
        }
        return list;
      }
    }
  }
  setFailed.insert(p);
  return list;
}

void ReverseWithCost(std::vector<int>& nums, int iStart, int cost)
{
  int i = iStart;
  int j = i + cost - 1;

  while(i < j)
  {
    int tmp = nums[i];
    nums[i] = nums[j];
    nums[j] = tmp;
    i++;
    j--;
  }
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    std::vector<int> costs = GetCostList(details.N, details.C);

    if(costs.size() == 0)
    {
      return solution;
    }

    std::vector<int> nums;
    for(int i=1; i<=details.N; i++)
    {
      nums.push_back(i);
    }

    for(int i=details.N; i>=1; i--)
    {
      ReverseWithCost(nums, i-1, costs[i-1]);
    }
    solution.nums = nums;

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

TEST(GTestGetCostList, BasicLists) {
  EXPECT_EQ(GetCostList(2, 2).size() > 0, true);
}

TEST(GTestGetCostList, MyExperimentation) {
  std::vector<int> list = GetCostList(7, 19);
  std::vector<int> exp = {7, 6, 3, 1, 1, 1,0};
  EXPECT_EQ(list.size() > 0, true);
  EXPECT_EQ(list.size(), exp.size());
  for(int i=0; i<list.size(); i++)
  {
    EXPECT_EQ(list[i], exp[i]);
  }
}

TEST(GTestGetCostList, MyExperimentation2) {
  std::vector<int> list = GetCostList(7, 20);
  std::vector<int> exp = {7, 6, 4, 1, 1, 1,0};
  EXPECT_EQ(list.size() > 0, true);
  EXPECT_EQ(list.size(), exp.size());
  for(int i=0; i<list.size(); i++)
  {
    EXPECT_EQ(list[i], exp[i]);
  }
}

TEST(GTestGetCostList, FullExploration7) {
  std::vector<int> list = GetCostList(7, 200000);
  EXPECT_EQ(list.size(), 0);
}

TEST(GTestGetCostList, FullExploration100Impossible) {
  std::vector<int> list = GetCostList(100, 20000000);
  EXPECT_EQ(list.size(), 0);
}

TEST(GTestGetCostList, FullExploration100Possible) {
  std::vector<int> list = GetCostList(100, GetMaxCost(100));
  EXPECT_EQ(list.size() > 0,true);
}

TEST(GTestGetMaxCost, Basic) {
  EXPECT_EQ(GetMaxCost(1), 0);
  EXPECT_EQ(GetMaxCost(2), 2);
  EXPECT_EQ(GetMaxCost(3), 5);
}

TEST(GTestReverse, Basic) {
  std::vector<int> base = {1,2,3,4,5,6,7};
  std::vector<int> test;

  test = base;
  ReverseWithCost(test, 6, 1);
  EXPECT_EQ(test[6], 7);

  test = base;
  ReverseWithCost(test, 5, 1);
  EXPECT_EQ(test[5], 6);
  EXPECT_EQ(test[6], 7);

  test = base;
  ReverseWithCost(test, 5, 2);
  EXPECT_EQ(test[5], 7);
  EXPECT_EQ(test[6], 6);

  test = base;
  ReverseWithCost(test, 5, 0);
  EXPECT_EQ(test[4], 5);
  EXPECT_EQ(test[5], 6);
  EXPECT_EQ(test[6], 7);
}

TEST(GTestGivenCases, Case2) {
  CaseDetails det;
  det.N = 2;
  det.C = 1;
  CaseSolution soln = SolveCase(det);
  EXPECT_EQ(soln.nums.size(), 2);
}

TEST(GTestGetCostList, FullExploration100PossibleMaxReps) {
  for(int i=0; i<100; i++)
  {
    std::vector<int> list = GetCostList(100, GetMaxCost(100));
    EXPECT_EQ(list.size() > 0,true);
  }
}

TEST(GTestGetCostList, FullExploration100ImpossibleMaxReps) {
  for(int i=0; i<100; i++)
  {
    std::vector<int> list = GetCostList(100, GetMaxCost(100)+1);
    EXPECT_EQ(list.size() > 0,false);
  }
}

TEST(GTestGetCostList, FullExploration100LowestMaxReps) {
  for(int i=0; i<100; i++)
  {
    std::vector<int> list = GetCostList(100, 99);
    EXPECT_EQ(list.size() > 0,true);
  }
}
#endif
