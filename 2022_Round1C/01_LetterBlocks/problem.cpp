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
bool OutputCompare(std::string mine, std::string ans);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specific Structs
struct CaseDetails {
  int N;
  std::deque<std::string> towers;
};

struct CaseSolution {
  std::string y;
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

    std::string sOut = file + "_results.txt";
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
        CaseSolution solution = SolveCase(case_details, std::to_string(i));
        OutputSolution(solution, out, i+1);
    }
}

void AnalyzeProblem(std::istream& in, std::istream& answers) {
    int num_problems = 0;
    in >> num_problems;

    std::cout << "ANALYZING " << num_problems << " problems" << std::endl;

    for(int i=0; i<num_problems; i++) {
        CaseDetails case_details = ReadCaseDetails(in);
        CaseSolution solution = SolveCase(case_details, std::to_string(i+1));
        
        std::string ans = CaseOutput(solution, i+1);
        ans.pop_back(); // Remove newline for logging.comparing
        
        std::string key;
        std::getline(answers, key);
        
        if(!OutputCompare(ans, key))
        {
          std::cout << "MISMATCH (mine, key)" << std::endl;
          std::cout << ans << std::endl;
          std::cout << key << std::endl;
          std::cout << "" << std::endl;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Problem-Specifics

CaseDetails ReadCaseDetails(std::istream& in) {
    CaseDetails p;
    in >> p.N;
    for(int i=0; i<p.N; i++)
    {
      std::string s;
      in >> s;
      p.towers.push_back(s);
    }
    return p;
}

std::string CaseOutput(CaseSolution solution, int case_num)
{
  std::stringstream ss;
  ss << "Case #" << case_num << ": " << solution.y << std::endl;
  return std::string(ss.str());
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << CaseOutput(solution, case_num);
}


bool isValid(const std::string& s)
{
  int counts[26] = {0};
  for(int i=0; i<s.length(); i++)
  {
    char c = s[i];
    int j = c - 'A';

    if (counts[j] > 0 && s[i-1] != c)
    {
      return false;
    }
    counts[j]++;
  }
  return true;
}


void baseStatsCheck(const std::string& s, int* startsOnly, int* middleOnly, int* endOnly)
{
  char cs = s[0];
  char cf = s[s.length()-1];

  bool didStartsOnly = false;
  bool didEndOnly = false;
  bool didMiddleOnly = false;

  for(int i=1; i<s.length()-1; i++)
  {
    if (s[i] != cs && !didStartsOnly)
    {
      startsOnly[cs-'A']++;
      didStartsOnly = true;
    }

    if (s[i] != cf && !didEndOnly)
    {
      endOnly[cf-'A']++;
      didEndOnly = true;
    }

    if(s[i] != cs && s[i] != cf && !didMiddleOnly)
    {
      middleOnly[s[i]-'A']++;
      didMiddleOnly = true;
    }
  }
}

bool baseStatsCheckAll(const std::deque<std::string>& towers)
{
  int startsOnly[26] = {false};
  int middleOnly[26] = {false};
  int endOnly[26] = {false};

  for(int i=0; i<towers.size(); i++)
  {
    baseStatsCheck(towers[i], startsOnly, middleOnly, endOnly);
  }

  for(int i=0; i<26; i++)
  {
    if(startsOnly[i] > 1)
    {
      return false;
    }
    if(endOnly[i] > 1)
    {
      return false;
    }
    if(middleOnly[i] > 1)
    {
      return false;
    }
    if(startsOnly[i] + middleOnly[i] > 1)
    {
      return false;
    }
    if(endOnly[i] + middleOnly[i] > 1)
    {
      return false;
    }
  }
  return true;
}



std::deque<std::string> mergeSolid(std::deque<std::string> towers)
{
  if(towers.size() <= 1)
  {
    return towers;
  }

  std::deque<std::string> out;

  for(int i=0; i<towers.size(); i++)
  {
    if(towers[i].size() == 0)
    {
      continue;
    }

    std::string s = towers[i];
    char c = s[0];
    bool solid = true;
    for(int i=0; i<s.size(); i++)
    {
      if (s[i] != c)
      {
        solid = false;
      }
    }

    if(!solid)
    {
      continue;
    }

    for(int j=0; j<towers.size(); j++)
    {
      if(j == i)
      {
        continue;
      }

      if(towers[j].size() == 0)
      {
        continue;
      }

      if(towers[j][towers[j].size()-1] == c)
      {
        towers[j] = towers[j] + s;
        towers[i] = "";
        break;
      }

      if(towers[j][0] == c)
      {
        towers[j] = s + towers[j];
        towers[i] = "";
        break;
      }
    }
  }

  for(int i=0; i<towers.size(); i++)
  {
    if (towers[i].size() > 0)
    {
      LOG("  " << i << "=" << towers[i]);
      out.push_back(towers[i]);
    }
  }

  return out;
}

std::deque<std::string> mergeBookends(std::deque<std::string> towers)
{
  if(towers.size() <= 1)
  {
    return towers;
  }

  std::deque<std::string> out;

  for(int i=0; i<towers.size(); i++)
  {
    if(towers[i].size() == 0)
    {
      continue;
    }

    std::string s = towers[i];
    char c0 = s[0];
    char c1 = s[s.size()-1];

    for(int j=0; j<towers.size(); j++)
    {
      if(j == i)
      {
        continue;
      }

      if(towers[j].size() == 0)
      {
        continue;
      }

      LOG("Compare bookends: " << towers[i] << ", " << towers[j]);

      if(towers[j][towers[j].size()-1] == c0)
      {
        towers[j] = towers[j] + s;
        towers[i] = "";
        LOG("Match1");
        break;
      }

      if(towers[j][0] == c1)
      {
        towers[j] = s + towers[j];
        towers[i] = "";
        LOG("Match2");
        LOG(towers[i].size());
        LOG(towers[j]);
        break;
      }
    }
  }

  for(int i=0; i<towers.size(); i++)
  {
    if (towers[i].size() > 0)
    {
      LOG("  " << i << "=" << towers[i]);
      out.push_back(towers[i]);
    }
  }

  return out;
}


CaseSolution SolveCase(CaseDetails details, std::string sCase) {
    LOG("SolveCase: " << sCase);
    CaseSolution solution;
    solution.y = "IMPOSSIBLE";

    for(int i=0; i<details.N; i++)
    {
      if(!isValid(details.towers[i]))
      {
        LOG("Found invalid at start [" << i << "]=" << details.towers[i]);
        return solution;
      }
    }

    std::deque<std::string> towers = mergeSolid(details.towers);
    LOG("After solids merged: " << towers.size());
    int iStart = towers.size();
    int iEnd = iStart + 1;
    while(iEnd != iStart)
    {
      iStart = towers.size();
      towers = mergeBookends(towers);
      LOG("After bookends merged: " << towers.size());

      for(int i=0; i<towers.size(); i++)
      {
        if(!isValid(towers[i]))
        {
          LOG("Found invalid [" << i << "]=" << towers[i]);
          return solution;
        }
      }

      iEnd = towers.size();
    }

    std::string sol;
    for(int i=0; i<towers.size(); i++)
    {
      sol = sol + towers[i];
    }
    LOG("  Final=" << sol);

    if(isValid(sol))
    {
      solution.y = sol;
      return solution;
    }

    return solution;
}

// Dummy so I don't need to modify everything here
CaseSolution SolveCase(CaseDetails details)
{
  return SolveCase(details, "dummyX");
}

bool OutputCompare(std::string mine, std::string ans)
{
  int i = 0;
  while (mine[i] != ':')
    i++;
  i++;
  i++;

  // Should start the same
  mine = mine.substr(i);
  ans = ans.substr(i);

  if (mine.compare(ans) == 0)
  {
    return true;
  }

  if( (mine.compare("IMPOSSIBLE") !=0) !=
      (ans.compare("IMPOSSIBLE") != 0) )
  {
    return false;
  }

  if(mine.size() != ans.size())
  {
    return false;
  }

  int letters[26] = {0};
  for(int i=0; i<mine.size(); i++)
  {
    letters[mine[i]-'A']--;
    letters[ans[i]-'A']++;
  }

  for(int i=0; i<26; i++)
  {
    if (letters[i] != 0)
    {
      return false;
    }
  }

  return true;
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
  det.N = 0;
  CaseSolution sol = SolveCase(det, "dummy");
}

TEST(GTestTest, Example1) {
  CaseDetails det;
  det.N = 5;
  det.towers = {"CODE", "JAM", "MIC", "EEL", "ZZZZZ"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "JAMMICCODEEELZZZZZ");
}

TEST(GTestTest, Example2) {
  CaseDetails det;
  det.N = 6;
  det.towers = {"CODE", "JAM", "MIC", "EEL", "ZZZZZ", "EEK"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, Example3) {
  CaseDetails det;
  det.N = 2;
  det.towers = {"OY", "YO"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, Example4) {
  CaseDetails det;
  det.N = 2;
  det.towers = {"HASH", "CODE"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, Example5) {
  CaseDetails det;
  det.N = 6;
  det.towers = {"A", "AA", "BB", "A", "BA", "BB"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "BBBBBAAAAA");
}

TEST(GTestTest, Example6) {
  CaseDetails det;
  det.N = 2;
  det.towers = {"CAT", "TAX"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, TestisValid) {
  std::string sGood1 = "AABBBCCCCCD";
  EXPECT_EQ(isValid(sGood1), true);

  std::string sGood2 = "A";
  EXPECT_EQ(isValid(sGood2), true);

  std::string sGood3 = "AB";
  EXPECT_EQ(isValid(sGood3), true);

  std::string sBad1 = "AABBBCCCCCDA";
  EXPECT_EQ(isValid(sBad1), false);

  EXPECT_EQ(isValid("ABC"), true);
  EXPECT_EQ(isValid("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), true);
  EXPECT_EQ(isValid("ABCDEFGHIJKLMNOPQRSTUVWXYZE"), false);
}

TEST(GTestTest, TestMergeSolid)
{
  std::deque<std::string> dq0 = {"AAA"};
  std::deque<std::string> out0 = mergeSolid(dq0);
  EXPECT_EQ(out0.size(), 1);

  std::deque<std::string> dq1 = {"AAA", "AB"};
  std::deque<std::string> out1 = mergeSolid(dq1);
  EXPECT_EQ(out1.size(), 1);
  EXPECT_EQ(out1[0], "AAAAB");

  std::deque<std::string> dq2 = {"AAA", "BA"};
  std::deque<std::string> out2 = mergeSolid(dq2);
  EXPECT_EQ(out2.size(), 1);
  EXPECT_EQ(out2[0], "BAAAA");

  std::deque<std::string> dq3 = {"AAA", "BA", "A"};
  std::deque<std::string> out3 = mergeSolid(dq3);
  EXPECT_EQ(out3.size(), 1);
  EXPECT_EQ(out3[0], "BAAAAA");

  std::deque<std::string> dq4 = {"AAA", "BA", "A", "C"};
  std::deque<std::string> out4 = mergeSolid(dq4);
  EXPECT_EQ(out4.size(), 2);
  EXPECT_EQ(out4[0], "BAAAAA");

  std::deque<std::string> dq5 = {"AAA", "BA", "A", "C", "AA", "D", "AAAA"};
  std::deque<std::string> out5 = mergeSolid(dq5);
  EXPECT_EQ(out5.size(), 3);
  EXPECT_EQ(out5[0], "BAAAAAAAAAAA");
} 

TEST(GTestTest, TestMergeBookend)
{
  std::deque<std::string> dq0 = {"AAA", "BBB"};
  std::deque<std::string> out0 = mergeBookends(dq0);
  EXPECT_EQ(out0.size(), 2);

  std::deque<std::string> dq1 = {"AAA", "BBB", "CA"};
  std::deque<std::string> out1 = mergeBookends(dq1);
  EXPECT_EQ(out1.size(), 2);
  EXPECT_EQ(out1[0], "BBB");
  EXPECT_EQ(out1[1], "CAAAA");


  std::deque<std::string> dq2 = {"CODE", "JAM", "MIC", "EEL", "ZZZZZ"};
  std::deque<std::string> out2 = mergeBookends(dq2);
  EXPECT_EQ(out2.size(), 2);
  EXPECT_EQ(out2[0], "JAMMICCODEEEL");
  EXPECT_EQ(out2[1], "ZZZZZ");
}

TEST(GTestTest, TestLargeGood)
{
  std::deque<std::string> data;
  char c = 'A';
  for(int j=0; j<25; j++)
  {
    std::string s1;
    for(int i=0; i<10; i++);
      s1 = s1 + c;
    
    data.push_back(s1);

    std::string s2;
    for(int i=0; i<5; i++)
      s2 = s2 + c;
    c++;
    for(int i=0; i<5; i++)
      s2 = s2 + c;
    
    data.push_back(s2);
  }

  CaseDetails det;
  det.N = data.size();
  det.towers = data;

  CaseSolution sol = SolveCase(det);
  EXPECT_NE(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, ExampleMine1) {
  CaseDetails det;
  det.N = 26;
  for(int i=0; i<26; i++)
  {
    std::string s;
    s = s + (char)('A'+i);
    det.towers.push_back(s);
  }

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

TEST(GTestTest, ExampleMine2) {
  CaseDetails det;
  det.N = 27;
  for(int i=0; i<26; i++)
  {
    std::string s;
    s = s + (char)('A'+i);
    det.towers.push_back(s);
  }
  det.towers.push_back("AA");

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "BCDEFGHIJKLMNOPQRSTUVWXYZAAA");
}

TEST(GTestTest, ExampleMine3) {
  CaseDetails det;
  det.N = 28;
  for(int i=0; i<26; i++)
  {
    std::string s;
    s = s + (char)('A'+i);
    det.towers.push_back(s);
  }
  det.towers.push_back("AA");
  det.towers.push_back("BC");

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "DEFGHIJKLMNOPQRSTUVWXYZAAABBCC");
}

TEST(GTestTest, ExampleMine5) {
  CaseDetails det;
  det.N = 1;
  det.towers = {"ABC"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "ABC");
}

TEST(GTestTest, ExampleMine6) {
  CaseDetails det;
  det.N = 1;
  det.towers = {"ABCB"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, ExampleMine7) {
  CaseDetails det;
  det.N = 2;
  det.towers = {"ABC", "CB"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "IMPOSSIBLE");
}

TEST(GTestTest, ExampleMine8) {
  CaseDetails det;
  det.N = 2;
  det.towers = {"ABC", "CDD"};

  CaseSolution sol = SolveCase(det);
  EXPECT_EQ(sol.y, "ABCCDD");
}

TEST(GTestTest, AnalysisPS1_16) {
  CaseDetails det;
  det.N = 6;
  det.towers = {"BBHH", "W", "HKKK", "OOO", "H", "MMMMM"};

  CaseSolution sol = SolveCase(det, "AnalysisPS1_16");
  EXPECT_EQ(sol.y, "WBBHHHHKKKOOOMMMMM");
}

TEST(GTestTest, AnalysisPS1_16_MergeSteps) {
  std::deque<std::string> towers = {"BBHH", "W", "HKKK", "OOO", "H", "MMMMM"};
  std::deque<std::string> out1 = mergeSolid(towers);

  EXPECT_EQ(out1.size(), 5);
  EXPECT_EQ(out1[0], "BBHHH");
  EXPECT_EQ(out1[1], "W");
  EXPECT_EQ(out1[2], "HKKK");
  EXPECT_EQ(out1[3], "OOO");
  EXPECT_EQ(out1[4], "MMMMM");

  std::deque<std::string> out2 = mergeBookends(out1);
  EXPECT_EQ(out2.size(), 4);
  EXPECT_EQ(out2[0], "W");
  EXPECT_EQ(out2[1], "BBHHHHKKK");
  EXPECT_EQ(out2[2], "OOO");
  EXPECT_EQ(out2[3], "MMMMM");
}

#endif
