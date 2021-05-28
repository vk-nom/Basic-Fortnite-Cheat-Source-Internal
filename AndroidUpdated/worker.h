#pragma once



namespace AndroidNiceThings {
	static uint64_t BaseAddr = (uint64_t)GetModuleHandleA(NULL);
	static DWORD_PTR Uworld;
	static DWORD_PTR LocalPawn;
	static DWORD_PTR PlayerState;
	static DWORD_PTR Localplayer;
	static DWORD_PTR Rootcomp;
	static DWORD_PTR PlayerController;
	static DWORD_PTR Persistentlevel;
	static PVOID TargetPawn;
	static PVOID GetPlayerName;
}



#define M_PI 3.14159265358979323846264338327950288419716939937510
#define RELATIVE_ADDR(addr, size) ((PBYTE)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

#define ReadPointer(base, offset) (*(PVOID *)(((PBYTE)base + offset)))
#define ReadDWORD(base, offset) (*(DWORD *)(((PBYTE)base + offset)))
#define ReadDWORD_PTR(base, offset) (*(DWORD_PTR *)(((PBYTE)base + offset)))
#define ReadFTRANSFORM(base, offset) (*(FTransform *)(((PBYTE)base + offset)))
#define ReadInt(base, offset) (*(int *)(((PBYTE)base + offset)))
#define ReadFloat(base, offset) (*(float *)(((PBYTE)base + offset)))
#define Readuintptr_t(base, offset) (*(uintptr_t *)(((PBYTE)base + offset)))
#define Readuint64_t(base, offset) (*(uint64_t *)(((PBYTE)base + offset)))
#define ReadVector3(base, offset) (*(Vector3 *)(((PBYTE)base + offset)))

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float number) const {
		return Vector3(x * number, y * number, z * number);
	}
};




struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};




namespace worker {
	Vector3 GetBonePos(DWORD_PTR mesh, int id);

	Vector3 W2S_IT(Vector3 WorldLocation, DWORD_PTR LocalPlaya, DWORD_PTR RootCompa);

	std::string GetObjectName(uintptr_t Object);

	VOID Free(PVOID buffer);

	PBYTE SigScan(LPCSTR pattern, LPCSTR mask);

	namespace _SpoofCallInternal {
		extern "C" PVOID RetSpoofStub();

		template <typename Ret, typename... Args>
		inline Ret Wrapper(PVOID shell, Args... args) {
			auto fn = (Ret(*)(Args...))(shell);
			return fn(args...);
		}

		template <std::size_t Argc, typename>
		struct Remapper {
			template<typename Ret, typename First, typename Second, typename Third, typename Fourth, typename... Pack>
			static Ret Call(PVOID shell, PVOID shell_param, First first, Second second, Third third, Fourth fourth, Pack... pack) {
				return Wrapper<Ret, First, Second, Third, Fourth, PVOID, PVOID, Pack...>(shell, first, second, third, fourth, shell_param, nullptr, pack...);
			}
		};

		template <std::size_t Argc>
		struct Remapper<Argc, std::enable_if_t<Argc <= 4>> {
			template<typename Ret, typename First = PVOID, typename Second = PVOID, typename Third = PVOID, typename Fourth = PVOID>
			static Ret Call(PVOID shell, PVOID shell_param, First first = First{}, Second second = Second{}, Third third = Third{}, Fourth fourth = Fourth{}) {
				return Wrapper<Ret, First, Second, Third, Fourth, PVOID, PVOID>(shell, first, second, third, fourth, shell_param, nullptr);
			}
		};
	}

	template <typename Ret, typename... Args>
	Ret SpoofCall(Ret(*fn)(Args...), Args... args) {
		static PVOID trampoline = nullptr;
		if (!trampoline) {
			trampoline = worker::SigScan("\xFF\x27", "xx");
			if (!trampoline) {
				MessageBox(NULL, xorstr("failed to find valid trampoline."), xorstr("error"), MB_OK);
				ExitProcess(0);
			}
		}

		struct {
			PVOID Trampoline;
			PVOID Function;
			PVOID Reg;
		} params = {
			trampoline,
			reinterpret_cast<void*>(fn),
		};

		return _SpoofCallInternal::Remapper<sizeof...(Args), void>::template Call<Ret, Args...>(&_SpoofCallInternal::RetSpoofStub, &params, args...);
	}

	BOOLEAN Start();
}