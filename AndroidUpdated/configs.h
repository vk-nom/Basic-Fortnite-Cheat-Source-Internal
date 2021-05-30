#pragma once

typedef struct AndroidUpdatedSettings{

	//esp
	bool Boxes;
	bool Visuals;
	bool Skeletons;
	bool PlayerLines;

} CONFIGS;

extern CONFIGS Configs;

namespace InitSettings
{
	VOID InitializeSettings();
}