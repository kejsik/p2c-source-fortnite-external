#include <cstdint>
#include <WinUser.h>

int SetMouseAbsPosition(DWORD x, DWORD y)
{
	typedef UINT(WINAPI* LPSENDINPUT)(UINT cInputs, LPINPUT pInputs, int cbSize);
	LPSENDINPUT pSendInput = (LPSENDINPUT)GetProcAddress(GetModuleHandleA("user32.dll"), "SendInput");
	if (pSendInput == NULL) {
		return 1;
	}

	// Move the mouse
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	// Set the mouse position

	return pSendInput(1, &input, sizeof(INPUT));
}

void aimbot1(uintptr_t target_pawn, uintptr_t closest_distance)
{
	if (!target_pawn) return;
	uintptr_t mesh = drv->Read<uintptr_t>(target_pawn + 0x310);
	if (!mesh)
	{
		closest_distance = FLT_MAX;
		target_pawn = NULL;
	}

	int hitboxid = 106;
	switch (settings::aimbot::hitbox)
	{
	case 0:
		hitboxid = 106; // head
		break;
	case 1:
		hitboxid = 67; // neck
		break;
	case 2:
		hitboxid = 7; // chest
		break;
	}

	Vector3 head3d = GetBoneWithRotation(mesh, hitboxid);
	Vector3 head2d = ProjectWorldToScreen(head3d);
	double dx = head2d.x - (settings::width / 2);
	double dy = head2d.y - (settings::height / 2);
	float dist = sqrtf(dx * dx + dy * dy);
	if (dist > settings::aimbot::fov or head2d.x == 0 or head2d.y == 0)
	{
		closest_distance = FLT_MAX;
		target_pawn = NULL;
	}
	Vector2 screen_center = { (double)settings::width / 2, (double)settings::height / 2 };
	Vector2 target{};
	if (IsVisible(mesh))
	{
		if (head2d.x != 0)
		{
			if (head2d.x > screen_center.x)
			{
				target.x = -(screen_center.x - head2d.x);
				target.x /= settings::aimbot::smoothness;
				if (target.x + screen_center.x > screen_center.x * 2) target.x = 0;
			}
			if (head2d.x < screen_center.x)
			{
				target.x = head2d.x - screen_center.x;
				target.x /= settings::aimbot::smoothness;
				if (target.x + screen_center.x < 0) target.x = 0;
			}
		}
		if (head2d.y != 0)
		{
			if (head2d.y > screen_center.y)
			{
				target.y = -(screen_center.y - head2d.y);
				target.y /= settings::aimbot::smoothness;
				if (target.y + screen_center.y > screen_center.y * 2) target.y = 0;
			}
			if (head2d.y < screen_center.y)
			{
				target.y = head2d.y - screen_center.y;
				target.y /= settings::aimbot::smoothness;
				if (target.y + screen_center.y < 0) target.y = 0;
			}
		}
		Vector3 Angles;
		Angles = Vector3(-target.y / settings::aimbot::smoothness, target.x / settings::aimbot::smoothness, 0);

		SetMouseAbsPosition((int)target.x, (int)(target.y));

	}
}

void AimAt(DWORD_PTR mesh, uintptr_t actor)
{

	uint64_t currentactormesh = mesh;
	Vector3 localactorpos;
	auto rootHead = GetBoneWithRotation(mesh, 68);
	float distance1 = localactorpos.Distance(rootHead) / 250;

	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);
	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{
		aimbot1(rootHeadOut.x, rootHeadOut.y);
	}


};