
#include <iostream>
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include <cmath>

namespace
{
    Hiwoong::Vector3 testVector(3.0f, 4.0f, 0.0f);
}



    bool TestVector3Constructor()
    {
        Hiwoong::Vector3 vector(1.0f, 2.0f, 3.0f);

        return vector.x == 1.0f && vector.y == 2.0f && vector.z == 3.0f;
    }


    bool TestGetLength()
    {
        float x = 1.0f;
        float y = 2.0f;
        float z = 3.0f;
        Hiwoong::Vector3 vector(x, y, z);

        return vector.Length() == sqrt(x*x + y*y + z*z);
    }


    bool GetNormalized()
    {
        Hiwoong::Vector3 normal = testVector.Normalized();
        
        Hiwoong::Vector3 testNormal(0.6f, 0.8f, 0);
        return testNormal == normal;
    }


    bool GetDot()
    {
        Hiwoong::Vector3 a(1,2,3);
        Hiwoong::Vector3 b(4, 5, 6);

        return Hiwoong::Vector3::Dot(a,b) == 32;
    }

    bool TestCross()
    {
        Hiwoong::Vector3 forward(0, 0, 1);
        Hiwoong::Vector3 left(-1, 0, 0);
        Hiwoong::Vector3 expectedUp(0, -1, 0);

        return Hiwoong::Vector3::Cross(forward, left) == expectedUp;
    }

    int main()
    {
        Hiwoong::Vector4 vector(1,2,3,4);

        std::cout << vector.x<< vector.y<< vector.z<< vector.w << std::endl;

        if (TestCross() == true)
        {
            std::cout << "Success" << std::endl;
        }
        else
        {
            std::cout << "failure" << std::endl;
        }

        return 0;
    }

