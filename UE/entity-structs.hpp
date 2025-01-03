#include "Source/Draws/ImGui/imgui.h"
#include <d3d9.h>
#include "Source/Draws/ImGui/imgui_impl_win32.h"
#include "defintions.hpp"
#include "../../Source/Draws/globals.hpp"
#include "../../../Source/UE/entity-vectors.hpp"
ImFont* poppins;
ImFont* titlefont;
ImFont* Menu;
ImFont* moomoo;
ImFont* roboto;
struct camera_position_s {
	fvector location{};
	fvector rotation{};
	float fov{};
};
inline camera_position_s camera_postion{};


class entity {
public:
	uintptr_t
		entity,
		skeletal_mesh,
		root_component,
		player_state;
	
	char
		IgnoreDeads;
		
	int
		team_index,
		kills;
	char
		team_number;
	fvector
		relative_location;
	float
		lastrendertime;
	bool
		is_visible;
};
std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;

struct WeaponInformation
{
	int32_t ammo_count;
	int32_t max_ammo;

	BYTE tier;
	BYTE tier2;
	std::string weapon_name;
	std::string buildplan;

};
std::string LocalPlayerWeapon;

struct FRankedProgressReplicatedData
{
	BYTE RankType; // 0x00(0x10)
	int32_t Rank; // 0x10(0x04)
	char pad_14[0x4]; // 0x14(0x04)
};
FRankedProgressReplicatedData nyger;
WeaponInformation WeaponInfo;

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name,
		isVehicle,
		isChest,
		isLamma,
		isPickup,
		isAmmoBox;
	float
		distance;
};
std::vector<item> item_pawns;

typedef struct _LootEntity {
	std::string GNames;
	uintptr_t ACurrentItem;
}LootEntity;

static std::vector<LootEntity> LootentityList;


class structs
{
public:
		
	uintptr_t
			Uworld,
			GameInstance,
			GameState,
			LocalPlayer,
			PlayerController,
			AcknownledgedPawn,
			Mesh,
			PlayerState,
			RootComp,
			PlayerArray,
			LocalWeapon;

		int32_t
			ammo_count;

		uint64_t CurrentWeapon,
			WeaponData;

		int
			TeamIndex,
			PlayerArraySize;

		fvector
			RelativeLocation,
			Velocity;

}; structs Globals;

struct CameraRotation {
	double a; //0x0000
	char pad_0008[24]; //0x0008
	double b; //0x0020
	char pad_0028[424]; //0x0028
	double c; //0x01D0
} inline camera_rotation;




