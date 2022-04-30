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

#ifdef LOCAL
  #define LOG(...) std::cerr << __VA_ARGS__ << std::endl
#else
  #define LOG(...) 
#endif

int main()
{
  int T;
  std::cin >> T;

  for(int t=0; t<T; t++)
  {
      unsigned long int N;
      unsigned long int K;
      std::cin >> N;
      std::cin >> K;

      LOG("Problem #: " << t << " N=" << N << ", K=" << K);

      std::vector<unsigned long int> passagesFrom;
      std::vector<bool> visited;
      passagesFrom.resize(N+1, 0);
      visited.resize(N+1, false);

      bool bExit = false;
      int lim = std::min(K, N);
      for(int i=0; i<K; i++)
      {
        unsigned long int room = 0;
        unsigned long int passages = 0;
        std::cin >> room;
        std::cin >> passages;

        //LOG("  room=" << room << " with passages " << passages);

        visited[room] = true;
        passagesFrom[room] = passages;

        unsigned long int dest = room;
        while(visited[dest])
        {
          dest = (dest + 1) % (N + 1);
          if (dest == 0)
          {
            dest = 1;
          }

          if (dest == room)
          {
            bExit = true;
            break;
          }
        }

        if(bExit)
        {
          break;
        }

        if(i < K-1)
        {
          //LOG("  go to " << dest);
          std::cout << "T " << dest << std::endl << std::flush;
        }
      }

      unsigned long int totalPassages = 0;
      unsigned long int visitedRooms = 0;
      unsigned long int unvisitedRooms = 0;

      for(unsigned long int i=1; i<=N; i++)
      {
        if (visited[i])
        {
          totalPassages += passagesFrom[i];
          visitedRooms += 1;
        }
        else
        {
          unvisitedRooms += 1;
        }
      }

      float avgPassages = (float)totalPassages / visitedRooms;
      unsigned long int guess = totalPassages + avgPassages * unvisitedRooms;
      guess /= 2;

      //LOG("  estimate " << guess);
      std::cout << "E " << guess << std::endl << std::flush;
  }

  return 0;
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
