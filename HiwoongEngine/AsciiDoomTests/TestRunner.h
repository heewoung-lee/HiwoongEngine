#pragma once

#include <iostream>

namespace Hiwoong::Tests
{
	class TestRunner
	{
	public:
		void Check(bool condition, const char* testName)
		{
			if (condition == true)
			{
				std::cout << "[PASS] " << testName << '\n';
				return;
			}

			std::cout << "[FAIL] " << testName << '\n';
			++failureCount;
		}

		int GetFailureCount() const
		{
			return failureCount;
		}

	private:
		int failureCount = 0;

	};
}

