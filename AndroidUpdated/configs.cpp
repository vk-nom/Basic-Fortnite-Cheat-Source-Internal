#include "include.h"

CONFIGS Configs = { 0 };

namespace InitSettings 
{
	VOID InitializeSettings()
	{
		//esp
		Configs.Boxes = false;
		Configs.Visuals = false;
		Configs.Skeletons = false;
		Configs.PlayerLines = false;
	}
}