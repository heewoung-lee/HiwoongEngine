
#include <iostream>
#include "Math/Vector3.h"

bool TestVector2Constructor()
{
    Hiwoong::Vector3 vector(1.0f, 2.0f, 3.0f);

    return vector.x == 1 && vector.y == 2;
}

int main()
{
    if (TestVector2Constructor() == false)
    {
        std::cout << "[FAILED] Vector3 Constructor\n";
        return 1;
    }

    std::cout << "[PASSED] Vector3 Constructor\n";
    return 0;
}

