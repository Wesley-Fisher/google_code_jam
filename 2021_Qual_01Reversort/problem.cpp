#include <fstream>
#include <iostream>

#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

// Includes for problem here
#include <vector>
#include <algorithm>

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
  int L;
  std::vector<int> list;
};

struct CaseSolution {
  int cost;
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
    in >> p.L;
    for(int i=0; i<p.L; i++)
    {
      int n;
      in >> n;
      p.list.push_back(n);
    }

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.cost << std::endl;
}

int GetMinNumIdx(std::vector<int>& list, int i)
{
  int min = list[i];
  int minI = i;
  for(int j=i; j < list.size(); j++)
  {
    if (list[j] < min)
    {
      min = list[j];
      minI = j;
    }
  }

  return minI;
}

int ReverseList(std::vector<int>& list, int i, int j)
{
  std::reverse(list.begin()+i, list.begin()+j+1);
  return j - i + 1;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;
    solution.cost = 0;

    for(int i=0; i<details.L-1; i++)
    {
      int j = GetMinNumIdx(details.list, i);
      int c = ReverseList(details.list, i, j);
      solution.cost += c;
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


TEST(TestGetMinNum, BasicSearch) {
  std::vector<int> list = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(GetMinNumIdx(list, 0), 0);
  EXPECT_EQ(GetMinNumIdx(list, 1), 1);
}

TEST(TestGetMinNum, ReverseSearch) {
  std::vector<int> list = {7, 6, 5, 4, 3, 2, 1};
  EXPECT_EQ(GetMinNumIdx(list, 0), 6);
  EXPECT_EQ(GetMinNumIdx(list, 1), 6);
}

TEST(TestGetMinNum, MixedSearch) {
  std::vector<int> list = {7, 6, 5, 1, 2, 3, 4};
  EXPECT_EQ(GetMinNumIdx(list, 0), 3);
  EXPECT_EQ(GetMinNumIdx(list, 4), 4);
}

TEST(TestReverse, FullList) {
  std::vector<int> list = {1,2,3,4,5,6,7};
  ReverseList(list, 0, 6);
  EXPECT_EQ(list[0], 7);
  EXPECT_EQ(list[1], 6);
}

TEST(TestReverse, MidList) {
  std::vector<int> list = {1,2,3,4,5,6,7};
  ReverseList(list, 1, 5);
  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 6);
}

TEST(Solution, Case1) {
  CaseDetails d;
  d.L = 4;
  d.list = {4,2,1,3};
  EXPECT_EQ(SolveCase(d).cost, 6);
}

TEST(Solution, Case2) {
  CaseDetails d;
  d.L = 2;
  d.list = {1,2};
  EXPECT_EQ(SolveCase(d).cost, 1);
}

TEST(Solution, Case3) {
  CaseDetails d;
  d.L = 7;
  d.list = {7,6,5,4,3,2,1};
  EXPECT_EQ(SolveCase(d).cost, 12);
}

#endif
