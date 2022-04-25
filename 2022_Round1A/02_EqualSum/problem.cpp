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
  #define LOG(...) std::cerr << __VA_ARGS__ << std::endl
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
CaseSolution SolveCase(CaseDetails details, std::istream& in, std::ostream& out);
void OutputSolution(CaseSolution solution, std::ostream& out, int case_num);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specific Structs

struct CaseDetails {
  int N;
};

struct CaseSolution {
  std::vector<unsigned long long> nums;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main(int argc, char** argv)
{
  SolveProblem(std::cin, std::cout);
  return 0;
}
#endif


void SolveProblem(std::istream& in, std::ostream& out) {
    int num_problems = 0;
    in >> num_problems;

    for(int i=0; i<num_problems; i++) {
        CaseDetails case_details = ReadCaseDetails(in);
        CaseSolution solution = SolveCase(case_details, in, out);
        OutputSolution(solution, out, i+1);
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

CaseDetails ReadCaseDetails(std::istream& in) {
    CaseDetails p;
    in >> p.N;
    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    //out << "Case #" << case_num << ": " << std::endl;
    for(int i=0; i<solution.nums.size(); i++)
    {
      out <<  solution.nums[i] << " ";
    }
    out << std::endl << std::flush;
}

std::vector<int> solveRecursive(const std::vector<unsigned long long>& nums, int i, unsigned long long sumChosen, unsigned long long sumGoal)
{
  std::vector<int> out;

  if(i == nums.size())
  {
    return out;
  }

  unsigned long long N = nums.at(i);
  //LOG("Sum is " << sumChosen << ", aiming for " << sumGoal << " and have " << N);

  if (sumChosen > sumGoal)
  {
    return out;
  }

  if(sumChosen + N == sumGoal)
  {
    out.push_back(i);
    return out;
  }


  std::vector<int> prev1 = solveRecursive(nums, i+1, sumChosen+N, sumGoal);
  if (prev1.size() > 0)
  {
    prev1.push_back(i);
    return prev1;
  }

  std::vector<int> prev2 = solveRecursive(nums, i+1, sumChosen, sumGoal);
  if (prev2.size() > 0)
  {
    return prev2;
  }

  return out;
}

unsigned long long sumUp(const std::vector<unsigned long long>& nums, const std::vector<int>& idxs)
{
  unsigned long long N = 0;
  for(int i=0; i<idxs.size(); i++)
  {
    int idx = idxs[i];
    N += nums[idx];
  }
  return N;
}


CaseSolution SolveCase(CaseDetails details, std::istream& in, std::ostream& out) {
    CaseSolution solution;

    std::vector<unsigned long long> blocks;
    std::vector<unsigned long long> bigs;

    unsigned long long TOTAL = 0;
    unsigned long long MAX_BLK = 0;
    unsigned long long MAX_BIG = 0;

    for(int i=1; i<=5; i++)
    {
      unsigned long long base = 1;
      while (base < 1000000000)
      {
        unsigned long long blk = base * i;
        blocks.push_back(blk);
        base = base * 10;

        TOTAL += blk;
        MAX_BLK += blk;
        out << blk << " ";
      }
    }
    LOG("Created blocks: " << blocks.size());

    unsigned long long big = 1000000000;
    while(blocks.size() + bigs.size() < details.N)
    {
      bigs.push_back(big);
      TOTAL += big;
      MAX_BIG+= big;
      out << big << " ";
      big--;
    }
    LOG("Created bigs: " << bigs.size());

    out << std::endl << std::flush;

    std::vector<unsigned long long> nums;
    for(int i=0; i<details.N; i++)
    {
      unsigned long long l;
      in >> l;
      nums.push_back(l);
      TOTAL += l;
    }

    unsigned long long HALFTOTAL = TOTAL / 2;
    LOG("Total: " << TOTAL << ", half = " << HALFTOTAL);
    LOG("Blocks can build: " << MAX_BLK);
    LOG("Bigs can build: " << MAX_BIG);

    std::vector<unsigned long long> used;
    while(HALFTOTAL > 999999999 && bigs.size() > 0)
    {
      unsigned long long l = bigs.at(bigs.size()-1);
      bigs.pop_back();
      HALFTOTAL -= l;
      used.push_back(l);
    }

    LOG("Target after bigs: " << HALFTOTAL);

    if (HALFTOTAL > 999999999)
    {
      LOG("Can't Build Yet");
    }

    int i=0;
    while(HALFTOTAL > 999999999)
    {
      if (nums[i] < 999999999)
      {
        HALFTOTAL -= nums[i];
        used.push_back(nums[i]);
      }
      i++;
    }

    int pow = 1;
    int fac = 1;
    while(HALFTOTAL > 0)
    {
      int need = HALFTOTAL % 10;

      switch(need)
      {
        case 0: break;
        case 1: used.push_back(1 * fac); break;
        case 2: used.push_back(2 * fac); break;
        case 3: used.push_back(3 * fac); break;
        case 4: used.push_back(4 * fac); break;
        case 5: used.push_back(5 * fac); break;
        case 6: used.push_back(1 * fac); used.push_back(2 * fac); used.push_back(3 * fac); break;
        case 7: used.push_back(3 * fac); used.push_back(4 * fac); break;
        case 8: used.push_back(1 * fac); used.push_back(3 * fac); used.push_back(4 * fac); break;
        case 9: used.push_back(5 * fac); used.push_back(4 * fac); break;
      }

      fac = fac * 10;
      HALFTOTAL = HALFTOTAL / 10;
    }
    
    solution.nums = used;
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

TEST(GTestTest, TestBasicRecursion) {
  std::vector<unsigned long long> nums = {5, 1, 3, 10, 4, 9};
  std::vector<int> idxs = solveRecursive(nums, 0, 0, 32/2);
  EXPECT_EQ(sumUp(nums, idxs), 16);
}

TEST(GTestTest, TestBasicRecursion2) {
  std::vector<unsigned long long> nums = {5, 2, 3, 10, 8, 12};
  std::vector<int> idxs = solveRecursive(nums, 0, 0, 40/2);
  EXPECT_EQ(sumUp(nums, idxs), 20);
}
#endif
