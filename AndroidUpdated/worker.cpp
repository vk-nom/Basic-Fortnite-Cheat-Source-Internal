#include "include.h"
#include "MainFunctions.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include "hooks/detours.h"
#pragma comment(lib, "detours.lib")


namespace worker {


	VOID(*FortniteFree)(PVOID) = nullptr;


	BOOLEAN MaskCompare(PVOID buffer, LPCSTR pattern, LPCSTR mask) {
		for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
			if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
				return FALSE;
			}
		}

		return TRUE;
	}

	PBYTE SigScan(LPCSTR pattern, LPCSTR mask) {
		MODULEINFO info = { 0 };
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

		info.SizeOfImage -= static_cast<DWORD>(strlen(mask));

		for (auto i = 0UL; i < info.SizeOfImage; ++i) {
			auto addr = reinterpret_cast<PBYTE>(info.lpBaseOfDll) + i;
			if (MaskCompare(addr, pattern, mask)) {
				return addr;
			}
		}
	}


	struct FTransform
	{
		FQuat rot;
		Vector3 translation;
		char pad[4];
		Vector3 scale;
		char pad1[4];


		D3DMATRIX ToMatrixWithScale()
		{
			D3DMATRIX m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			float x2 = rot.x + rot.x;
			float y2 = rot.y + rot.y;
			float z2 = rot.z + rot.z;

			float xx2 = rot.x * x2;
			float yy2 = rot.y * y2;
			float zz2 = rot.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale.z;

			float yz2 = rot.y * z2;
			float wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * scale.z;
			m._23 = (yz2 + wx2) * scale.y;

			float xy2 = rot.x * y2;
			float wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * scale.y;
			m._12 = (xy2 + wz2) * scale.x;

			float xz2 = rot.x * z2;
			float wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * scale.z;
			m._13 = (xz2 - wy2) * scale.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};


	D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
	{
		D3DMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}


	Vector3 GetBonePos(DWORD_PTR mesh, int id)
	{

		auto bonearray = ReadPointer(mesh, 0x4A8);

		if (bonearray == NULL)
		{
			bonearray = ReadPointer(mesh, 0x4A8 + 0x10);
		}

		FTransform bone = ReadFTRANSFORM(bonearray, (id * 0x30));


		FTransform ComponentToWorld = ReadFTRANSFORM(mesh, 0x1C0);

		D3DMATRIX Matrix;

		Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

		return Vector3(Matrix._41, Matrix._42, Matrix._43);
	}

	D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
	{
		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}




	Vector3 W2S_IT(Vector3 WorldLocation, DWORD_PTR LocalPlaya, DWORD_PTR RootCompa)
	{
		Vector3 Screenlocation = Vector3(0, 0, 0);
		Vector3 Camera;
		auto chain69 = Readuintptr_t(LocalPlaya, 0xa8);
		uint64_t chain699 = Readuintptr_t(chain69, 8);
		Camera.x = 0;
		Camera.y = 0;


		//Camera Rotation
		Camera.x = ReadFloat(chain699, PitchOffset);  //PITCH
		Camera.y = ReadFloat(RootCompa, YawOffset);  //YAW

		float test = asin(Camera.x);
		float degrees = test * (180.0 / M_PI);
		Camera.x = degrees;

		if (Camera.y < 0)
			Camera.y = 360 + Camera.y;

		D3DMATRIX tempMatrix = Matrix(Camera);
		Vector3 vAxisX, vAxisY, vAxisZ;

		vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
		uint64_t chain = Readuint64_t(LocalPlaya, 0x70);
		uint64_t chain1 = Readuint64_t(chain, 0x98);
		uint64_t chain2 = Readuint64_t(chain1, 0x130);

		Vector3 vDelta = WorldLocation - ReadVector3(chain2, 0x10);
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;


		float FovAngle = 80.0f / ((*(float*)(chain699 + 0x590)) / 1.19f);

		float ScreenCenterX = 960;
		float ScreenCenterY = 540;


		Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.z = 0;
		return Screenlocation;
	}



	VOID Free(PVOID buffer) {
		FortniteFree(buffer);
	}

	//Credits to https://github.com/homeless1337 for this function
	std::string GetObjectName(uintptr_t Object) {
		static uintptr_t addr = 0;

		if (!addr) {
			addr = (uintptr_t)SigScan(xorstr(GetNameByIndexSig), xorstr(GetNameByIndexMask));
			if (!addr) {
				std::string errorcool = "Can't find GetNameByIndex Signature at ";
				std::string errorcool2 = errorcool + GetNameByIndexSig;
				MessageBoxA(NULL, errorcool2.c_str(), xorstr("Error dm Android#1336 on Discord if you see that"), MB_OK);
				exit(0);
			}
		}


		if (Object == NULL) {
			return xorstr("");
		}

		auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(int* index, FString * res)>(addr);

		int index = *(int*)(Object + 0x18);

		FString result;
		fGetObjName(&index, &result);

		if (result.c_str() == NULL) {
			return xorstr("");
		}


		std::wstring result_str1 = result.toWstring();
		std::string result_str(result_str1.begin(), result_str1.end());

		if (result.c_str() != NULL) {
			Free(result.c_str());
		}
			

		return result_str;
	}





	
	BOOLEAN Start()
	{
		// FreeFN
		auto addr = SigScan(xorstr(FreeSig), xorstr(FreeMask));
		FortniteFree = reinterpret_cast<decltype(FortniteFree)>(addr);
		if (!addr) {
			std::string errorcooll = "Can't find Free Signature at ";
			std::string errorcooll2 = errorcooll + FreeSig;
			MessageBoxA(NULL, errorcooll2.c_str(), xorstr("Error dm Android#1336 on Discord if you see that"), MB_OK);
		}

		return TRUE;
	}
}
