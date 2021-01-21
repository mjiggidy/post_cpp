#include <iostream>
#include <set>
#include "shot.h"
#include "timecode.h"

int main(int argc, char* argv[])
{
	std::set<post::Shot> shots;

	try
	{
		shots.insert(post::Shot("A001C003_190112_R1CB", post::Timecode("59:52:00"), post::Timecode("40:00"), 23.98));
		shots.insert(post::Shot("A001C002_190112_R1CB", post::Timecode("59:51:00", 30), post::Timecode("40:00", 30), 29.97));
		shots.insert(post::Shot("A001C001_190112_R1CB", post::Timecode("59:52:00"), post::Timecode("40:00"), 23.98));
		shots.insert(post::Shot("A001C003_190112_R1CB", post::Timecode("59:52:00"), post::Timecode("40:00"), 23.98));
		shots.insert(post::Shot("A001C003_190112_R1CB", 86400, 600, 29.97));
	}
	catch (std::exception& e)
	{
		std::cout << "Nope. " << e.what() << std::endl;
	}

	for (const auto& testshot : shots)
	{
		std::cout << "Created shot " << testshot.GetShotName() << " (" << testshot.GetTimecodeStart() << " - " << testshot.GetTimecodeEnd() << " @ " << testshot.GetFrameRate() << ")" << std::endl;
	}

	return 0;
}
