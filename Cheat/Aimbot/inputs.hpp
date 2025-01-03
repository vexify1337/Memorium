#include "../../Cheat/A-Xor/a-xor.hpp"
#include <libloaderapi.h>
#include "aimbot-class.hpp"

bool(*MOVE)(smv::aimbot::MOUSE_INFO*, int) = nullptr;

bool smv :: aimbot :: init ( ) {

	auto win32u = IMPORT( LoadLibraryA ).safe_cached( ) ( "win32u.dll" );
	void* ADDRESS = ( void* )IMPORT ( GetProcAddress).safe_cached( ) ( win32u, ( "NtUserInjectMouseInput" ) );
	if ( !ADDRESS ) return INFO::FAIL;
	*(void**)&MOVE = ADDRESS;
	IMPORT( FreeLibrary ).get( )( win32u );
	return INFO::SUCCESS;
}

bool Move ( int X, int Y) {

	smv::aimbot::MOUSE_INFO info = {};
	info.mouse_data = 0;
	info.mouse_options = smv::aimbot::TYPES::move;
	info.move_direction_x = X;
	info.move_direction_y = Y;
	info.time_offset_in_miliseconds = 0;
	return MOVE ( &info, 1 ) ;
}

bool smv :: aimbot :: Left ( int x = 0, int y = 0 ) {

	MOUSE_INFO info = {};
	info.mouse_data = 0;
	info.mouse_options = TYPES::left_down;
	info.move_direction_x = x;
	info.move_direction_y = y;
	info.time_offset_in_miliseconds = 0;
	return MOVE( &info, 1 );

}

bool smv :: aimbot :: Up ( int x = 0, int y = 0 ) {

	MOUSE_INFO info = {};
	info.mouse_data = 0;
	info.mouse_options = TYPES::left_up;
	info.move_direction_x = x;
	info.move_direction_y = y;
	info.time_offset_in_miliseconds = 0;
	return MOVE( &info, 1 );
}

void smv :: ejmbot :: initialize ( ) {

	HMODULE hDll = LoadLibraryW( L"C:\\dd32695.x64.dll" );

	if ( hDll == nullptr )
	{
	}

	DD_btn = ( pDD_btn ) GetProcAddress ( hDll, "DD_btn" );
	DD_key = ( pDD_key ) GetProcAddress ( hDll, "DD_key" );
	DD_mov = ( pDD_mov ) GetProcAddress ( hDll, "DD_mov" );
	DD_str = ( pDD_str ) GetProcAddress ( hDll, "DD_str" );
	DD_todc = ( pDD_todc ) GetProcAddress ( hDll, "DD_todc" );
	DD_movR = ( pDD_movR ) GetProcAddress ( hDll, "DD_movR" );

	if ( ! ( DD_btn && DD_key && DD_mov && DD_str && DD_todc && DD_movR ) )
	{
	}

	int st = DD_btn( 0 );
	if ( st != 1 )
	{
	};

}