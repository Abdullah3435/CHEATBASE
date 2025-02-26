#include "Aimbot.h"

float Aimbot::dist(vec3 p1, vec3 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2)); // Norm / distance form 
}

void Aimbot::frame()
{

	uintptr_t entities = memory::Read<uintptr_t>(base + cs2_dumper::offsets::client_dll::dwEntityList);
	if (!entities) { return; }


	uintptr_t player_transform = memory::Read<uintptr_t>(base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	if (!player_transform) { return; }
	OutputDebugString(L"we here 2?");

	BYTE team = memory::Read<BYTE>(player_transform + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

	vec3 selfEye = memory::Read<vec3>(player_transform + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) + memory::Read<vec3>(player_transform + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

	float closest_dist = 99999999999;
	vec3 enemytransform;

	for (int i = 0; i < 32; i++)
	{
		uintptr_t item = memory::Read<uintptr_t>(entities + ((8 * (i & 0x7ff) >> 9) + 16));
		if (!item) continue;
		OutputDebugString((L"bot position is at this address: " + std::to_wstring(reinterpret_cast<intptr_t>(&item)) + L"iteration number is" + std::to_wstring(i) + L"\n" ).c_str());
		uintptr_t entityController = memory::Read<uintptr_t>(item + 120 * (i & 0x1ff));
		if (!entityController) continue;
		OutputDebugString(L"we here 0?");
		uintptr_t entityControllerPawn = memory::Read<uintptr_t>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!entityControllerPawn) continue;
		OutputDebugString(L"we here 1?");
		uintptr_t entityPawn = memory::Read<uintptr_t>(item + 120 * (entityControllerPawn & 0x1ff));
		if (!entityPawn) continue;
		OutputDebugString(L"we here 2?");
		if (team == memory::Read<BYTE>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum))
			continue;
		OutputDebugString(L"we here 3?\n");
		vec3 item_eye = memory::Read<vec3>(entityPawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin) + memory::Read<vec3>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);

		OutputDebugString((L"bot Head is at : X" + std::to_wstring(item_eye.x) + L"Y" + std::to_wstring(item_eye.y) + L"Z" + std::to_wstring(item_eye.z) + L"\n").c_str());
		float targetdist = dist(selfEye, item_eye);
		
		if (targetdist > 0 && targetdist < closest_dist) 
		{ 
			closest_dist = targetdist;
			enemytransform = item_eye;
			// Format the enemy position as a string
			OutputDebugString(L"we here?{}" );
		}
		//TODO FROM HERE
	}

		//// Format the enemy position as a string
		//OutputDebugString((L"Closest Enemy Position: x=" + std::to_wstring(enemytransform.x) +
		//	L" y=" + std::to_wstring(enemytransform.y) + L" z=" + std::to_wstring(enemytransform.z) + L"\n").c_str());



	vec3 relativeAngle = (enemytransform - selfEye).RelativeAngle();

	//internal way
	//memory::Write<vec3>(base + cs2_dumper::offsets::client_dll::dwViewAngles,relativeAngle);

	//external way
	MoveCursorBasedOnAngle(enemytransform, selfEye);


	/*uintptr_t localplayer = memory*/
}


void Aimbot::MoveCursorBasedOnAngle(vec3 selfEye, vec3 enemytransform) {
	// Get the relative angle
	vec3 relativeAngle = (enemytransform - selfEye).RelativeAngle() ;

	// Convert relative angle to screen coordinates
	// These factors depend on the screen resolution and sensitivity settings
	const float screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const float screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Assuming relativeAngle is in degrees (adjust as necessary)
	float yaw = relativeAngle.y;  // Horizontal angle (yaw)
	float pitch = relativeAngle.x;  // Vertical angle (pitch)

	// Example scaling factor for mouse movement (adjust as needed for sensitivity)
	float sensitivity = 1.0f;

	// Convert angle to pixel movement
	int mouseX = static_cast<int>((yaw * sensitivity) + screenWidth / 2);  // Horizontal movement
	int mouseY = static_cast<int>((pitch * sensitivity) + screenHeight / 2);  // Vertical movement

	// Constrain mouse position within screen bounds
	mouseX = max(0, min(mouseX, static_cast<int>(screenWidth - 1)));
	mouseY = max(0, min(mouseY, static_cast<int>(screenHeight - 1)));

	// Move the mouse cursor to the new position
	SetCursorPos(mouseX, mouseY);
	OutputDebugString(L"Cursor moved based on angle\n");
}