#include "draw.hpp"
#include "ida.hpp"

class utilities {
public:

	[[nodiscard]] void render_elements ( );
	[[nodiscard]] bool in_screen ( fvector2d );
	[[nodiscard]] auto CanHit ( fvector, fvector );
	[[nodiscard]] auto calc_text( float );

}; utilities game_utils;
