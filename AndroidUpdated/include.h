#pragma once

//Basic Includes
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <stdio.h>
#include <psapi.h>
#include <intrin.h>
#include <string>
#include <vector>

//Dear imgui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

//Hooks
#include "hooks/MinHook.h"
#pragma comment(lib, "minhook.lib")
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

//Files
#include "class.h"
#include "offsets.h"
#include "configs.h"
#include "mainfunctions.h"
#include "worker.h"
