#pragma once
#include <d3d9.h>




//invis



namespace gui
{
	inline bool is_open = false;
	inline bool is_setup = false;

	void setup(LPDIRECT3DDEVICE9 device, HWND window);
	void render();

	enum heads {
		tab1, tab2, tab3
	};

	enum sub_heads {
		subtab1, subtab2
	};
}