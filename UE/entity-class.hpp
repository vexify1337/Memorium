#include "Source/Driver/driver.h"
#include "entity-structs.hpp"
#include "ida.hpp"
#include "stdafx.h"
#include "getoffsets.hpp"
#include <string>
#include <stdexcept>
#include <memory> // For std::unique_ptr

enum bone : int {
	Head = 110,
	Neck = 67,
	Chest = 66,
	Pelvis = 2,
	LShoulder = 9,
	LElbow = 10,
	LHand = 11,
	RShoulder = 38,
	RElbow = 39,
	RHand = 40,
	LHip = 71,
	LKnee = 72,
	LFoot = 73,
	RHip = 78,
	RKnee = 79,
	RFoot = 82,
	Root = 0
};

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};

enum offset {

	PrimaryPickupItemEntry = 0x350, // Class FortniteGame.FortPawn -> enum class EFortPickupSpawnSource PrimaryPickupItemEntry;
	displayname = 0x40, // Class ItemizationCoreRuntime.ItemDefinitionBase -> struct FText ItemName;
	tier = 0x9B, // Class FortniteGame.FortItemDefinition -> enum class EFortRarity Rarity;
	last_sumbit = 0x2E8, // Class Engine.Actor -> float LastSumbit;
	seconds = 0x148, // somewhere in uworld
	relative_location = 0x138, // Class Engine.SceneComponent -> struct FVector RelativeLocation;
	last_render = 0x30c, // Class Engine.Actor -> float LastRender;
	velocity = 0x168, // Class Engine.SceneComponent -> struct FVector Velocity;
	kills = 0x1224, // Class FortniteGame.AActor -> int32_t Kills;
	levels = 0x178, // Class Engine.World -> struct TArray<struct ULevel*> Levels;
	component_to_world = 0x1E0, // Class Engine.Actor -> struct FTransform ComponentToWorld;
	bone_array = 0x5A8, // Class Engine.SkinnedMeshComponent -> struct TArray<int> BoneArray;
	pname = 0xaf8, // Class FortniteGame.APlayerState -> struct FName PName;
	platform = 0x430,
};



class GetC {
public:

	class uObject {
	public:

	}; uObject UObject;

	class UActorComponent : uObject {
	public:
	};

	class uSceneComponent : UActorComponent {
	public:

		uintptr_t Getcomponentvelocity() {

			fvector Component = readd <fvector>(Globals.RootComp + offset::velocity);

			Globals.Velocity = Component;

			return 0;
		}

		fvector GetRelativeLocation() {

			return readd <fvector>(Globals.RootComp + Offsets::RootComponent);
		}

	}; uSceneComponent USceneComponent;

	class UPrimitiveComponent : uSceneComponent {
	public:
	};

	class UMeshComponent : UPrimitiveComponent {
	public:
	};

	class aActor : uObject {
	public:

		uintptr_t GetRootComponent() {

			return readd <uintptr_t>(Globals.AcknownledgedPawn + Offsets::RootComponent);
		}

	}; aActor AActor;

	class aInfo : aActor {
	public:



	};

	class AController : aActor {
	public:
	};

	class UDataAsset : uObject {
	public:
	};

	class UPrimaryDataAsset : UDataAsset {
	public:
	};

	class UMcpItemDefinitionBase : UPrimaryDataAsset {
	public:
	};

	class FText {
	public:

	};

	class UFortItemDefinition : UMcpItemDefinitionBase {
	public:

	};

	class UFortWorldItemDefinition : UFortItemDefinition {
	public:
	};

	class UFortWeaponItemDefinition : UFortWorldItemDefinition {
	public:
	};

	class aFortWeapon : aActor {
	public:
		std::string get_weapon_name(uintptr_t pawn_private)
		{
			std::string weapon_name;
			uint64_t current_weapon = readd<uint64_t>(pawn_private + 0xa80);
			uint64_t weapon_data = readd<uint64_t>(current_weapon + 0x6a0);
			uint64_t item_name = readd<uint64_t>(weapon_data + 0x40);
			if (!item_name) return "None";

			uint64_t FData = readd<uint64_t>(item_name + 0x18);
			int FLength = readd<int>(item_name + 0x20);

			if (FLength > 0 && FLength < 50)
			{
				wchar_t* WeaponBuffer = new wchar_t[FLength];
				Driver::read_physical(FData, (PVOID)WeaponBuffer, FLength * sizeof(wchar_t));
				std::wstring wstr_buf(WeaponBuffer);
				weapon_name.append(std::string(wstr_buf.begin(), wstr_buf.end()));

				delete[] WeaponBuffer;
			}

			return weapon_name;
		}
	}; aFortWeapon AFortWeapon;

	class APawn : aActor {
	public:

	};

	class aPlayerState : aInfo {
	public:
		std::string getPlayerPlatform(uintptr_t player_state) {
			std::string results;

			uintptr_t platformPtr = readd<uintptr_t>(player_state + offset::platform);
			wchar_t platformChar[64] = { 0 };
			Driver::read_physical(platformPtr, reinterpret_cast<uint8_t*>(platformChar), sizeof(platformChar));



			std::wstring platformWstr(platformChar);
			std::string platformStr(platformWstr.begin(), platformWstr.end());

			if (platformStr == "XBL")
			{
				results = "Xbox One";
			}
			else if (platformStr == "XSX")
			{
				results = "Xbox Series X";
			}
			else if (platformStr == "PSN")
			{
				results = "PlayStation 4";
			}
			else if (platformStr == "PS5")
			{
				results = "PlayStation 5";
			}
			else if (platformStr == "SWT")
			{
				results = "Nintendo Switch";
			}
			else if (platformStr == "WIN")
			{
				results = "PC";
			}
			else if (platformStr == "")
			{
				results = "No Platform";
			}

			return results;
		}

		std::string getPlayerName(uintptr_t player_state) {
			int pNameLength = 0;
			_WORD* pNameBufferPointer = nullptr;
			int i = 0;
			char v25 = 0;
			int v26 = 0;
			int v29 = 0;

			char16_t* pNameBuffer = nullptr;

			// Assuming driver and offset::pname are defined elsewhere
			try {
				uintptr_t pNameStructure = readd<uintptr_t>(player_state + offset::pname);
				pNameLength = readd<int>(pNameStructure + 0x10);
				if (pNameLength <= 0) {
					return "AI/BOT";
				}

				pNameBuffer = new (std::nothrow) char16_t[pNameLength + 1];
				if (pNameBuffer == nullptr) {
					return "Memory allocation failed";
				}

				uintptr_t pNameEncryptedBuffer = readd<uintptr_t>(pNameStructure + 0x8);

				Driver::read_physical(pNameEncryptedBuffer, pNameBuffer, pNameLength + sizeof(wchar_t));


				v25 = pNameLength - 1;
				v26 = 0;
				pNameBufferPointer = (_WORD*)pNameBuffer;

				for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7) {
					v29 = pNameLength - 1;
					if (!(_DWORD)pNameLength)
						v29 = 0;

					if (v26 >= v29)
						break;

					i += 3;
					++v26;
				}

				std::u16string temp_wstring(pNameBuffer);
				delete[] pNameBuffer;
				return std::string(temp_wstring.begin(), temp_wstring.end());
			}
			catch (const std::exception& e) {
				if (pNameBuffer != nullptr) {
					delete[] pNameBuffer;
				}
				return std::string("Exception occurred: ") + e.what();
			}
			catch (...) {
				if (pNameBuffer != nullptr) {
					delete[] pNameBuffer;
				}
				return "Unknown exception occurred";
			}
		}

	};  aPlayerState APlayerState;

	class AFGF_PlayerState : aPlayerState {
	public:

	};

	class AFortPlayerState : AFGF_PlayerState {
	public:
	};

	class AFortPlayerStateZone : AFortPlayerState {
	public:

	};

	class aFortPlayerStateAthena : AFortPlayerStateZone {
	public:

		char GetTeamIndex() {

			return readd <char>(Globals.PlayerState + Offsets::TeamIndex);
		}

	}; aFortPlayerStateAthena AFortPlayerStateAthena;

	class aCharacter : public APawn {
	public:

		uintptr_t GetSkeletalMesh() {

			return readd <uintptr_t>(Globals.AcknownledgedPawn + Offsets::Mesh);
		}

		uintptr_t GetPlayerState() {

			return readd <uintptr_t>(Globals.AcknownledgedPawn + Offsets::player_state);
		}

	}; aCharacter ACharacter;

	struct AFGF_Character : aCharacter {

	};

	struct aFortPawn : AFGF_Character {

	};

	struct AFortPlayerPawn : aFortPawn {

	};

	struct AFortPlayerPawnAthena : AFortPlayerPawn {


	};

	class aPlayerController : AController {
	public:

		uintptr_t GetAcknowledgedpawn() {

			return readd <uintptr_t>(Globals.PlayerController + Offsets::AcknowledgedPawn);
		}

	}; aPlayerController APlayerController;

	struct AFGF_PlayerController : aPlayerController {


	};

	struct AFortPlayerController : AFGF_PlayerController {

	};

	class uPlayer : uObject {
	public:

		uintptr_t GetPlayerController() {

			return readd <uintptr_t>(Globals.LocalPlayer + Offsets::PlayerController);
		}

	}; uPlayer UPlayer;

	struct ULocalPlayer : uPlayer {

	};

	struct UFortGenericRecord : uObject {

	};

	struct UFortClientSettingsRecord : UFortGenericRecord {

	};

	struct UFortLocalPlayer : ULocalPlayer {

	};

	class aGameStateBase : aInfo {
	public:

		uintptr_t GetPlayerArray() {

			return readd <uintptr_t>(Globals.GameState + Offsets::PlayerArray);
		}

		int GetPlayerArraySize() {

			return readd <int>(Globals.GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
		}

	}; aGameStateBase AGameStateBase;

	class uGameInstancee {
	public:

		uintptr_t GetLocalPlayer() {

			return readd <uintptr_t>(readd <uintptr_t>(Globals.GameInstance + Offsets::LocalPlayers));
		}

	}; uGameInstancee UGameInstance;


	class UWorldd : uObject {
	public:

		uintptr_t GetWorld() {

			return readd <uintptr_t>(baseaddy + Offsets::UWorld);
		}

		uintptr_t GetGameInstance() {

			return readd <uintptr_t>(Globals.Uworld + Offsets::OwningGameInstance);
		}

		uintptr_t GetGameState() {

			return readd <uintptr_t>(Globals.Uworld + Offsets::GameState);
		}

	}; UWorldd UWorld;

}; GetC GetClass;

struct FPlane : fvector
{
	double W;

	FPlane() : W(0) {}
	FPlane(double W) : W(W) {}
};

class FMatrix
{
public:
	double m[4][4];
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() {}
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
		: XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) {
	}
};

inline double RadiansToDegrees(double dRadians)
{
	return dRadians * (180.0 / M_PI);
}

template< typename t >
class TArray
{
public:
	TArray() : tData(), iCount(), iMaxCount() {}
	TArray(t* data, int count, int max_count) :
		tData(tData), iCount(iCount), iMaxCount(iMaxCount) {
	}

public:
	auto Get(int idx) -> t
	{
		return readd< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
	}

	auto Size() -> std::uint32_t
	{
		return this->iCount;
	}

	bool IsValid()
	{
		return this->iCount != 0;
	}

	t* tData;
	int iCount;
	int iMaxCount;
};

class camera {
public:

	camera_position_s camera_render()
	{
		camera_position_s camera;

		for (;; ) {

			if (!Globals.Uworld)
				continue;

			auto location_pointer = readd<uintptr_t>(Globals.Uworld + 0x128);
			auto rotation_pointer = readd<uintptr_t>(Globals.Uworld + 0x138);

			double pitch = readd<double>(rotation_pointer);
			double yaw = readd<double>(rotation_pointer + 0x20);
			double roll = readd<double>(rotation_pointer + 0x1d0);

			camera.rotation.x = asin(roll) * (180.0 / M_PI);
			camera.rotation.y = ((atan2(pitch * -1, yaw) * (180.0 / M_PI)) * -1) * -1;
			camera.location = readd<fvector>(location_pointer);
			camera.fov = readd<float>(Globals.PlayerController + 0x3ac) * 90.0f; // we can either call call Globals.Localplayer, or Global.PlayerController, just remember to change the offset we are parsing. 3AC	For Controller, 4AC for localplayer, and ofc for local player, get rid of the 90 degree rotation

			return camera;
		}
	}

}; camera camera_thread;


class uskinnedMeshComponent {
public:

	static auto GetBoneLocation(uintptr_t skeletal_mesh, int bone_index) -> fvector {
		uintptr_t bone_array = readd<uintptr_t>(skeletal_mesh + 0x5a8);
		if (bone_array == NULL) bone_array = readd<uintptr_t>(skeletal_mesh + 0x5a8 + 0x10);

		FTransform bone = readd<FTransform>(bone_array + (bone_index * 0x60));
		FTransform component_to_world = readd<FTransform>(skeletal_mesh + 0x1e0);
		D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());

		return fvector(matrix._41, matrix._42, matrix._43);
	} // there lol

	inline fvector2d World2Screen(fvector worldlocation) {

		camera_postion = camera_thread.camera_render();

		_MATRIX tempMatrix = Matrix(camera_postion.rotation);

		fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		fvector vDelta = worldlocation - camera_postion.location;
		fvector vTransformed = fvector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;
		return fvector2d((sw / 2.0f) + vTransformed.x * (((sw / 2.0f) / custom_tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (sh / 2.0f) - vTransformed.y * (((sw / 2.0f) / custom_tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);
	}

}; uskinnedMeshComponent USkinnedMeshComponent;

class uSkeletalMeshComponent : uskinnedMeshComponent {
public:

	bool WasRecentlyRendered(uintptr_t skeletal_mesh, float tolerance = 0.06f) {

		double Seconds = readd <double>(Globals.Uworld + 0x148);

		float LastRenderTimeOnScreen = readd <float>(skeletal_mesh + 0x30C);	// LastRenderTimeOnScreen

		return Seconds - LastRenderTimeOnScreen <= tolerance;
	}

}; uSkeletalMeshComponent USkeletalMeshComponent;