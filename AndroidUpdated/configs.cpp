/*
This source is free on githuh: https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal
If you sell this I will make you famous (in a bad way)
*/

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