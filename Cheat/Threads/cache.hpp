#include "../threads/actors.hpp"
#include <D3DX11tex.h>
#include "../../Source/Draws/font_vars.hpp"
#include "controller.hpp"
HWND hwnd1 = 0;

//WeaponInformation getWeaponInfo(uint64_t player_address) {
//	uint64_t player_weapon = readd<uint64_t>(player_address + Offsets::CurrentWeapon);
//	WeaponInformation held_weapon;
//
//	uint64_t weapon_data = readd<uint64_t>(player_weapon + Offsets::WeaponData);
//	held_weapon.ammo_count = readd<int32_t>(player_weapon + 0xEDC);
//
//	BYTE tier = readd<BYTE>(weapon_data + offset::tier);
//
//	if (utils->is_valid(weapon_data)) {
//		held_weapon.tier = tier;
//
//		uint64_t display_name_ptr = readd<uint64_t>(weapon_data + offset::displayname);
//		held_weapon.tier2 = readd<uint8>(weapon_data + offset::tier);
//
//		if (utils->is_valid(display_name_ptr)) {
//			uint64_t display_name_data = readd<uint64_t>(display_name_ptr + 0x28);
//			int display_name_length = readd<int>(display_name_ptr + 0x30);
//
//			if (display_name_length > 0 && display_name_length < 50) {
//				wchar_t* display_name_buf = new wchar_t[display_name_length + 1];
//
//				readArray(display_name_data, display_name_buf, display_name_length * sizeof(wchar_t));
//
//				if (display_name_buf[0] != L'\0') {
//					std::wstring WeaponNameWStr(display_name_buf);
//					std::string WeaponName(WeaponNameWStr.begin(), WeaponNameWStr.end());
//
//					// Check if WeaponName includes "Reaper Sniper Ri"
//					std::size_t found = WeaponName.find("Reaper Sniper Ri");
//					if (found != std::string::npos) {
//						// Replace "Reaper Sniper Ri" with "Reaper Sniper Rifle"
//						WeaponName = "Reaper Sniper Rifle";
//					}
//
//					held_weapon.weapon_name = WeaponName;
//				}
//				else {
//					held_weapon.weapon_name = "Unknown";
//				}
//
//				delete[] display_name_buf;
//			}
//		}
//	}
//
//	return held_weapon;
//}

struct WeaponStats {
	float speed;
	float gravityScale;
};

static std::map<std::string, WeaponStats> weapon_projectile_map = {
	{"Default", {0.0f, 0.0f}},
	{"Rifle", {0.0f, 0.0f}},
	{"Tactical Assault Rifle", {80000.0f, 3.5f}},
	{"Striker AR", {80000.0f, 3.5f}},
	{"Nemesis AR", {80000.0f, 3.5f}},
	{"Huntress DMR", {96000.0f, 2.5f}},
	{"Harbinger SMG", {70000.0f, 3.0f}},
	{"Hades' Harbinger SMG", {70000.0f, 3.0f}},
	{"Thunder Burst SMG", {70000.0f, 3.0f}},
	{"Warforged Assault Rifle", {80000.0f, 3.5f}},
	{"Ares' Warforged Assault Rifle", {80000.0f, 3.5f}},
	{"Reaper Sniper Rifle", {50000.0f, 3.5f}},
	{"Drum Gun", {75000.0f, 3.0f}},
	{"Midas Drum Gun", {75000.0f, 3.0f}},
    {"Pistol", {67000.f, 2.5f}},
	{"Cobra DMR", {55000.f, 0.16f}},
	{"Heavy Sniper Rifle", {45000.f, 0.12f}},
	{"Dragon's Breath Sniper", {30000.f, 0.12f}},
	{"Storm Scout", {30000.f, 0.12f}},
	{"Storm Scout Sniper Rifle", {30000.f, 0.12f}},
	{"Hunting Rifle", {30000.f, 0.12f}},
	{"Explosive Repeater Rifle", {30000.f, 0.12f}},
	{"Bolt-Action Sniper Rifle", {30000.f, 0.12f}}

};

WeaponStats getWeaponStats(const std::string& weaponName) {
	auto it = weapon_projectile_map.find(weaponName);
	if (it != weapon_projectile_map.end()) {
		return it->second;
	}
	return { 0.f, 0.f }; // Default values if weapon not found
}


class g_fn {
public:
	
	auto actor_loop() -> void {

		ImGui::PushFont(moomoo);
		if (entity_list.empty()) {
			ImGui::PopFont();  // Ensure stack is balanced
			return;
		}
		float target_dist = FLT_MAX;
		uintptr_t target_entity = 0;
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		game_utils.render_elements();

		for (auto& cached : entity_list) {
			if (!cached.skeletal_mesh) continue;

			auto root_bone = USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, bone::Root);
			if (!root_bone.is_valid()) continue;

			root = USkinnedMeshComponent.World2Screen(root_bone);
			if (!root.is_valid()) continue;
			r = USkinnedMeshComponent.World2Screen(fvector(root_bone.x, root_bone.y, root_bone.z + 30));

			r1 = USkinnedMeshComponent.World2Screen(fvector(root_bone.x, root_bone.y, root_bone.z - 15));

			auto head_bone = USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, bone::Head);

			head = USkinnedMeshComponent.World2Screen(head_bone);

			h = USkinnedMeshComponent.World2Screen(fvector(head_bone.x, head_bone.y, head_bone.z + 15));

			float bh = abs(head.y - r1.y);
			float w = bh * g_width;
			float distance = camera_postion.location.distance(root_bone) * 0.01;

			float TextSize = game_utils.calc_text(distance);

			if (distance > g_render_distance && Globals.AcknownledgedPawn) continue;

			if (g_aimbot || g_memory) {

				auto dx = head.x - (sw / 2);
				auto dy = head.y - (sh / 2);
				auto dist = sqrtf(dx * dx + dy * dy);

				if (g_visible_check) {
					if (USkeletalMeshComponent.WasRecentlyRendered(cached.skeletal_mesh)) {
						if (dist < g_aimfov && dist < target_dist) {
							target_dist = dist;
							target_entity = cached.entity;
						}
					}
				}
				else {
					if (dist < g_aimfov && dist < target_dist) {
						target_dist = dist;
						target_entity = cached.entity;
					}
				}
			}

			ImColor vis;
			ImColor invis;
			ImColor normal;

			vis = ImVec4(g_color_Visible[0], g_color_Visible[1], g_color_Visible[2], 1.0f);
			invis = ImVec4(g_color_invisible[0], g_color_invisible[1], g_color_invisible[2], 1.0f);
			normal = ImVec4(g_color[0], g_color[1], g_color[2], 1.0f);

			ImColor vc;
		
			if (g_visible_check && g_enable_esp) { vc = USkeletalMeshComponent.WasRecentlyRendered(cached.skeletal_mesh) ? ImColor(vis) : ImColor(invis); }
			else { vc = ImColor(normal); }

			//if (g_line && g_enable_esp)
			//{
			//	switch (g_line_type)
			//	{
			//	case 0: draw_list->AddLine(ImVec2(sw / 2, 0), ImVec2(h.x, h.y), vc, glt);	break;
			//	case 1:	draw_list->AddLine(ImVec2(sw / 2, sh / 2), ImVec2(h.x, h.y), vc, glt);	break;
			//	case 2: draw_list->AddLine(ImVec2(sw / 2, sh), ImVec2(r.x, r.y), vc, glt); break;
			//	}
			//}
	
		
			 
			if (g_box_esp && g_enable_esp) {
				ImU32 topColor = ImColor(43, 40, 46, 128);
				ImU32 bottomColor = ImColor(184, 106, 236, 128);
				
				if ((g_boxfilled && g_box_type != 3) || (g_cornerboxfilled && g_box_type == 3)) {
					ImVec2 topLeft;
					ImVec2 bottomRight;
					
					switch (g_box_type) {
						case 0: // 2D box
						case 2: // Rounded box
						case 4: // Bounding box
							topLeft = ImVec2(h.x - (w / 2), h.y);
							bottomRight = ImVec2((h.x - (w / 2)) + w, h.y + bh);
							draw_list->AddRectFilledMultiColor(topLeft, bottomRight, topColor, topColor, bottomColor, bottomColor);
							break;
							
						case 3: // Cornered box
							topLeft = ImVec2(root.x - (w / 2), h.y);
							bottomRight = ImVec2(root.x + (w / 2), root.y);
							draw_list->AddRectFilledMultiColor(topLeft, bottomRight, topColor, topColor, bottomColor, bottomColor);
							break;
							
						case 1: // 3D box
							// Skip filled box for 3D type
							break;
					}
				}

				switch (g_box_type)
				{
				case 0:  //2d
					if (g_outline)
					{
						draw_list->AddLine(ImVec2(h.x - (w / 2), h.y), ImVec2(root.x + (w / 2), h.y), ImColor(0, 0, 0), gbt + 2);
						draw_list->AddLine(ImVec2(h.x - (w / 2), h.y), ImVec2(h.x - (w / 2), root.y), ImColor(0, 0, 0), gbt + 2);
						draw_list->AddLine(ImVec2(h.x - (w / 2), root.y), ImVec2(root.x + (w / 2), root.y), ImColor(0, 0, 0), gbt + 2);
						draw_list->AddLine(ImVec2(root.x + (w / 2), h.y), ImVec2(root.x + (w / 2), root.y), ImColor(0, 0, 0), gbt + 2);
					}

					draw_list->AddLine(ImVec2(h.x - (w / 2), h.y), ImVec2(root.x + (w / 2), h.y), vc, gbt);
					draw_list->AddLine(ImVec2(h.x - (w / 2), h.y), ImVec2(h.x - (w / 2), root.y), vc, gbt);
					draw_list->AddLine(ImVec2(h.x - (w / 2), root.y), ImVec2(root.x + (w / 2), root.y), vc, gbt);
					draw_list->AddLine(ImVec2(root.x + (w / 2), h.y), ImVec2(root.x + (w / 2), root.y), vc, gbt);
					break;
				case 1: if (g_outline) { g_draw->DrawThreeDBox(root_bone, head_bone, 218.4375 * g_width, 100, gbt + 2.0, ImColor(0, 0, 0)); } g_draw->DrawThreeDBox(root_bone, head_bone, 218.4375 * g_width, 100, gbt, vc); break;
				case 2:	if (g_outline) { draw_list->AddRect(ImVec2(h.x - w / 2, h.y), ImVec2((h.x - w / 2) + w, h.y + bh), ImColor(0, 0, 0), 10, 0, gbt + 2.0); } draw_list->AddRect(ImVec2(h.x - w / 2, h.y), ImVec2((h.x - w / 2) + w, h.y + bh), vc, 10, 0, gbt); break;
				case 3: if (g_outline) { g_draw->draw_cornered_box(root.x - (w / 2), h.y, w, bh, ImColor(0, 0, 0), gbt + 2.0, 0, 0); } g_draw->draw_cornered_box(root.x - (w / 2), h.y, w, bh, vc, gbt, 0, 0); break;
				case 4: if (g_outline) { g_draw->bounding(h.x - (w / 2), h.y, w, bh, ImColor(0, 0, 0), gbt + 2.5); } g_draw->bounding(h.x - (w / 2), h.y, w, bh, vc, gbt); break;
				}
			}

			if (g_username && g_enable_esp) {
				std::string kills_str = (cached.kills >= 0) ? std::to_string(cached.kills) : "0 Kills";
				string username_str = GetClass.APlayerState.getPlayerName(cached.player_state) + " | " + kills_str;

				if (username_str.size() > 1) {
					if (selected_font == 0) {
						ImGui::PushFont(moomoo);
					}
					else {
						ImGui::PushFont(poppins); 
					}

					g_draw->draw_string(
						g_font_size,
						h.x - TextSize - 17,
						h.y - 20,
						ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]),
						false,
						true,
						username_str.c_str()
					);

					ImGui::PopFont();
				}
			}

			if (g_username && g_enable_esp) {
				std::string kills_str = (cached.kills >= 0) ? std::to_string(cached.kills) : "0 Kills";
				std::string username_str = GetClass.APlayerState.getPlayerName(cached.player_state) + " | " + kills_str;

				if (username_str.size() > 1) {
					if (selected_font == 0) {
						ImGui::PushFont(moomoo);
					}
					else {
						ImGui::PushFont(poppins);
					}

					g_draw->draw_string(
						g_font_size,
						h.x - TextSize - 17,
						h.y - 20,
						ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]),
						false,
						true,
						username_str.c_str()
					);

					ImGui::PopFont();
				}
			}

			if (g_platform && g_enable_esp)
			{
				std::string platform_string = GetClass.APlayerState.getPlayerPlatform(cached.player_state);
				ImVec2 text_size = ImGui::CalcTextSize(platform_string.c_str());

				if (selected_font == 0) {
					ImGui::PushFont(moomoo);
				} else {
					ImGui::PushFont(poppins);
				}

				g_draw->draw_string(
					g_font_size, 
					root.x - (TextSize), 
					root.y + 20, 
					ImColor(255, 255, 255),
					false, 
					true, 
					platform_string.c_str()
				);

				ImGui::PopFont();
			}

			if (g_weapon_esp && g_enable_esp)
			{
				std::string weapon_string = GetClass.AFortWeapon.get_weapon_name(cached.entity);
				ImVec2 text_size = ImGui::CalcTextSize(weapon_string.c_str());

				if (selected_font == 0) {
					ImGui::PushFont(moomoo);
				} else {
					ImGui::PushFont(poppins);
				}

				float y_offset = 34.0f;
				g_draw->draw_string(
					g_font_size, 
					root.x - (TextSize), 
					root.y + y_offset, 
					ImColor(255, 255, 255),
					false, 
					true, 
					weapon_string.c_str()
				);

				ImGui::PopFont();
			}

			if (g_distance && g_enable_esp)
			{
				string Distance = "" + customstring((float)distance) + "M";
				ImVec2 text_size = ImGui::CalcTextSize(Distance.c_str());
				
				if (selected_font == 0) {
					ImGui::PushFont(moomoo); 
				} else {
					ImGui::PushFont(poppins);
				}
				
				g_draw->draw_string(
					g_font_size, 
					root.x - (TextSize), 
					root.y + 5, 
					ImColor(g_distance_color[0], g_distance_color[1], g_distance_color[2]), 
					false, 
					true, 
					Distance.c_str()
				);
				
				ImGui::PopFont();
			}

			if ( g_radar )
			{
				add_to_radar(root_bone, 187, USkeletalMeshComponent.WasRecentlyRendered(cached.skeletal_mesh));
			}		

			if (g_rendered && g_enable_esp)
			{
				switch (g_rendered_type) {
				case 0:
					char players_found[256];
					sprintf_s(players_found, "Non Visible: %d", entity_list.size());
					
					if (selected_font == 0) {
						ImGui::PushFont(moomoo);
					} else {
						ImGui::PushFont(poppins); 
					}
					
					g_draw->draw_string(15, 7, 19, ImColor(19, 255, 255), false, true, players_found);
					
					ImGui::PopFont(); 
					break;
				case 1:
					char enemies_found[256];
					char visible_found[256];
					char player_array = Globals.PlayerArraySize;
					int visible = USkeletalMeshComponent.WasRecentlyRendered(cached.entity);

					std::string player_num = std::to_string(player_array);
					std::string visible_num = std::to_string(visible);

					sprintf_s(enemies_found, "Enemies Around: %d", entity_list.size());
					sprintf_s(visible_found, "Visible: %d", entity_list.size() && USkeletalMeshComponent.WasRecentlyRendered(cached.skeletal_mesh));

					// Render the strings with the same size
					g_draw->draw_string(15, 800, 14, ImColor(255, 140, 0), false, true, enemies_found);  
					g_draw->draw_string(15, 927, 14, ImColor(255, 255, 255), false, true, "|");       
					g_draw->draw_string(15, 935, 14, ImColor(195, 62, 232), false, true, visible_found);
					break;
				}
			}

			if (g_skeleton && g_enable_esp) {
				fvector2d Neck = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 67)); // Neck
				fvector2d RightShoulder = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 9)); // Right Shoulder
				fvector2d LeftShoulder = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 38)); // Left Shoulder
				fvector2d RightElbow = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 10)); // Right Elbow
				fvector2d LeftElbow = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 39)); // Left Elbow
				fvector2d RightHand = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 11)); // Right Hand
				fvector2d LeftHand = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 40)); // Left Hand
				fvector2d RightKnee = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 72)); // Right Knee
				fvector2d LeftKnee = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 79)); // Left Knee
				fvector2d RightHip = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 71)); // Right Hip
				fvector2d LeftHip = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 78)); // Left Hip
				fvector2d Pelvis = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 2)); // Pelvis
				fvector2d RightFoot = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 75)); // Right Foot
				fvector2d LeftFoot = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(cached.skeletal_mesh, 82)); // Left Foot

				g_draw->line(Neck, RightShoulder, gsk, vc, gso);
				g_draw->line(Neck, LeftShoulder, gsk, vc, gso);
				g_draw->line(Neck, Pelvis, gsk, vc, gso);

				g_draw->line(RightShoulder, RightElbow, gsk, vc, gso);
				g_draw->line(LeftShoulder, LeftElbow, gsk, vc, gso);

				g_draw->line(RightElbow, RightHand, gsk, vc, gso);
				g_draw->line(LeftElbow, LeftHand, gsk, vc, gso);

				g_draw->line(Pelvis, RightHip, gsk, vc, gso);
				g_draw->line(Pelvis, LeftHip, gsk, vc, gso);

				g_draw->line(RightHip, RightKnee, gsk, vc, gso);
				g_draw->line(LeftHip, LeftKnee, gsk, vc, gso);

				g_draw->line(RightKnee, RightFoot, gsk, vc, gso);
				g_draw->line(LeftKnee, LeftFoot, gsk, vc, gso);
			}
		}




		if (target_entity && g_aimbot || g_memory) {

			auto closest_mesh = readd <uint64_t>(target_entity + Offsets::Mesh);

			fvector hitbox;
			fvector2d hitbox_screen;

			switch (g_hitbox) {
			case 0:	hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Head));	hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Head); break;
			case 1: hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Neck));	hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Neck); break;
			case 2:	hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Chest));	hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Chest); break;
			case 3:

				mt19937 rng(static_cast <unsigned int> (time(nullptr))); uniform_int_distribution <int> distribution(0, 3); int randomHitbox = distribution(rng);
				switch (randomHitbox) {
				case 0:	hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Neck)); hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Neck); break;
				case 1:	hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Chest)); hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::Chest); break;
				case 2:	hitbox_screen = USkinnedMeshComponent.World2Screen(USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::RShoulder)); hitbox = USkinnedMeshComponent.GetBoneLocation(closest_mesh, bone::RShoulder);	break;
				}
			}

			float projectileSpeed = 0;
			float projectileGravityScale = 0;

			if (g_prediction) {

				WeaponStats stats = getWeaponStats(LocalPlayerWeapon);
				projectileSpeed = stats.speed;
				projectileGravityScale = stats.gravityScale;
			}

			int bone;
			/*std::cout << "weapon_name:\t" << LocalPlayerWeapon.c_str() << std::endl;
			std::cout << "projectileSpeed:\t" << projectileSpeed << std::endl;
			std::cout << "projectileGravityScale:\t" << projectileGravityScale << std::endl;*/
			if (projectileSpeed) {
				bone = bone::Head;
			}

			auto root = readd <uintptr_t>(target_entity + Offsets::RootComponent);
			auto Distance = camera_postion.location.distance(hitbox);
			fvector Velocity = readd <fvector>(root + offset::velocity);

			if (projectileSpeed) {
				hitbox = PredictLocation(hitbox, Velocity, projectileSpeed, projectileGravityScale, Distance);
			}

			hitbox_screen = USkinnedMeshComponent.World2Screen(hitbox);

			if (hitbox.x != 0 || hitbox.y != 0 && (get_cross_distance(hitbox.x, hitbox.y, sw / 2, sh / 2) <= g_aimfov))
			{
				if (GetAsyncKeyState(aimkey) || Controller::IsPressingLeftTrigger())
					input->move(hitbox_screen);
			}
		}
		else {
			target_dist = FLT_MAX;
			target_entity = NULL;
		}

		ImGui::PopFont();
	}
};
static g_fn* m_main = new g_fn();