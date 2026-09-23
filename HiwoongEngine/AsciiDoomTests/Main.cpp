#include "PathFinder/NavigationGridTests.h"
#include "PathFinder/AStarPathFinderTests.h"
int main()
{
    Hiwoong::Tests::TestRunner testRunner;

    Hiwoong::Tests::RunNavigationGridTests(testRunner);
    Hiwoong::Tests::RunAStarPathFinderTests(testRunner);

    if (testRunner.GetFailureCount() > 0)
    {
        return 1;
    }

    return 0;
}