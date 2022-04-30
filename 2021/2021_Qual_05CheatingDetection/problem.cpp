#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef TEST
// https://google.github.io/googletest/quickstart-cmake.html
#include <gtest/gtest.h>
#endif

// Includes for problem here
#include <vector>
#include <string>

#include <algorithm>
#include <functional>
#include <numeric>
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Define Utilities
#define iow(w) std::setw(w)
#define iop(p) std::setprecision(p)
#define iowp(w,p) iow(w) << iop(p)

#ifdef LOCAL
  #define LOG(...) std::cout << __VA_ARGS__
#else
  #define LOG(...) 
#endif


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
  std::vector<std::string> answers;
};

struct CaseSolution {
  int guess;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main Function

#ifndef NOMAIN
int main(int argc, char** argv)
{
  #ifdef LOCAL
    std::cout << "Starting" << std::endl;
    std::ifstream fin;

    std::string file = "tests";

    if(argc == 2)
    {
      file = std::string(argv[1]);
    }

    std::string filename = "tests/" + file + ".txt";
    
    fin.open(filename);
    if(!fin.is_open())
    {
      std::cout << "Create a tests.txt or other file" << std::endl;
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
    int P = 0;
    in >> P;

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

    for(int i=0; i<100; i++)
    {
      std::string s;
      in >> s;
      p.answers.push_back(s);
    }

    return p;
}

void OutputSolution(CaseSolution solution, std::ostream& out, int case_num) {
    out << "Case #" << case_num << ": " << solution.guess << std::endl;
}

bool compare(int a, int b, float* data)
{
  //std::cout << "data[" << a << "] vs data[" << b << "]:   " << data[a] << " vs " << data[b] << std::endl;
  return data[a] < data[b];
}

float sig(float x)
{
  return 1.0 / (1.0 + exp(-x));
}

CaseSolution SolveCase(CaseDetails details) {
    CaseSolution solution;
    solution.guess = 0;

    float question_probs[10000];
    for(int i=0; i<10000; i++)
    {
      int right = 0;
      int wrong = 0;

      for(int j=0; j<100; j++)
      {
        char c = details.answers[j][i];
        if(c == '1')
        {
          right++;
        }
        else
        {
          wrong++;
        }
      }

      float prob = (float)right / (float)(right + wrong);
      question_probs[i] = (prob);
    }

    int question_indices[10000];
    std::iota(std::begin(question_indices), std::end(question_indices), 0);
    std::sort(std::begin(question_indices), std::end(question_indices), std::bind(compare, std::placeholders::_1, std::placeholders::_2, question_probs));

    float student_probs[100] = {0};
    for(int i=0; i<100; i++)
    {
      int right = 0;
      int wrong = 0;
      for(int j=0; j<10000; j++)
      {
        char c = details.answers[i][j];
        if(c == '1')
        {
          right++;
        }
        else
        {
          wrong++;
        }
      }
      float prob = (float)right / (float)(right + wrong);
      student_probs[i] = (prob);
    }

    /*
    for(int i=0; i<10000; i++)
    {
      std::cout << question_probs[question_indices[i]] << ",";
    }
    */

    const float FRAC = 0.1;

    // Several attempts
    /*
    float pCheatBayes[100];
    float inversions[100] = {0};
    float inversionProbs[100] = {0};
    int correctEasy[100] = {0};
    int correctDiff[100] = {0};
    int randomInversions[100] = {0};
    for(int i=0; i<100; i++)
    {
      pCheatBayes[i] = 0.01;
    }


    for(int i=0; i<FRAC * 10000; i++)
    {
      int id = question_indices[i];
      int ie = question_indices[10000 - i - 1];

      float pd = question_probs[id];
      float pe = question_probs[ie];

      //std::cout << pe << " vs " << pd << std::endl;

      for(int j=0; j<100; j++)
      {
        
        char easy = details.answers[j][ie];
        char diff = details.answers[j][id];

        float p = (1 - pe)*pd;
          inversionProbs[j] += p;

        if (easy == '0' && diff == '1')
        {
          

          float f1 = pCheatBayes[j] / (pd * (1-pe));
          float f2 = (0.5 + 0.5*pd)*(0.5 * (1-pe));
          pCheatBayes[j] = f1*f2;
                    
          inversions[j] += 1.0;
        }

        if(easy == '1')
        {
          correctEasy[j]++;
        }
        if(diff == '1')
        {
          correctDiff[j]++;
        }

        randomInversions[i] += (int) (details.answers[j][i] != details.answers[j][i+1]);
      }
    }

    float expectedInversions[100] = {0};
    int deDiff[100] = {0};
    for(int i=0; i<100; i++)
    {
      float pInvRand = student_probs[i] * (1 - student_probs[i]);
      expectedInversions[i] = pInvRand * FRAC * 10000;
      deDiff[i] = correctEasy[i] - correctDiff[i];
      inversions[i] = inversions[i];
    }

    float cheatingScore[100] = {0};
    for(int i=0; i<100; i++)
    {
      cheatingScore[i] = inversions[i]/randomInversions[i];
    }
    */


    // Working Solution
    // Based on solution of ksun48, accessed through Google Code Jam site
    // For each problem solved right (in order of easiest to most difficult)
    // - Add up difference in student probability and all incorrect question probabilites so far
    // - The sum becomes the score, normalized by some amounts
    // Attempted to modify here to use probabilities instead of counts
    // - Did not succeed
    float topPerformersScore[100] = {0};
    for(int j=0; j<100; j++)
    {
      float right = 0;
      float easyWrong = 0;
      int numEasyWrong = 0;
      float inv = 0;
      for(int i=0; i<10000; i++)
      {
        int ie = question_indices[10000 - i - 1];
        char c = details.answers[j][ie];

        if(c == '1')
        {
          right += 1.0;//question_probs[ie];
          inv += easyWrong;//-(easyWrong - numEasyWrong*student_probs[j]);
        }
        else
        {
          easyWrong += 1; //question_probs[ie];
          numEasyWrong += 1;
        }
      }
      float score = (float)inv / right/easyWrong;
      topPerformersScore[j] = score;
    }

    // Attempt: Calculate Variance of probabilities of questions solved wrongly
    // Did not see any major indications of cheating
    /*
    float varianceProbMissed[100] = {0};
    for(int j=0; j<100; j++)
    {
      std::vector<float> pMissed;
      for(int i=0; i<10000; i++)
      {
        char c = details.answers[j][i];
        if(c == '0')
        {
          pMissed.push_back(question_probs[i]);
        }
      }

      float mean = 0;
      for(int i=0; i<pMissed.size(); i++)
      {
        mean += pMissed[i];
      }
      mean /= (pMissed.size() + 1);

      float var = 0;
      for(int i=0; i<pMissed.size(); i++)
      {
        float diff = (mean - pMissed[i]);
        diff = diff * diff / pMissed.size();
        var += diff;
      }
      varianceProbMissed[j] = var;
    }
    */


    // Attempt to estimate each Q and S value
    // Could potential use to improve probability estimates
    // Currently does not limit values to [-3, 3]
    // But solves for student probabilities in (with debugging file) correct ascending order
    // - Cheaters have noticable jump
    /*
    float estStudentScores[100];
    for(int i=0; i<100; i++) estStudentScores[i] = student_probs[i]*6 - 3.0;
    float estQuestionScores[10000];
    for(int i=0; i<10000; i++) estQuestionScores[i] = question_probs[i]*6 - 3.0;

    for(int k=0; k<5; k++)
    {
      for(int i=0; i<10000; i++)
      {
        for(int j=0; j<100; j++)
        {
          char c = details.answers[j][i];
          float y = (c == '1') ? 1 : 0;

          float Sj = estStudentScores[j];
          float Qi = estQuestionScores[i];
          float sig = 1.0 / (1 + exp(-(Sj-Qi)));

          float del = -2 * (sig - y) * sig * (1 - sig);

          estStudentScores[j] = Sj + 0.01*del;
          estQuestionScores[i] = Qi - 0.01*del;
        }
      }
    }
    */

    // Output for debugging/testing
    /*
    for(int i=0; i<100; i++)
    {
      LOG( iow(3) << i+1 \
                << ": "<< iow(5) << deDiff[i] \
                << iow(5) << (int)inversions[i] \
                << iowp(15,3) << expectedInversions[i] \
                << iowp(15,3) << randomInversions[i] \
                << std::fixed << iowp(15,5) << topPerformersScore[i] \
                << std::fixed << iowp(15,5) << cheatingScore[i] \
                << std::fixed << iowp(15,5) << varianceProbMissed[i] \
                << std::fixed << iowp(15,5) << estStudentScores[i] << std::endl);
    } 
    */

    float* maximizing = topPerformersScore;

    float max = maximizing[0];
    int maxInd = 0;
    for(int i=0; i<100; i++)
    {
      if (maximizing[i] > max)
      {
        max = maximizing[i];
        maxInd = i;
      }
    }
    solution.guess = maxInd + 1;

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
