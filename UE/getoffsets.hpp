
#include <iostream>
#include <fstream> 

class Offsets
{
public:
#define STATIC
#ifdef STATIC
    inline static uintptr_t UWorld = 0x169D6108;
    inline static uintptr_t AcknowledgedPawn = 0x350;
    inline static uintptr_t Mesh = 0x328;
    inline static uintptr_t RootComponent = 0x1B0;
    inline static uintptr_t LocalPlayers = 0x38;
    inline static uintptr_t PlayerController = 0x30;
    inline static uintptr_t OwningGameInstance = 0x1f0;
    inline static uintptr_t PawnPrivate = 0x320;
    inline static uintptr_t GameState = 0x178;
    inline static uintptr_t PlayerArray = 0x2c0;
    inline static uintptr_t bCanBeMarkedAsTeammate = 0x5b38;
    inline static uintptr_t CurrentWeapon = 0xA80;
    inline static uintptr_t WeaponData = 0x548;
    inline static char TeamIndex = 0x1261;
    inline static int KillScore = 0x1274;
    inline static uintptr_t btargetedbuild;
    inline static uintptr_t player_state = 0x2b0;
    inline static uintptr_t pawn_private = 0x320;


#else
    inline static uintptr_t UWorld;
    inline static uintptr_t AcknowledgedPawn;
    inline static uintptr_t Mesh;
    inline static uintptr_t RootComponent;
    inline static uintptr_t LocalPlayers;
    inline static uintptr_t PlayerController;
    inline static uintptr_t OwningGameInstance;
    inline static uintptr_t PawnPrivate;
    inline static uintptr_t GameState;
    inline static uintptr_t PlayerArray;
    inline static uintptr_t bCanBeMarkedAsTeammate;
    inline static uintptr_t CurrentWeapon;
    inline static uintptr_t WeaponData;
    inline static BYTE Rarity;
    inline static char TeamIndex;
    inline static int KillScore;
    inline static uintptr_t btargetedbuild;
    inline static uintptr_t player_state;
    inline static uintptr_t pawn_private;
#endif

};