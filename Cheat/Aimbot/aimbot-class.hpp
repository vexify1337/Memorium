#include <minwindef.h>



namespace smv {
	class aimbot {
	public:

		enum TYPES
		{
			move = 1,
			left_up = 4,
			left_down = 2,
			right_up = 8,
			right_down = 16
		};

		struct MOUSE_INFO
		{
			int move_direction_x;
			int move_direction_y;
			unsigned int mouse_data;
			TYPES mouse_options;
			unsigned int time_offset_in_miliseconds;
			void* extra_info;
		};

	
		enum INFO : int {
			FAIL = 0,
			SUCCESS = 1
		};

		[[nodiscard]] bool init();
		[[nodiscard]] bool Left(int x, int y);
		[[nodiscard]] bool Up(int x, int y);

	};

	class ejmbot {
	public:

		typedef int(*pDD_btn)(int btn);
		typedef int(*pDD_whl)(int whl);
		typedef int(*pDD_key)(int keycode, int flag);
		typedef int(*pDD_mov)(int x, int y);
		typedef int(*pDD_str)(char* str);
		typedef int(*pDD_todc)(int vk);
		typedef int(*pDD_movR)(int dx, int dy);

		pDD_btn   DD_btn;
		pDD_key   DD_key;
		pDD_mov   DD_mov;
		pDD_str   DD_str;
		pDD_todc  DD_todc;
		pDD_movR  DD_movR;
		HMODULE hDll;

		[[nodiscard]] void initialize();
	};  

}; static smv::aimbot* Mouse = new smv::aimbot();
	static smv::ejmbot* Dll = new smv::ejmbot();

