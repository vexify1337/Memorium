#include "../../../Source/Cheat/Aimbot/ue5.hpp"

#define debug true

class loop {
public:

	static auto actors( ) -> void {

		for (;;) {

			temporary_entity_list.clear( );

			lock_guard<mutex> lock( fuck );

			Globals.Uworld = GetClass.UWorld.GetWorld( );

			Globals.GameInstance = GetClass.UWorld.GetGameInstance( );

			Globals.LocalPlayer = GetClass.UGameInstance .GetLocalPlayer( );

			Globals.PlayerController = GetClass.UPlayer.GetPlayerController( );

			Globals.AcknownledgedPawn = GetClass.APlayerController.GetAcknowledgedpawn( );

			Globals.Mesh = GetClass.ACharacter.GetSkeletalMesh( );

			Globals.PlayerState = GetClass.ACharacter.GetPlayerState( );

			Globals.RootComp = GetClass.AActor.GetRootComponent( );

			Globals.RelativeLocation = GetClass.USceneComponent.GetRelativeLocation( );

			Globals.TeamIndex = GetClass.AFortPlayerStateAthena.GetTeamIndex( );

			Globals.GameState = GetClass.UWorld.GetGameState( );

			Globals.PlayerArray = GetClass.AGameStateBase.GetPlayerArray( );

			Globals.PlayerArraySize = GetClass.AGameStateBase.GetPlayerArraySize( );

			for ( int i = 0; i < Globals.PlayerArraySize; ++i ) {

				auto player_state = readd <uintptr_t> ( Globals.PlayerArray + ( i * sizeof ( uintptr_t ) ) ); auto current_actor = readd <uintptr_t> ( player_state + Offsets::PawnPrivate);

				if ( current_actor == Globals.AcknownledgedPawn ) continue;

				auto skeletalmesh = readd <uintptr_t> ( current_actor + Offsets::Mesh );

				auto GetKills =  readd <int> ( player_state + Offsets::KillScore );

				if ( !skeletalmesh ) continue;

				auto base_bone = USkinnedMeshComponent.GetBoneLocation ( skeletalmesh, bone::Root );

				auto TeamIndex = readd <char >( player_state + Offsets::TeamIndex );
				if (base_bone.x == 0 && base_bone.y == 0 && base_bone.z == 0) continue;

				if ( g_performance && !game_utils.in_screen ( USkinnedMeshComponent.World2Screen ( USkinnedMeshComponent.GetBoneLocation ( skeletalmesh, bone::Pelvis ) ) ) ) continue;

				if ( Globals.AcknownledgedPawn && readd <int> ( player_state + Offsets::TeamIndex ) == Globals.TeamIndex ) {
					continue;
				}

				entity cached_actors{ };
				cached_actors.entity = current_actor;
				cached_actors.skeletal_mesh = skeletalmesh;
				cached_actors.player_state = player_state;
				cached_actors.kills = GetKills;
				cached_actors.team_index = TeamIndex;

				temporary_entity_list.push_back ( cached_actors );
			}

			
				entity_list.clear();
				entity_list = temporary_entity_list;
			
			sleep_for ( milliseconds ( 250 ) );
		}
	}	
}; loop threads;



