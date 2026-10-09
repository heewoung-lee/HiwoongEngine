#include "PathFinder/NavigationGridTests.h"
#include "PathFinder/AStarPathFinderTests.h"

namespace Hiwoong::Tests
{
    void RunMeshLoaderTests(TestRunner& testRunner);
}

int main()
{
    Hiwoong::Tests::TestRunner testRunner;

    Hiwoong::Tests::RunNavigationGridTests(testRunner);
    Hiwoong::Tests::RunAStarPathFinderTests(testRunner);
    Hiwoong::Tests::RunMeshLoaderTests(testRunner);

    if (testRunner.GetFailureCount() > 0)
    {
        return 1;
    }

    return 0;
}