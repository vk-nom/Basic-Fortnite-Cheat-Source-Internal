#pragma once

#define UworldOffset 0x9645CC0
#define GameinstanceOffset 0x180
#define LocalPlayersOffset 0x38
#define PlayerControllerOffset 0x30
#define LocalPawnOffset 0x2A0 
#define PlayerStateOffset 0x240
#define RootComponentOffset 0x130 
#define LevelsOffset 0x138
#define MeshOffset 0x280 
#define PitchOffset 0x7F8
#define YawOffset 0x12C

LPCSTR FreeSig = "\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x53\x48\x83\xEC\x20\x48\x89\x7C\x24\x00\x48\x8B\xD9\x48\x8B\x3D\x00\x00\x00\x00\x48\x85\xFF";
LPCSTR FreeMask = "xxxxx????xxxxxxxxx?xxxxxx????xxx";

LPCSTR GetNameByIndexSig = "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x45\x33\xF6\x48\x8B\xF2\x44\x39\x71\x04\x0F\x85\x00\x00\x00\x00\x8B\x19\x0F\xB7\xFB\xE8\x00\x00\x00\x00\x8B\xCB\x48\x8D\x54\x24";
LPCSTR GetNameByIndexMask = "xxxx?xxxx?xxxxxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxx????xxxxxx????xxxxxx";