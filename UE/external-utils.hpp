#include "external-helper.hpp"
#include "../Draws/font_vars.hpp"

inline float text_font_size = 15.0f;

bool utilities::in_screen(fvector2d screen_location)
{
	if (screen_location.x > 0 && screen_location.x < sw && screen_location.y > 0 && screen_location.y < sh) return true;
	else return false;
}

void utilities::render_elements() {

	// For watermark
	if (g_watermark) {
		if (selected_font == 0) {
			ImGui::PushFont(moomoo);
		} else {
			ImGui::PushFont(poppins);
		}
		g_draw->draw_string(15, 7, 5, ImColor(255, 50, 50, 255), false, true, std::string("Memorium Private -> [ F8 OPEN/CLOSE ]").c_str());
		ImGui::PopFont();
	}

	// For framerate
	if (g_framerate) {
		char fppss[256];
		sprintf_s(fppss, std::string("[%.3f MS] [%.1f FPS]").c_str(), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		if (selected_font == 0) {
			ImGui::PushFont(moomoo);
		} else {
			ImGui::PushFont(poppins);
		}
		
		if (g_rendered_type == 1 && !g_rendered) {
			g_draw->draw_string(15, 7, 35, ImColor(240, 70, 19, 255), false, true, fppss);
		} else {
			g_draw->draw_string(15, 7, 35, ImColor(240, 70, 19, 255), false, true, fppss);
		}
		ImGui::PopFont();
	}

	if ( g_radar ) { render_radar_main( ); }

	if (g_render_crosshair) {

		// Crosshair size and thickness
		float crosshairSize = 10.0f; // Smaller size for a nice, compact crosshair
		float thickness = 3.0f; // Line thickness

		// Draw black outline (horizontal and vertical lines)
		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(sw / 2 - crosshairSize - 1, sh / 2),
			ImVec2(sw / 2 + crosshairSize + 1, sh / 2),
			ImColor(0, 0, 0), // Black color for outline
			thickness + 1 // Slightly thicker to create the outline effect
		);

		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(sw / 2, sh / 2 - crosshairSize - 1),
			ImVec2(sw / 2, sh / 2 + crosshairSize + 1),
			ImColor(0, 0, 0), // Black color for outline
			thickness + 1 // Slightly thicker to create the outline effect
		);

		// Draw white crosshair (horizontal and vertical lines)
		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(sw / 2 - crosshairSize, sh / 2),
			ImVec2(sw / 2 + crosshairSize, sh / 2),
			ImColor(255, 255, 255), // White color
			thickness // Normal thickness for the white lines
		);

		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(sw / 2, sh / 2 - crosshairSize),
			ImVec2(sw / 2, sh / 2 + crosshairSize),
			ImColor(255, 255, 255), // White color
			thickness // Normal thickness for the white lines
		);
	}


	if (g_render_fov) {

		// Add transparent black fill to the FOV
		ImGui::GetBackgroundDrawList()->AddCircleFilled(
			ImVec2(sw / 2, sh / 2),
			g_aimfov,
			ImColor(0, 0, 0, 60) // Transparent black (alpha 100 for some transparency)
		);

		// Add the FOV circle outline
		ImGui::GetBackgroundDrawList()->AddCircle(
			ImVec2(sw / 2, sh / 2),
			g_aimfov,
			ImColor(g_fov_color[0], g_fov_color[1], g_fov_color[2]),
			64, 1
		);
	}

}

auto utilities::CanHit(fvector lur, fvector wl) {

	if (lur.x >= wl.x - 20 && lur.x <= wl.x + 20 && lur.y >= wl.y - 20 && lur.y <= wl.y + 20 && lur.z >= wl.z - 30 && lur.z <= wl.z + 30)
		return true;
	else
		return false;	
}

auto utilities::calc_text(float Distance) {
	float BaseTextSize = text_font_size;
	float MaxDistance = 270.0f;
	float MinTextSize = text_font_size - 2.0f;
	float TextSize = BaseTextSize - ((Distance / MaxDistance) * (BaseTextSize - MinTextSize));
	TextSize = TextSize < MinTextSize ? MinTextSize : TextSize;

	return TextSize;
}

