#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

#include <fstream>
#include <iostream>
#include <iomanip>

#include <vector>
#include <deque>
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
  std::string filename = name + ".txt";
  fin.open(filename);
  return fin.is_open();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Define common structures and functions
struct CaseDetails;
struct CaseSolution;

void SolveProblem(std::istream& in, std::ostream& out);
void AnalyzeProblem(std::istream& in, std::istream& answers);
CaseDetails ReadCaseDetails(std::istream& in);
CaseSolution SolveCase(CaseDetails details, std::string sCase);
std::string CaseOutput(CaseSolution solution, int case_num);
void OutputSolution(CaseSolution solution, std::ostream& out, int case_num);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specific Structs
struct CaseDetails {
};

struct CaseSolution {
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main(int argc, char** argv)
{
  #ifdef ANALYSIS
    if(argc != 3)
    {
      std::cout << "Call with [exe, input, output] as arguments" << std::endl;
      return 0;
    }
    std::ifstream fin;
    std::string inName = std::string(argv[1]);
    if(!OpenTest(inName, fin))
    {
      std::cout << "Create input file" << std::endl;
      return 0;
    }
    std::ifstream answers;
    std::string ansName = std::string(argv[2]);
    if(!OpenTest(ansName, answers))
    {
      std::cout << "Create answers file" << std::endl;
      return 0;
    }

    AnalyzeProblem(fin, answers);
  #elif defined LOCAL
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
        CaseSolution solution = SolveCase(case_details, std::to_string(i+1));
        OutputSolution(solution, out, i+1);
    }
}

void AnalyzeProblem(std::istream& in, std::istream& answers) {
    int num_problems = 0;
    in >> num_problems;

    LOG("ANALYZING " << num_problems << " problems");

    for(int i=0; i<num_problems; i++) {
        CaseDetails case_details = ReadCaseDetails(in);
        CaseSolution solution = SolveCase(case_details, std::to_string(i+1));
        
        std::string ans = CaseOutput(solution, i+1);
        ans.pop_back(); // Remove newline for logging.comparing
        
        std::string key;
        std::getline(answers, key);
        
        if(ans.compare(key)!= 0)
        {
          LOG("MISMATCH (mine, key)");
          LOG(ans);
          LOG(key);
          LOG("");
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

std::string CaseOutput(CaseSolution solution, int case_num)
{
  std::stringstream ss;
  //ss << "Case #" << case_num << ": " << std::endl;
  return std::string(ss.str());
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << CaseOutput(solution, case_num);
}

CaseSolution SolveCase(CaseDetails details, std::string sCase) {
    LOG("SolveCase: " << sCase);
    CaseSolution solution;

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

TEST(GTestTest, SolveCaseDummy) {
  CaseDetails det;
  CaseSolution sol = SolveCase(det, "dummy");
}
#endif
