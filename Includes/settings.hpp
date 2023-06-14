#pragma once

namespace settings
{
	inline int width = GetSystemMetrics(SM_CXSCREEN);
	inline float size = 18.0f;
	inline int height = GetSystemMetrics(SM_CYSCREEN);
	namespace aimbot
	{
		inline bool enable = false;
		inline bool Aimline = false;
		inline bool bIgnoreTeam = false;
		inline bool bIgnoreDead = false;
		inline bool renderfps = true;
		inline bool bIgnoreHidden = false;
		inline bool panicquit = false;
		inline bool bigplayer = false;
		inline bool smallplayer = false;
		inline bool aimbot12 = true;
		inline bool MEMaim = true;
		inline bool show_fov = false;
		inline bool filled_fov = false;
		inline int fov = 160.0f;
		inline int smoothness = 8.0f;
		inline float smoothness2 = 50.0f;
		inline int hitbox = 1;
		inline int FOVS = 0;
		inline bool bTargetedBuild = 0;
	}
	namespace exploits
	{
		inline bool playertp = false;
		inline bool Bigplayer = false;
	

	}
	namespace visuals
	{
		inline float maxdistance = 50.0f;
		inline bool enable = true;
		inline bool bweapon = false;
		inline bool roundedboxes = true;
		inline bool fingy = false;
		inline bool skeleton = false;
		inline bool draw_held_weapon = false;
		inline bool gayskeleton = true;
		inline bool fill = false;
		inline bool nameesp = false;
		inline bool playercounts = false;
		inline bool niggaboxfilled = false;
		inline bool box = false;
		inline bool cornered_box = false;
		inline bool line = false;
		inline bool distance = false;
		inline bool distancetop = false;
		inline bool distancebottom = false;
		inline bool Outline = true;
		inline bool vischeck = true;
		inline bool enemiesrendered = false;
		inline bool settingsStreamproof = false;
		inline bool threed = false;

		inline float box_color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		inline float VisCOlor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		inline float NOtVIs[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		inline int boxwidth = 1;
		inline int thickness = 1;
		inline float FovColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		inline float skeletoncolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	

		inline float inv_box_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		inline float inv_filled_box_color[4] = { 0.0f, 0.0f, 0.0f, 0.2f };
		inline float inv_line_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	
		inline int thickness2 = 1;






	}
}