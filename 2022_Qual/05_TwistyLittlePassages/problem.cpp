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

int main()
{
  int T;
  std::cin >> T;

  for(int t=0; t<T; t++)
  {
      int N;
      int K;
      std::cin >> N;
      std::cin >> K;

      std::vector<int> passagesFrom;
      std::vector<bool> visited;
      passagesFrom.resize(N+1, 0);
      visited.resize(N+1, false);

      bool bExit = false;
      int lim = std::min(K, N);
      for(int i=0; i<lim; i++)
      {
        int room;
        int passages;
        std::cin >> room;
        std::cin >> passages;

        visited[room] = true;
        passagesFrom[room] = passages;

        int dest = room;
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
        std::cout << "T " << dest << std::endl << std::flush;
      }

      int totalPassages = 0;
      int visitedRooms = 0;
      int unvisitedRooms = 0;

      for(int i=1; i<=N; i++)
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
      int guess = totalPassages + avgPassages * unvisitedRooms;
      guess /= 2;

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
