#include "Math/Vector3.h"

int main()
{
	Hiwoong::Vector3 test(0, 0, 1);

	return (test.Length() == 1) ? 0 : 1;
}
