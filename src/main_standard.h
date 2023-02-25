
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

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

CaseDetails* ReadCaseDetails(std::istream& in);
CaseSolution* SolveCase(CaseDetails* details, std::string sCase);
std::string CaseOutput(CaseSolution* solution, int case_num);
bool OutputCompare(std::string mine, std::string ans);
void delete_impl(CaseDetails* details, CaseSolution* solution);

void OutputSolution(CaseSolution* solution, std::ostream& out, int case_num) {
    out << CaseOutput(solution, case_num);
}

void SolveProblem(std::istream& in, std::ostream& out) {
    int num_problems = 0;
    in >> num_problems;

    for(int i=0; i<num_problems; i++) {
        CaseDetails* case_details = ReadCaseDetails(in);
        CaseSolution* solution = SolveCase(case_details, std::to_string(i+1));
        OutputSolution(solution, out, i+1);

        delete_impl(case_details, solution);
    }
}

void AnalyzeProblem(std::istream& in, std::istream& answers) {
    int num_problems = 0;
    in >> num_problems;

    LOG("ANALYZING " << num_problems << " problems");

    for(int i=0; i<num_problems; i++) {
        CaseDetails* case_details = ReadCaseDetails(in);
        CaseSolution* solution = SolveCase(case_details, std::to_string(i+1));

        std::string ans = CaseOutput(solution, i+1);
        ans.pop_back(); // Remove newline for logging.comparing

        std::string key;
        std::getline(answers, key);

        if(!OutputCompare(ans, key))
        {
          LOG("MISMATCH (mine, key)");
          LOG(ans);
          LOG(key);
          LOG("");
        }

        delete_impl(case_details, solution);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

int main_analysis(int argc, char** argv)
{
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
    return 0;
}

int main_local(int argc, char** argv)
{
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
    return 0;
}