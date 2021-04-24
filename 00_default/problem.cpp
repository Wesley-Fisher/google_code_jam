#include "problem.h"


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

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    //out << "Case #" << case_num << ": " << std::endl;
}

CaseSolution SolveCase(CaseDetails details) {
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
#endif
