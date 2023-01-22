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

typedef std::vector< std::vector< unsigned long > > pressures;

struct CaseDetails {
  int N;
  int P;
  pressures Press;
  std::vector<int> Mins;
  std::vector<int> Maxs;
};

struct CaseSolution {
  unsigned long y;
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
    in >> p.P;

    for(int i=0; i<p.N; i++)
    {
      unsigned long min = 0;
      unsigned long max = 0;
      std::vector<unsigned long> ps;
      for(int j=0; j<p.P; j++)
      {
        unsigned long u;
        in >> u;

        ps.push_back(u);
        if (min == 0 || u < min)
        {
          min = u;
        }
        if (max == 0 || u > max)
        {
          max = u;
        }
      }

      p.Press.push_back(ps);
      p.Maxs.push_back(max);
      p.Mins.push_back(min);
    }

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.y << std::endl;
}

unsigned long absDiff(unsigned long a, unsigned long b)
{
  if (a >= b)
  {
    return a - b;
  }
  return b -a;
}

void AddCoverage2(unsigned long& p0, unsigned long pMin, unsigned long pMax, unsigned long pMinNext, unsigned long pMaxNext, unsigned long& presses)
{
  unsigned long pass1 = absDiff(pMax, pMin);

  LOG("From " << p0 << " to (" << pMin << "," << pMax << "), (" << pMinNext << ", " << pMaxNext << ")");

  unsigned long pMaxMax = absDiff(pMax, p0) + pass1 +absDiff(pMin, pMaxNext);
  unsigned long pMaxMin = absDiff(pMax, p0) + pass1 +absDiff(pMin, pMinNext);
  unsigned long pMinMax = absDiff(pMin, p0) + pass1 +absDiff(pMax, pMaxNext);
  unsigned long pMinMin = absDiff(pMin, p0) + pass1 +absDiff(pMax, pMinNext);

  LOG("Paths: " << pMaxMax << ", " << pMaxMin << ", " << pMinMax << ", " << pMinMin);

  unsigned long pMinPath = std::min(std::min(pMaxMax, pMaxMin), std::min(pMinMax, pMinMin));

  if(pMaxMax == pMinPath || pMaxMin == pMinPath)
  {
    unsigned long pAdd = absDiff(pMax, p0) + pass1;
    p0 = pMin;
    presses += pAdd;
     LOG("To Max, add " << pAdd << ", total = " << presses);
  }
  else
  {
    unsigned long pAdd = absDiff(pMin, p0) + pass1;
    p0 = pMax;
    presses += pAdd;
    LOG("To Min, add " << pAdd << ", total = " << presses);
  }
}

void AddCoverage(unsigned long& p0, unsigned long pMin, unsigned long pMax, unsigned long& presses)
{
  unsigned long d1 = absDiff(p0, pMin);
  unsigned long d2 = absDiff(p0, pMax);

  LOG("From " << p0 << " to (" << pMin << ", " << pMax << ")");
  LOG("Coverage compare " << d1 << " vs " << d2);

  unsigned long pEnd;
  if (d1 < d2)
  {
    presses += d1;
    presses += pMax - pMin;
    p0 = pMax;
    LOG("To Min, add " << d1 + pMax - pMin << ", total = " << presses);
  }
  else
  {
    presses += d2;
    presses += pMax - pMin;
    p0 = pMin;
    LOG("To Max, add " << d2 + pMax - pMin << ", total = " << presses);
  }
}

void AddCoverage3Reverse(unsigned long& pLast, unsigned long pMin, unsigned long pMax, unsigned long& pressed)
{
  unsigned long d1 = absDiff(pLast, pMin);
  unsigned long d2 = absDiff(pLast, pMax);

  if (d1 < d2)
  {
    pressed = pressed + (pMax - pMin) + d1;
    pLast = pMax;
  }
  else
  {
    pressed = pressed + (pMax - pMin) + d2;
    pLast = pMin;
  }
}

void AddCoverage4(unsigned long& lastUp, unsigned long& lastDown, unsigned long pMin, unsigned long pMax, unsigned long& pressUp, unsigned long& pressDown)
{
  unsigned long newUp = pMax - pMin + std::min(absDiff(lastUp, pMin)+pressUp, absDiff(lastDown, pMin)+ pressDown);
  unsigned long newDown = pMax - pMin + std::min(absDiff(lastUp, pMax)+pressUp, absDiff(lastDown, pMax)+ pressDown);

  pressUp = newUp;
  pressDown = newDown;
  lastUp = pMax;
  lastDown = pMin;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;
    solution.y = 0;

    int iLast = details.Mins.size()-1;
    unsigned long lastUp = 0;
    unsigned long pressesUp = 0;
    unsigned long lastDown = 0;
    unsigned long pressesDown = 0;

    for(int i=0; i<details.Mins.size(); i++)
    {
      AddCoverage4(lastUp, lastDown, details.Mins[i], details.Maxs[i], pressesUp, pressesDown);
    }

    solution.y = std::min(pressesUp, pressesDown);

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

TEST(GTestTest, Example1) {
  CaseDetails det;
  det.N = 3;
  det.P = 3;
  det.Press.push_back({30, 10, 40});
  det.Press.push_back({20, 50, 60});
  det.Press.push_back({60, 60, 50});
  det.Mins = {10, 20, 50};
  det.Maxs = {40, 60, 60};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 110);
}

TEST(GTestTest, Example2) {
  CaseDetails det;
  det.N = 5;
  det.P = 2;
  det.Press.push_back({1, 1000000000});
  det.Press.push_back({500000000, 1000000000});
  det.Press.push_back({1, 1000000000});
  det.Press.push_back({500000000, 1});
  det.Press.push_back({1, 1000000000});
  det.Mins = {1, 500000000, 1, 1, 1};
  det.Maxs = {1000000000, 1000000000, 1000000000, 500000000, 1000000000};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 4999999996);
}

TEST(GTestTest, CoverageEx1A) {
  unsigned long p0 = 0;
  unsigned long presses = 0;

  AddCoverage2(p0, 10, 40, 20, 60, presses);

  EXPECT_EQ(p0, 40);
  EXPECT_EQ(presses, 40);
}

TEST(GTestTest, CoverageEx1B) {
  unsigned long p0 = 40;
  unsigned long presses = 40;

  AddCoverage2(p0, 20, 60, 50, 60, presses);

  EXPECT_EQ(p0, 60);
  EXPECT_EQ(presses, 100);
}

TEST(GTestTest, CoverageEx1C) {
  unsigned long p0 = 60;
  unsigned long presses = 100;

  AddCoverage(p0, 50, 60, presses);

  EXPECT_EQ(p0, 50);
  EXPECT_EQ(presses, 110);

  p0 = 50;
  presses = 100;

  AddCoverage(p0, 50, 60, presses);

  EXPECT_EQ(p0, 60);
  EXPECT_EQ(presses, 110);
}

TEST(GTestTest, ExampleMine1) {
  CaseDetails det;
  det.N = 5;
  det.P = 2;
  det.Press.push_back({1, 10});
  det.Press.push_back({5, 10});
  det.Press.push_back({1, 10});
  det.Press.push_back({5, 1});
  det.Press.push_back({1, 10});
  det.Mins = {1, 5, 1, 1, 1};
  det.Maxs = {10, 10, 10, 5, 10};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 46);
}

TEST(GTestTest, ExampleMine2) {
  CaseDetails det;
  det.N = 5;
  det.P = 2;
  det.Mins = {1, 1, 1, 1, 1};
  det.Maxs = {10, 10, 10, 1, 10};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 46);
}

TEST(GTestTest, ExampleMine3) {
  CaseDetails det;
  det.N = 5;
  det.P = 2;
  det.Mins = {1, 1, 11, 1, 1};
  det.Maxs = {10, 10, 20, 1, 10};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 66);
}

TEST(GTestTest, ExampleMine4) {
  CaseDetails det;
  det.N = 3;
  det.P = 2;
  det.Mins = {1, 11, 1};
  det.Maxs = {10, 20, 10};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 39);
}

TEST(GTestTest, ExampleMine4A) {
  unsigned long p0 = 0;
  unsigned long presses = 0;

  AddCoverage2(p0, 1, 10, 11, 20, presses);

  EXPECT_EQ(p0, 10);
  EXPECT_EQ(presses, 10);
}
TEST(GTestTest, ExampleMine4B) {
  unsigned long p0 = 10;
  unsigned long presses = 10;

  AddCoverage2(p0, 11, 20, 1, 10, presses);

  EXPECT_EQ(p0, 11);
  EXPECT_EQ(presses, 29);
}

TEST(GTestTest, ExampleMine4C) {
  unsigned long p0 = 11;
  unsigned long presses = 29;

  AddCoverage(p0, 1, 10, presses);

  EXPECT_EQ(p0, 1);
  EXPECT_EQ(presses, 39);
}

TEST(GTestTest, ExampleMine5) {
  CaseDetails det;
  det.N = 3;
  det.P = 2;
  det.Mins = {11, 1, 11};
  det.Maxs = {20, 10, 20};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 58);
}

TEST(GTestTest, ExampleMine6) {
  CaseDetails det;
  det.N = 3;
  det.P = 2;
  det.Mins = {11, 1, 11};
  det.Maxs = {20, 30, 20};

  CaseSolution sol = SolveCase(det);

  EXPECT_EQ(sol.y, 78);
}

TEST(GTestTest, AddCoverage3Reverse1)
{
  unsigned long pressed = 0;
  unsigned long pMin = 10;
  unsigned long pMax = 15;
  unsigned long pLast = 9;

  AddCoverage3Reverse(pLast, pMin, pMax, pressed);
  EXPECT_EQ(pressed, 6);
  EXPECT_EQ(pLast, 15);
}

TEST(GTestTest, AddCoverage3Reverse2)
{
  unsigned long pressed = 0;
  unsigned long pMin = 10;
  unsigned long pMax = 15;
  unsigned long pLast = 20;

  AddCoverage3Reverse(pLast, pMin, pMax, pressed);
  EXPECT_EQ(pressed, 10);
  EXPECT_EQ(pLast, 10);
}
#endif
