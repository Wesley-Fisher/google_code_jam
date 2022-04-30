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
  int N;
  std::vector<int> D;
};

struct CaseSolution {
  int y;
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

    for(int i=0; i < p.N; i++)
    {
      int d;
      in >> d;
      p.D.push_back(d);
    }

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.y << std::endl;
}

bool ServeDecision(const std::vector<int>& D, int& i0, int& i1, int& payed, int& best_so_far)
{
  bool use_start = (D[i0] <= D[i1]);
  
  int d;
  if (use_start)
  {
    d = D[i0];
  }
  else
  {
    d = D[i1];
  }

  if (d >= best_so_far)
  {
    best_so_far = d;
    payed++;
  }

  if (use_start)
  {
    i0++;
  }
  else
  {
    i1--;
  }

  return i0 <= i1;
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;

    int i0 = 0;
    int i1 = details.D.size() - 1;
    int payed = 0;
    int best_so_far = -1;

    while(ServeDecision(details.D, i0, i1, payed, best_so_far))
    {

    }
    solution.y = payed;

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

TEST(GTestTest, ChoosesLeft) {
  std::vector<int> D = {1, 2, 4};
  int i0 = 0;
  int i1 = 2;
  int payed = 0;
  int best_so_far = -1;

  ServeDecision(D, i0, i1, payed, best_so_far);
  EXPECT_EQ(i0, 1);
  EXPECT_EQ(payed, 1);
  EXPECT_EQ(best_so_far, 1);
}

TEST(GTestTest, ChoosesRight) {
  std::vector<int> D = {5, 2, 4};
  int i0 = 0;
  int i1 = 2;
  int payed = 0;
  int best_so_far = -1;

  ServeDecision(D, i0, i1, payed, best_so_far);
  EXPECT_EQ(i0, 0);
  EXPECT_EQ(i1, 1);
  EXPECT_EQ(payed, 1);
  EXPECT_EQ(best_so_far, 4);
}

TEST(GTestTest, Example1) {
  CaseDetails det;
  det.N = 1;
  det.D = {1, 5};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 2);
}

TEST(GTestTest, Example2) {
  CaseDetails det;
  det.N = 4;
  det.D = {1, 4, 2, 3};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 3);
}

TEST(GTestTest, Example3) {
  CaseDetails det;
  det.N = 5;
  det.D = {10, 10, 10, 10, 10};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 5);
}

TEST(GTestTest, Example4) {
  CaseDetails det;
  det.N = 4;
  det.D = {7, 1, 3, 1000000};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 2);
}

TEST(GTestTest, Toy1) {
  CaseDetails det;
  det.N = 5;
  det.D = {1, 2, 3, 4, 5};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 5);
}

TEST(GTestTest, Toy2) {
  CaseDetails det;
  det.N = 5;
  det.D = {5, 4, 3, 2, 1};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 5);
}

TEST(GTestTest, Toy3) {
  CaseDetails det;
  det.N = 5;
  det.D = {5, 3, 1, 2, 4};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, 2);
}
#endif
