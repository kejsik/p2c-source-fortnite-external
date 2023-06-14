#include <Windows.h>
#include "../imgui/imgui.h"
#include <vector>
#include <d3d9.h>
#include "settings.hpp"
#include "../Gui/gui.h"
#include "../string.hpp"
#include "../Font/byte_array.h"
#include <dwmapi.h>
#include "sdk.hpp"
#include <string>
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <string>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

IDirect3D9Ex* p_object = NULL;
IDirect3DDevice9Ex* p_device = NULL;
D3DPRESENT_PARAMETERS p_params = { NULL };
HWND game_wnd = NULL;
RECT game_rect = { NULL };

MSG messager = { NULL };
HWND win = NULL;
HRESULT directx_init()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object)))
		exit(3);
	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = win;
	p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = settings::width;
	p_params.BackBufferHeight = settings::height;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, win, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		exit(4);
	}

	gui::setup(p_device, win);

	p_object->Release();
	return S_OK;
}

#define VK_W		0x57
#define VK_A		0x41
#define VK_S		0x53
#define VK_D		0x44
#define VK_SPACE    0x20
#define VK_R			0x52

int enemyID = 0;
int visennemy = 0;
int InFovEnemy = 0;

namespace Offsets
{
	uintptr_t PlayerState = 0x2A8; //APlayerState*
	uintptr_t PersistentLevel = 0x30; //ULevel*
}
float color_red = 1.;
float color_green = 0;
float color_blue = 0;
float color_random = 0.0;
float color_speed = -10.0;

ImFont* medium;

ImFont* bold;
ImFont* tab_title;
ImFont* combo_arrow;
ImFont* FN;
struct player
{

	uint64_t Actor;
	uint64_t Mesh;
	uint64_t RootComp;
	bool valid;
	bool IsOnScreen;
	bool enemy;
	bool isbot;
};

struct item
{

	uint64_t Actor;
	uint64_t Mesh;
	uint64_t RootComp;
	bool IsOnScreen;

};

std::vector<player> player_pawns;
std::vector<item> item_pawns;

bool vsyncenable = true;


void create_overlay()
{


	win = FindWindowA(_("CEF-OSC-WIDGET"), _("NVIDIA GeForce Overlay"));
	if (!win)
	{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		MessageBoxA(0, _("Failed To Find NVIDIA."), _("Overlay"), MB_OK);
		Sleep(5000);
		exit(1);
	}

	ShowWindow(win, SW_SHOW);

	SetWindowLongA(win, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	SetWindowLongA(
		win,
		-20,
		static_cast<LONG_PTR>(
			static_cast<int>(GetWindowLongA(win, -20)) | 0x20
			)
	);

	//transparency
	MARGINS margin = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(
		win,
		&margin
	);

	SetLayeredWindowAttributes(
		win,
		NULL,
		0xFF,
		0x02
	);

	// top most
	SetWindowPos(
		win,
		HWND_TOPMOST,
		0, 0, 0, 0,
		0x0002 | 0x0001
	);



	UpdateWindow(win);


}

void cleanup_d3d()
{
	if (p_device != NULL)
	{
		p_device->EndScene();
		p_device->Release();
	}
	if (p_object != NULL)
	{
		p_object->Release();
	}
}

void DrawFilledRect(int x, int y, int w, int h, ImU32 color)
{
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void DrawBox(float X, float Y, float W, float hight, float H, ImU32 Col)
{

	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), Col);
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), Col);
}

static auto DrawNormalBox(int x, int y, int w, int h, int borderPx, ImU32 color, float thickness) -> void
{
	DrawFilledRect(x + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x + w - w + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x, y, borderPx, h, color); //left 
	DrawFilledRect(x, y + h - h + borderPx * 2, borderPx, h, color); //left 
	DrawFilledRect(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w + borderPx, y, borderPx, h, color);//right 
	DrawFilledRect(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);//right 
}


void draw_cornered_box(int X, int Y, int W, int H, const ImU32& color, int thickness, int borderPx) {
	float lineW = (W / 3);
	float lineH = (H / 3);
	DrawFilledRect(X + borderPx, Y, W, borderPx, ImColor(10, 10, 10, 180));

	//black outlines
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

	//corners
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void draw_rect(int x, int y, int w, int h, ImColor color, int thickness) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 3.f, 15, thickness);
}


void box_esp(float OffsetW, float OffsetH, int X, int Y, int W, int H, ImU32 Color, int thickness, bool filled, bool outlined) {
	if (outlined) {
		draw_rect(X, Y, W, H, ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), thickness + 1);
	}

	draw_rect(X, Y, W, H, Color, thickness);
}

ImFont* infoF;

void DrawLine(int x1, int y1, int x2, int y2, ImU32 color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void draw_line(Vector3 target, ImColor color, float thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(settings::width / 2, settings::height), ImVec2(target.x, target.y), color, 0.1f);
}
void draw_text(Vector3 target, const char* text, ImColor color)
{
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(target.x, target.y), color, text);
}

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}


typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;



std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 2;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), color, text.c_str());
}

class Color
{
public:
	RGBA red = { 255,0,0,255 };
	RGBA redlol = { 255,0,0,200 };
	RGBA Magenta = { 255,0,255,255 };
	RGBA yellow = { 255,255,0,255 };
	RGBA grayblue = { 128,128,255,255 };
	RGBA green = { 128,224,0,255 };
	RGBA darkgreen = { 0,224,128,255 };
	RGBA brown = { 192,96,0,255 };
	RGBA pink = { 255,168,255,255 };
	RGBA DarkYellow = { 216,216,0,255 };
	RGBA SilverWhite = { 236,236,236,255 };
	RGBA purple = { 144,0,255,255 };
	RGBA Navy = { 88,48,224,255 };
	RGBA skyblue = { 0,136,255,255 };
	RGBA graygreen = { 128,160,128,255 };
	RGBA blue = { 0,96,192,255 };
	RGBA orange = { 255,128,0,255 };
	RGBA peachred = { 255,80,128,255 };
	RGBA reds = { 255,128,192,255 };
	RGBA darkgray = { 96,96,96,255 };
	RGBA Navys = { 0,0,128,255 };
	RGBA cyan = { 0,255,255,255 };
	RGBA darkgreens = { 0,128,0,255 };
	RGBA darkblue = { 0,128,128,255 };
	RGBA redbrown = { 128,0,0,255 };
	RGBA purplered = { 128,0,128,255 };
	RGBA greens = { 0,255,0,255 };
	RGBA envy = { 0,255,255,255 };
	RGBA black = { 100,0,0,0 };
	RGBA gray = { 128,128,128,255 };
	RGBA white = { 255,255,255,255 };
	RGBA blues = { 30,144,255,255 };
	RGBA lightblue = { 135,206,250,160 };
	RGBA Scarlet = { 220, 20, 60, 160 };
	RGBA white_ = { 255,255,255,200 };
	RGBA gray_ = { 128,128,128,200 };
	RGBA black_ = { 100,000, 000, 000 };
	RGBA red_ = { 255,0,0,200 };
	RGBA Magenta_ = { 255,0,255,200 };
	RGBA yellow_ = { 255,255,0,200 };
	RGBA grayblue_ = { 128,128,255,200 };
	RGBA green_ = { 128,224,0,200 };
	RGBA darkgreen_ = { 0,224,128,200 };
	RGBA brown_ = { 192,96,0,200 };
	RGBA pink_ = { 255,168,255,200 };
	RGBA darkyellow_ = { 216,216,0,200 };
	RGBA silverwhite_ = { 236,236,236,200 };
	RGBA purple_ = { 144,0,255,200 };
	RGBA Blue_ = { 88,48,224,200 };
	RGBA skyblue_ = { 0,136,255,200 };
	RGBA graygreen_ = { 128,160,128,200 };
	RGBA blue_ = { 0,96,192,200 };
	RGBA orange_ = { 255,128,0,200 };
	RGBA pinks_ = { 255,80,128,200 };
	RGBA Fuhong_ = { 255,128,192,200 };
	RGBA darkgray_ = { 96,96,96,200 };
	RGBA Navy_ = { 0,0,128,200 };
	RGBA darkgreens_ = { 0,128,0,200 };
	RGBA darkblue_ = { 0,128,128,200 };
	RGBA redbrown_ = { 128,0,0,200 };
	RGBA purplered_ = { 128,0,128,200 };
	RGBA greens_ = { 0,255,0,200 };
	RGBA envy_ = { 0,255,255,200 };

	RGBA glassblack = { 0, 0, 0, 160 };
	RGBA GlassBlue = { 65,105,225,80 };
	RGBA glassyellow = { 255,255,0,160 };
	RGBA glass = { 200,200,200,60 };

	RGBA filled = { 0, 0, 0, 150 };

	RGBA Plum = { 221,160,221,160 };

	RGBA VisibleColor = { 255 / 255, 0, 0 };
	RGBA InvisibleColor = { 0, 255 / 255, 0 };
	RGBA FilledFov = { 0,0,0,160 };

	RGBA rainbow() {

		static float x = 0, y = 0;
		static float r = 0, g = 0, b = 0;

		if (y >= 0.0f && y < 255.0f) {
			r = 255.0f;
			g = 0.0f;
			b = x;
		}
		else if (y >= 255.0f && y < 510.0f) {
			r = 255.0f - x;
			g = 0.0f;
			b = 255.0f;
		}
		else if (y >= 510.0f && y < 765.0f) {
			r = 0.0f;
			g = x;
			b = 255.0f;
		}
		else if (y >= 765.0f && y < 1020.0f) {
			r = 0.0f;
			g = 255.0f;
			b = 255.0f - x;
		}
		else if (y >= 1020.0f && y < 1275.0f) {
			r = x;
			g = 255.0f;
			b = 0.0f;
		}
		else if (y >= 1275.0f && y < 1530.0f) {
			r = 255.0f;
			g = 255.0f - x;
			b = 0.0f;
		}

		float rainbow_speed = 0.25f;

		x += rainbow_speed; //increase this value to switch colors faster
		if (x >= 255.0f)
			x = 0.0f;

		y += rainbow_speed; //increase this value to switch colors faster
		if (y > 1530.0f)
			y = 0.0f;


		return RGBA{ (DWORD)r, (DWORD)g, (DWORD)b, 255 };
	}


};
static Color Col;

void DrawNormalText(ImFont* Font, ImVec2 pos, ImU32 color, const char* text, bool outline = false);
void DrawNormalText(ImFont* Font, ImVec2 pos, ImU32 color, const char* text, bool outline)
{
	ImDrawList* Drawlist = ImGui::GetOverlayDrawList();
	if (outline) Drawlist->AddText(Font, 18.0f, pos, ImColor(0, 0, 0, 0), text);
	Drawlist->AddText(Font, 18.0f, pos, ImGui::GetColorU32(color), text);
}

void skeleton(uint64_t acotrmesh, bool isvisible) {

	Vector3 neck2 = GetBoneWithRotation(acotrmesh, 68);
	Vector3 vneck2 = ProjectWorldToScreen(neck2);


	Vector3 neck = GetBoneWithRotation(acotrmesh, 66);
	Vector3 vneck = ProjectWorldToScreen(neck);
	Vector3 rightChest = GetBoneWithRotation(acotrmesh, 8);
	Vector3 vrightChest = ProjectWorldToScreen(rightChest);
	Vector3 leftChest = GetBoneWithRotation(acotrmesh, 37);
	Vector3 vleftChest = ProjectWorldToScreen(leftChest);
	Vector3 leftShoulder = GetBoneWithRotation(acotrmesh, 38);
	Vector3 vleftShoulder = ProjectWorldToScreen(leftShoulder);
	Vector3 rightShoulder = GetBoneWithRotation(acotrmesh, 9);
	Vector3 vrightShoulder = ProjectWorldToScreen(rightShoulder);
	Vector3 leftElbow = GetBoneWithRotation(acotrmesh, 39);
	Vector3 vleftElbow = ProjectWorldToScreen(leftElbow);
	Vector3 rightElbow = GetBoneWithRotation(acotrmesh, 10);
	Vector3 vrightElbow = ProjectWorldToScreen(rightElbow);
	Vector3 leftWrist = GetBoneWithRotation(acotrmesh, 62);
	Vector3 vleftWrist = ProjectWorldToScreen(leftWrist);
	Vector3 rightWrist = GetBoneWithRotation(acotrmesh, 33);
	Vector3 vrightWrist = ProjectWorldToScreen(rightWrist);
	Vector3 pelvis = GetBoneWithRotation(acotrmesh, 2);
	Vector3 vpelvis = ProjectWorldToScreen(pelvis);
	Vector3 leftAss = GetBoneWithRotation(acotrmesh, 78);
	Vector3 vleftAss = ProjectWorldToScreen(leftAss);
	Vector3 rightAss = GetBoneWithRotation(acotrmesh, 71);
	Vector3 vrightAss = ProjectWorldToScreen(rightAss);
	Vector3 leftKnee = GetBoneWithRotation(acotrmesh, 79);
	Vector3 vleftKnee = ProjectWorldToScreen(leftKnee);
	Vector3 rightKnee = GetBoneWithRotation(acotrmesh, 72);
	Vector3 vrightKnee = ProjectWorldToScreen(rightKnee);
	Vector3 leftAnkle = GetBoneWithRotation(acotrmesh, 82);
	Vector3 vleftAnkle = ProjectWorldToScreen(leftAnkle);
	Vector3 rightAnkle = GetBoneWithRotation(acotrmesh, 86);
	Vector3 vrightAnkle = ProjectWorldToScreen(rightAnkle);

	DrawLine(vleftChest.x, vleftChest.y, vrightChest.x, vrightChest.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vleftChest.x, vleftChest.y, vleftShoulder.x, vleftShoulder.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vrightChest.x, vrightChest.y, vrightShoulder.x, vrightShoulder.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vleftShoulder.x, vleftShoulder.y, vleftElbow.x, vleftElbow.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vrightShoulder.x, vrightShoulder.y, vrightElbow.x, vrightElbow.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vleftElbow.x, vleftElbow.y, vleftWrist.x, vleftWrist.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vrightElbow.x, vrightElbow.y, vrightWrist.x, vrightWrist.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vneck2.x, vneck2.y, vpelvis.x, vpelvis.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vpelvis.x, vpelvis.y, vleftAss.x, vleftAss.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vpelvis.x, vpelvis.y, vrightAss.x, vrightAss.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vleftAss.x, vleftAss.y, vleftKnee.x, vleftKnee.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vrightAss.x, vrightAss.y, vrightKnee.x, vrightKnee.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vleftKnee.x, vleftKnee.y, vleftAnkle.x, vleftAnkle.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
	DrawLine(vrightKnee.x, vrightKnee.y, vrightAnkle.x, vrightAnkle.y, ImColor({ settings::visuals::skeletoncolor[0], settings::visuals::skeletoncolor[1], settings::visuals::skeletoncolor[2], settings::visuals::skeletoncolor[3] }), settings::visuals::boxwidth);
}

bool IsInScreen(Vector3 pos, int over = 30) {
	if (pos.x > -over && pos.x < settings::width + over && pos.y > -over && pos.y < settings::height + over) {
		return true;
	}
	else {
		return false;
	}
}



void gui::setup(LPDIRECT3DDEVICE9 device, HWND window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui 

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	font_config.GlyphRanges = ranges;
	ImFont* SmallIcon;
	ImFontConfig CustomFont;
	ImFontConfig ArialFont;
	ImFontConfig icons_config;
	ImFontConfig icons_config2;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 1;
	icons_config.OversampleV = 1;

	icons_config2.MergeMode = true;
	icons_config2.PixelSnapH = true;
	icons_config2.OversampleH = 1;
	icons_config2.OversampleV = 1;
	static const ImWchar icons_ranges[] = { 0xf000, 0xf5ff, 0 };
	static const ImWchar icons_ranges2[] = { 0xf000, 0xf5ff, 0 };

	FN = io.Fonts->AddFontFromFileTTF("C:\\JosefinSans-Bold.ttf", 15.0f, &font_config, ranges); 	// u gotta download this font and put it in ur c drive as im not downloadin it from bytes


	CustomFont.FontDataOwnedByAtlas = false;


	combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, sizeof(combo), 9.0f, &font_config, ranges);

	is_setup = true;
}


static auto DrawCircle(int x, int y, int radius, ImU32 color, int segments) -> void
{
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, segments, 2);
}

namespace fn {

	class fvector {
	public:
		fvector() : x(0.f), y(0.f), z(0.f)
		{

		}

		fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
		{

		}
		~fvector()
		{

		}

		double x;
		double y;
		double z;

		double Dot(const fvector& vOther) const {
			const fvector& a = *this;
			return (a.x * vOther.x + a.y * vOther.y + a.z * vOther.z);
		}

		double Distance(fvector v) {
			return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
		}

		fvector operator-(fvector V) {
			return fvector(x - V.x, y - V.y, z - V.z);
		}

		fvector operator+(fvector V) {
			return fvector(x + V.x, y + V.y, z + V.z);
		}

		fvector operator*(double Scale) {
			return fvector(x * Scale, y * Scale, z * Scale);
		}

		fvector operator/(double Scale) {
			double RScale = 1.f / Scale;
			return fvector(x * RScale, y * RScale, z * RScale);
		}

		fvector operator+(double A) {
			return fvector(x + A, y + A, z + A);
		}

		fvector operator-(double A) {
			return fvector(x - A, y - A, z - A);
		}

		fvector operator*(fvector V) {
			return fvector(x * V.x, y * V.y, z * V.z);
		}

		fvector operator/(fvector V) {
			return fvector(x / V.x, y / V.y, z / V.z);
		}

		double operator|(fvector V) {
			return x * V.x + y * V.y + z * V.z;
		}

		double operator^(fvector V) {
			return x * V.y - y * V.x - z * V.z;
		}

		fvector operator+=(fvector v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		fvector operator-=(fvector v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		fvector operator*=(fvector v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		fvector operator/=(fvector v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		bool operator==(fvector src) {
			return (src.x == x) && (src.y == y) && (src.z == z);
		}

		bool operator!=(fvector src) {
			return (src.x != x) || (src.y != y) || (src.z != z);
		}

		double Size() {
			return sqrt(x * x + y * y + z * z);
		}

		double Size2D() const {
			return sqrt(x * x + y * y);
		}

		double SizeSquared() {
			return x * x + y * y + z * z;
		}

		double SizeSquared2D() {
			return x * x + y * y;
		}
	};


}