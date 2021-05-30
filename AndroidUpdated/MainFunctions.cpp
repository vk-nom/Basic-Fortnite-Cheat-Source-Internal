/*
This source is free on githuh: https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal
If you sell this I will make you famous (in a bad way)
*/

#include "include.h"
#include <math.h>
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <wchar.h>
uint64_t BaseAddy = (uint64_t)GetModuleHandleA(NULL);
ImFont* m_pFont;
int hitbox;
float smooth = 1.0f;

namespace AndroidUpdated {



	float DrawNormalText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		std::stringstream stream(text); std::string line;
		float y = 0.0f; int i = 0;
		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
			if (center) { window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str()); }
			else { window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str()); }
			y = pos.y + textSize.y * (i + 1); i++;
		}
		return y;
	}


	std::string TextFormat(const char* format, ...)
	{
		va_list argptr;
		va_start(argptr, format); char buffer[2048]; vsprintf(buffer, format, argptr); va_end(argptr);
		return buffer;
	}

	void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
		float lineW = (W / 3); float lineH = (H / 5);

		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness); ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness); ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness); ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness); ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	}


	VOID AddLine(float x, float y, float xx, float yy, ImU32 color, int tickness) { ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(xx, yy), color, tickness); }



	BOOLEAN showMenu = FALSE;

	ID3D11Device* device = nullptr; ID3D11DeviceContext* immediateContext = nullptr; ID3D11RenderTargetView* renderTargetView = nullptr;

	WNDPROC WndProcOriginal = nullptr; HRESULT(*PresentOriginal)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr; HRESULT(*ResizeOriginal)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

	ImGuiWindow& BeginScene() {
		//Here we start the DirectX 11 Frame of Dear ImGui
		ImGui_ImplDX11_NewFrame();

		//Here we push some vars that we want for our Dear ImGui
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(xorstr("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

		auto& io = ImGui::GetIO();
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		//Here we set our Dear ImGui window size
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		return *ImGui::GetCurrentWindow();
	}


	void Style() {
		//Here we have all of the colors yall skids can change them to whatever u want
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.058823529f, 0.058823529f, 0.058823529f, 0.5f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
		style.Colors[ImGuiCol_Border] = ImColor(15, 15, 15, 255);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.09f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 0.88f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15f, 0.60f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.15f, 0.60f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.02f, 0.60f, 0.22f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	}



	VOID EndScene(ImGuiWindow& window)
	{
		window.DrawList->PushClipRectFullScreen();
		ImGui::End();


		//Here we Clear the style color just to be sure to re-push it later
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);



		//Here we push the style color (to make it apply to the menu)
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.058823529f, 0.058823529f, 0.058823529f, 0.5f));


		static bool open = true;

		//Here we check if our menu is open (if we pressed insert)
		if (showMenu)
		{

			Style();


			//Here we set our menu first position on the screen
			ImGui::SetNextWindowPos(ImVec2((width / 2) - 230, (height / 2) - 150), ImGuiCond_FirstUseEver);

			//Here we begin our menu
			ImGui::Begin(xorstr("https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal"), &open, ImVec2(460, 300), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
			{
				ImGui::Text(xorstr("Yo Im here https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal"));


				ImGui::Separator();
				ImGui::Text(xorstr("Player ESP"));
				ImGui::Checkbox(xorstr("Box"), &Configs.Boxes);
				ImGui::Checkbox(xorstr("Skeleton"), &Configs.Skeletons);
				ImGui::Checkbox(xorstr("Snaplines"), &Configs.PlayerLines);
			}
			//Here we end our menu
			ImGui::End();


		}
		//Here we Clear the style color
		ImGui::PopStyleColor();

		//Here we render our menu to imgui
		ImGui::Render();
	}






	__declspec(dllexport) LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		//Here we check if we press insert
		if (msg == WM_KEYUP && (wParam == VK_INSERT)) {
			//if we press insert then it will set showMenu to true, if we re - press it then it will set it to false
			showMenu = !showMenu;
			ImGui::GetIO().MouseDrawCursor = showMenu;
		}

		//And here we check if the showMenu variable that we changed above is true
		if (showMenu) {
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
			return TRUE;
		}

		return CallWindowProc(WndProcOriginal, hWnd, msg, wParam, lParam);
	}






	static int VisDist = 500;
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;


	using f_present = HRESULT(__stdcall*)(IDXGISwapChain* pthis, UINT sync_interval, UINT flags);
	f_present o_present = nullptr;
	HRESULT __stdcall hk_present(IDXGISwapChain* pSwapChain, UINT sync_interval, UINT flags)
	{
		static HWND hWnd = 0;

		if (!device) {
			pSwapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
			device->GetImmediateContext(&immediateContext);
			ID3D11Texture2D* renderTarget = nullptr;
			pSwapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
			device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
			renderTarget->Release();
			ID3D11Texture2D* backBuffer = 0;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
			D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
			backBuffer->GetDesc(&backBufferDesc);
			hWnd = FindWindow(xorstr(L"UnrealWindow"), xorstr(L"Fortnite  "));
			if (!width) { WndProcOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc))); }
			width = (float)backBufferDesc.Width;
			height = (float)backBufferDesc.Height;
			backBuffer->Release();
			m_pFont = ImGui::GetIO().Fonts->AddFontDefault();
			ImGui_ImplDX11_Init(hWnd, device, immediateContext);
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);



		//Here we define our window variable, we will use it everywhere to draw
		auto& window = BeginScene();

		// hehehe watermark
		DrawNormalText(m_pFont, TextFormat(xorstr("https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal")), ImVec2(20, 20), 16.0f, ImColor(255, 255, 255, 255), false);


		do {



			//Get the main Uworld address with Uworld Offset
			AndroidNiceThings::Uworld = ReadDWORD_PTR(BaseAddy, UworldOffset);
			//if it can't get the uworld address then break the loop
			if (!AndroidNiceThings::Uworld) break;

			//Get the GameInstance address with uworld + 0x180
			DWORD_PTR Gameinstance = ReadDWORD_PTR(AndroidNiceThings::Uworld, 0x180);
			//if it can't get the GameInstance address then break the loop
			if (!Gameinstance) break;

			//Get LocalPlayers address with uworld + 0x180
			DWORD_PTR LocalPlayers = ReadDWORD_PTR(Gameinstance, 0x38);
			//if it can't get the LocalPlayers address then break the loop
			if (!LocalPlayers) break;
			
			//Get the LocalPlayer address with a pointer to 0x0
			AndroidNiceThings::Localplayer = ReadDWORD_PTR(LocalPlayers, 0x0);
			//if it can't get the LocalPlayer address then break the loop
			if (!AndroidNiceThings::Localplayer) break;

			//Get the PlayerController address with LocalPlayer + 0x30
			AndroidNiceThings::PlayerController = ReadDWORD_PTR(AndroidNiceThings::Localplayer, 0x30);
			//if it can't get the PlayerController address then break the loop
			if (!AndroidNiceThings::PlayerController) break;

			//Get the LocalPawn Pointer address with PlayerController + 0x2A0
			auto LocalPawnInAuto = ReadPointer(AndroidNiceThings::PlayerController, 0x2A0);
			//if it can't get the LocalPawn address then break the loop
			if (!LocalPawnInAuto) break;

			//Get the LocalPawn address with PlayerController + 0x2A0
			AndroidNiceThings::LocalPawn = ReadDWORD_PTR(AndroidNiceThings::PlayerController, 0x2A0);
			//if it can't get the LocalPawn address then break the loop
			if (!AndroidNiceThings::LocalPawn) break;
			
			
			//Get the PlayerState address with LocalPawn + 0x240
			AndroidNiceThings::PlayerState = ReadDWORD_PTR(AndroidNiceThings::LocalPawn, 0x240);
			if (!AndroidNiceThings::PlayerState) break;

			//Get the RootComponent address with LocalPawn + 0x130
			AndroidNiceThings::Rootcomp = ReadDWORD_PTR(AndroidNiceThings::LocalPawn, 0x130);
			if (!AndroidNiceThings::Rootcomp) break;


			//If the LocalPawn is not 0 (he is not lobby)
			if (AndroidNiceThings::LocalPawn != 0) {
				//Get the LocalPlayerID address with LocalPawn + 0x18
				AndroidNiceThings::localplayerID = ReadInt(AndroidNiceThings::LocalPawn, 0x18);
			}


			//Define a simple vector to do later a cache system
			std::vector<PVOID> playerPawns;


			//Here we have our first for, that is used to loop the levels
			for (auto li = 0UL; li < ReadDWORD(AndroidNiceThings::Uworld, 0x138 + sizeof(PVOID)); ++li) {
				//Here we define the  levels
				auto levels = ReadPointer(AndroidNiceThings::Uworld, 0x138);
				
				if (!levels) break;

				//Here we define every single level
				auto level = ReadPointer(levels, li * sizeof(PVOID));
				
				if (!level) continue;


				//Here we have another for that is used this time to get the player list count
				for (auto ai = 0UL; ai < ReadDWORD(level, 0x98 + sizeof(PVOID)); ++ai) {
					//Here we define the actors (the pawns)
					auto actors = ReadPointer(level, 0x98);
					if (!actors) break;
					//Here we define every pawn (every actor)
					auto pawn = reinterpret_cast<UObject*>(ReadPointer(actors, ai * sizeof(PVOID)));

					if (!pawn || pawn == LocalPawnInAuto) continue;

					//Here we define the curactor id (that is the other players actor's ids)
					int PawnId = ReadInt(pawn, 0x18);

					//If the PawnId (that is the other players ids) is equal to the localplayerID (that is your player id) that means that is a real player so
					if (PawnId == AndroidNiceThings::localplayerID) {
						playerPawns.push_back(pawn);

					}





				}
			}


			//Here we do another for with the pawn list we had push before and we define a new variable named pawn
			for (auto pawn : playerPawns) {

				

				//Getting PlayerState address with pawn + 0x240
				auto state = ReadPointer(pawn, 0x240);
				
				if (!state) continue;

				//Getting EntityMesh address with pawn + 0x280
				DWORD_PTR player_mesh = ReadDWORD_PTR(pawn, 0x280); //mesh
				
				if (!player_mesh) continue;

				










				//Gettings Headposition without worldtoscreen
				Vector3 HeadPos = worker::GetBonePos(player_mesh, 66);

				//Get the Rootposition of LocalPawn (so the localplayer)
				Vector3 LocalPawnRootPos = ReadVector3(AndroidNiceThings::Rootcomp, 0x11C);


				//Doing worldtoscreen on HeadPosition
				Vector3 HeadPos_w2s = worker::W2S_IT(Vector3(HeadPos.x, HeadPos.y, HeadPos.z + 15), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);


				//Getting BottomPosition
				Vector3 bottom = worker::W2S_IT(worker::GetBonePos(player_mesh, 0), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);


				//Define some colors in rgb
				ImColor SkeletonColor = ImColor(255, 255, 255);
				ImColor BoxCompleteColor = ImColor(255, 255, 255);



				//Getting CornerHeight by subtracting head.y and bottom.y
				float CornerHeight = abs(HeadPos_w2s.y - bottom.y);

				//Getting CornerWidth by doing the CornerHeight * 0.45
				float CornerWidth = CornerHeight * 0.45;


				//Getting the distance by doing sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)) to get the distance from the localplayer to the other players
				float distance = LocalPawnRootPos.Distance(HeadPos) / 100.f;

				//Here we subtract from the HeadPosition.x the center of the axis x of the screen in pixels (ex. if you have 1920 x 1080 screen this will do HeadPosition.x - (1920 / 2))
				auto dx = HeadPos_w2s.x - (width / 2);

				//Here we subtract from the HeadPosition.y the center of the axis y of the screen in pixels (ex. if you have 1920 x 1080 screen this will do HeadPosition.y - (1080 / 2))
				auto dy = HeadPos_w2s.y - (height / 2);
				auto dist = sqrtf(dx * dx + dy * dy);




				//Check if Boxes option is enabled
				if (Configs.Boxes) {

					//Draw the box
					DrawCorneredBox(HeadPos_w2s.x - (CornerWidth / 2), HeadPos_w2s.y, CornerWidth, CornerHeight, BoxCompleteColor, 2.0f);

				}

				//Check if Skeletons option is enabled
				if (Configs.Skeletons) {

					//Getting all the Bone Positions
					Vector3 Skel0 = worker::W2S_IT(worker::GetBonePos(player_mesh, 2), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel1 = worker::W2S_IT(worker::GetBonePos(player_mesh, 65), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel2 = worker::W2S_IT(worker::GetBonePos(player_mesh, 35), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel3 = worker::W2S_IT(worker::GetBonePos(player_mesh, 63), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel4 = worker::W2S_IT(worker::GetBonePos(player_mesh, 75), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel5 = worker::W2S_IT(worker::GetBonePos(player_mesh, 68), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel6 = worker::W2S_IT(worker::GetBonePos(player_mesh, 69), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					Vector3 Skel7 = worker::W2S_IT(worker::GetBonePos(player_mesh, 76), AndroidNiceThings::Localplayer, AndroidNiceThings::Rootcomp);
					
					//Drawing al the bone position
					AddLine(Skel0.x, Skel0.y, HeadPos_w2s.x, HeadPos_w2s.y, SkeletonColor, 0.5f);
					AddLine(Skel2.x, Skel2.y, HeadPos_w2s.x, HeadPos_w2s.y, SkeletonColor, 0.5f);
					AddLine(Skel3.x, Skel3.y, HeadPos_w2s.x, HeadPos_w2s.y, SkeletonColor, 0.5f);
					AddLine(Skel5.x, Skel5.y, Skel0.x, Skel0.y, SkeletonColor, 0.5f);
					AddLine(Skel4.x, Skel4.y, Skel0.x, Skel0.y, SkeletonColor, 0.5f); 
					AddLine(Skel6.x, Skel6.y, Skel5.x, Skel5.y, SkeletonColor, 0.5f);
					AddLine(Skel7.x, Skel7.y, Skel4.x, Skel4.y, SkeletonColor, 0.5f);

				}

				//Check if PlayerLines option is enabled
				if (Configs.PlayerLines) {

					//Drawing a Line from the bottom of the screen (width / 2, height / 2) to the HeadPosition of the player
					window.DrawList->AddLine(ImVec2(width / 2, height - 10), ImVec2(HeadPos_w2s.x, HeadPos_w2s.y), ImColor(255, 255, 255, 100));


				}


			}



		} while (FALSE);

		//If for some reasons the while above will break we end the imgui
		EndScene(window);

		return o_present(pSwapChain, sync_interval, flags);
	}

	__declspec(dllexport) HRESULT DoResize(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		ImGui_ImplDX11_Shutdown();
		renderTargetView->Release();
		immediateContext->Release();
		device->Release();
		device = nullptr;

		return ResizeOriginal(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	}

	BOOLEAN Start()
	{
		IDXGISwapChain* swapChain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		auto                 featureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		//Choose the OutputWindow where the esp should draw out, in this case Fortnite. The first string is the Fortnite class name, the second string is the Fortnite window name.
		sd.OutputWindow = FindWindow(xorstr(L"UnrealWindow"), xorstr(L"Fortnite  "));
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;

		//Check if the process uses DirectX 11
		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
			//If it doesn't use DirectX 11 then print a message and terminate the application.
			MessageBox(0, xorstr(L"The process hasn't DirectX 11"), xorstr(L"AndroidUpdated Error"), MB_ICONERROR);
			return FALSE;
		}

		auto table = *reinterpret_cast<PVOID**>(swapChain);
		auto present = table[8];
		auto resize = table[13];

		context->Release();
		device->Release();
		swapChain->Release();

		//Create the hook to initialize MinHook and ImGui to call hk_present
		MH_CreateHook(present, hk_present, reinterpret_cast<PVOID*>(&o_present));
		MH_EnableHook(present);

		//End the hook
		MH_CreateHook(resize, DoResize, reinterpret_cast<PVOID*>(&ResizeOriginal));
		MH_EnableHook(resize);

		return TRUE;
	}
}