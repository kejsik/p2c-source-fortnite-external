//#include <wtypes.h>
//#include <string>
//#include <cstdint>
//#include <Windows.h>
//#include "offsets.h"
//#include "../memory/memory1.h"
//#include "../Includes/sdk.hpp"
//#include "../Includes/overlay.h"
//#include <thread>
//
//
//
//namespace ItemPawns
//{
//
//	bool IsHoldingAssaultRifle(std::string WeaponName) {
//		if (WeaponName == "Combat Assault Rifle" ||
//			WeaponName == "Assault Rifle  " ||
//			WeaponName == "Light Machine Gun" ||
//			WeaponName == "Scoped Assault Rifle" ||
//			WeaponName == "Makeshift Rifle" ||
//			WeaponName == "Sideways Rifle" ||
//			WeaponName == "Heavy Assault Rifle" ||
//			WeaponName == "Infantry Rifle" ||
//			WeaponName == "Suppressed Assault Rifle" ||
//			WeaponName == "Ranger Assault Rifle" ||
//			WeaponName == "Burst Assault Rifle" ||
//			WeaponName == "Primal Rifle" ||
//			WeaponName == "Striker Burst Rifle" ||
//			WeaponName == "Tactical Assault Rifle" ||
//			WeaponName == "Sideways Minigun" ||
//			WeaponName == "Burst Pulse Rifle" ||
//			WeaponName == "Jules' Drum Gun" ||
//			WeaponName == "Brutus' Minigun" ||
//			WeaponName == "Spire Guardian's Primal Assault Rifle" ||
//			WeaponName == "Drum Gun" ||
//			WeaponName == "Shadow Midas' Drum Gun" ||
//			WeaponName == "MK-Seven Assault Rifle" ||
//			WeaponName == "Slone's Burst Assault Rifle" ||
//			WeaponName == "Thermal Scoped Assault Rifle" ||
//			WeaponName == "Skye's Assault Rifle" ||
//			WeaponName == "Minigun" ||
//			WeaponName == "Meowscle's Peow Peow Rifle" ||
//			WeaponName == "Slone's Pulse Rifle" ||
//			WeaponName == "Combat Assault Rifle" ||
//			WeaponName == "Ocean's Burst Assault Rifle")
//			return true;
//
//		return false;
//	}
//
//	bool IsHoldingShotgun(std::string WeaponName) {
//		if (WeaponName == "Lever Action Shotgun" ||
//			WeaponName == "Combat Shotgun" ||
//			WeaponName == "Striker Pump Shotgun" ||
//			WeaponName == "Tactical Shotgun" ||
//			WeaponName == "Ranger Shotgun" ||
//			WeaponName == "Primal Shotgun" ||
//			WeaponName == "Pump Shotgun" ||
//			WeaponName == "Auto Shotgun" ||
//			WeaponName == "Charge Shotgun" ||
//			WeaponName == "Drum Shotgun" ||
//			WeaponName == "Dragon's Breath Shotgun" ||
//			WeaponName == "Spire Assassin's Primal Shotgun" ||
//			WeaponName == "Kit's Charge Shotgun")
//			return true;
//
//		return false;
//	}
//
//	bool IsHoldingSMG(std::string WeaponName) {
//		if (WeaponName == "Combat SMG" ||
//			WeaponName == "Burst SMG" ||
//			WeaponName == "Primal SMG" ||
//			WeaponName == "Tactical Submachine Gun" ||
//			WeaponName == "Rapid Fire SMG" ||
//			WeaponName == "Suppressed Submachine Gun" ||
//			WeaponName == "Makeshift Submachine Gun" ||
//			WeaponName == "Compact SMG" ||
//			WeaponName == "Kymera Ray Gun" ||
//			WeaponName == "Zyg and Choppy's Ray Gun")
//			return true;
//
//		return false;
//	}
//
//	bool IsHoldingPistol(std::string WeaponName) {
//		if (WeaponName == "Hand Cannon" ||
//			WeaponName == "Sidearm Pistol" ||
//			WeaponName == "Revolver" ||
//			WeaponName == "Scoped Revolver" ||
//			WeaponName == "Dual Pistols" ||
//			WeaponName == "Suppressed Pistol" ||
//			WeaponName == "Flint-Knock Pistol" ||
//			WeaponName == "Tactical Pistol")
//			return true;
//
//		return false;
//	}
//
//	bool IsHoldingSniper(std::string WeaponName) {
//		if (WeaponName == "Storm Scout" ||
//			WeaponName == "Dragon's Breath Sniper" ||
//			WeaponName == "Boom Sniper Rifle" ||
//			WeaponName == "Storm Scout Sniper Rifle" ||
//			WeaponName == "Hunter Bolt-Action Sniper" ||
//			WeaponName == "Rail Gun" ||
//			WeaponName == "Heavy Sniper Rifle" ||
//			WeaponName == "Suppressed Sniper Rifle" ||
//			WeaponName == "Bolt-Action Sniper Rifle" ||
//			WeaponName == "Lever Action Rifle" ||
//			WeaponName == "Automatic Sniper Rifle" ||
//			WeaponName == "Semi-Auto Sniper Rifle   ")
//			return true;
//
//		return false;
//	}
//
//	VOID World()
//	{
//
//		while (true)
//		{
//
//			uint64_t CurrentWeapon = drv->Read<uintptr_t>(pointer::local_pawn + offsets::CurrentWeapon);
//			// std::cout << " CurrentWeapon " << CurrentWeapon << std::endl;
//			if (CurrentWeapon)
//				continue;
//
//			uintptr_t WeaponData = drv->Read<uintptr_t>(CurrentWeapon + offsets::WeaponData);
//			// std::cout << " WeaponData " << WeaponData << std::endl;
//			if (!WeaponData)
//				continue;
//
//			uintptr_t DisplayName = drv->Read<uintptr_t>(WeaponData + 0x90);
//			// std::cout << " DisplayName " << DisplayName << std::endl;
//			if (!DisplayName)
//				continue;
//
//			int Tier = drv->Read<BYTE>(WeaponData + 0x73); // DisplayTier 0xbfe
//			uint32_t WeaponLength = drv->Read<uint32_t>(DisplayName + 0x38);
//			wchar_t* WeaponName = new wchar_t[uintptr_t(WeaponLength) + 1];
//			std::wstring wWeaponName(WeaponName);
//			std::string InventoryWeapon(wWeaponName.begin(), wWeaponName.end());
//
//			for (unsigned long x = 0; x <= WeaponLength; x++)
//			{
//				uintptr_t WorldLevels = drv->Read<uintptr_t>(pointer::uworld + 0x170); // levels
//				// std::cout << " WorldLevels " << WorldLevels << std::endl;
//				if (!WorldLevels)
//					continue;
//
//				uintptr_t ItemLevels = drv->Read<uintptr_t>(WorldLevels + (x * sizeof(uintptr_t)));
//				// std::cout << " ItemLevels " << ItemLevels << std::endl;
//				if (!ItemLevels)
//					continue;
//
//				for (unsigned long a = 0; a < drv->Read<DWORD>(ItemLevels + (0x98 + sizeof(PVOID))); ++a)
//				{
//					uintptr_t ItemsPawns = drv->Read<uintptr_t>(ItemLevels + 0x98);
//					// std::cout << " ItemsPawns " << ItemsPawns << std::endl;
//					if (!ItemsPawns)
//						continue;
//
//					uintptr_t CurrentActor = drv->Read<uintptr_t>(ItemsPawns + (a * sizeof(uintptr_t)));
//					// std::cout << " CurrentActor " << CurrentActor << std::endl;
//					if (!CurrentActor)
//						continue;
//
//					INT32 ObjectID = drv->Read<INT32>(CurrentActor + 0x18);
//					// std::cout << " ObjectID " << ObjectID << std::endl;
//					if (!ObjectID)
//						continue;
//
//					std::string ItemFName = find_object_name(ObjectID);
//					if (ItemFName == "") continue;
//
//					// Basic Object names
//					if (ItemFName.find("Tiered_Chest") != std::string::npos ||
//						ItemFName.find("Tiered_Ammo") != std::string::npos ||
//						ItemFName.find("FortPickupAthena") != std::string::npos ||
//						ItemFName.find("AthenaSupplyDrop_Llama") != std::string::npos ||
//						ItemFName.find("AthenaSupplyDrop_C") != std::string::npos ||
//						ItemFName.find("Valet_BasicCar_") != std::string::npos ||
//						ItemFName.find("BGA_Athena_FlopperSpawn_World_C") != std::string::npos ||
//						ItemFName.find("CBGA_ShieldsSmall_C") != std::string::npos ||
//						ItemFName.find("Athena_Prop_SilkyBingo_C") != std::string::npos)
//					{
//
//						BOOL bHidden = ((drv->Read<int>(CurrentActor + 0x58) >> 7) & 1);
//						BOOL bPickedUp = drv->Read<BOOL>(CurrentActor + 0x5c8);
//						FLOAT DespawnTime = drv->Read<FLOAT>(CurrentActor + 0x720);
//						FColor ItemColor;
//
//						if (!bHidden && !bPickedUp)
//							continue;
//
//						uint64_t RootComponent = drv->Read<uintptr_t>(CurrentActor + offsets::RootComponent);
//						pointer::local_relative_location = drv->Read <Vector3>(RootComponent + offsets::RelativeLocation);
//						double ItemDistance = VirtualCamera.Location.Distance(RelativeLocation);
//						Vector3 ScreenPosition = ProjectWorldToScreen(RelativeLocation);
//
//						switch (Tier)
//						{
//						case 0: // Invalid
//							ItemColor.R = 0;
//							ItemColor.G = 0;
//							ItemColor.B = 0;
//							break;
//						case 1: // Common
//							ItemColor.R = 129;
//							ItemColor.G = 137;
//							ItemColor.B = 145;
//							break;
//						case 2: // Uncommon
//							ItemColor.R = 115;
//							ItemColor.G = 175;
//							ItemColor.B = 57;
//							break;
//						case 3: // Rare
//							ItemColor.R = 111;
//							ItemColor.G = 171;
//							ItemColor.B = 54;
//							break;
//						case 4: // Epic
//							ItemColor.R = 141;
//							ItemColor.G = 62;
//							ItemColor.B = 188;
//							break;
//						case 5: // Legendary
//							ItemColor.R = 230;
//							ItemColor.G = 167;
//							ItemColor.B = 76;
//							break;
//						case 6: // Mythic
//							ItemColor.R = 224;
//							ItemColor.G = 202;
//							ItemColor.B = 88;
//							break;
//						case 7: // Exotic
//							ItemColor.R = 132;
//							ItemColor.G = 211;
//							ItemColor.B = 178;
//							break;
//						default: // Other
//							ItemColor.R = 254;
//							ItemColor.G = 255;
//							ItemColor.B = 255;
//							break;
//						}
//
//						std::string TextItem = DisplayName + " [" + std::to_string(ItemDistance) + "m]";
//						ImVec2 ItemSize = ImGui::CalcTextSize(TextItem.c_str());
//						ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPosition.x, ScreenPosition.y), ImVec2(screen, settings::height), ImColor(255, 255, 255), 0.5f);
//						ImGui::GetBackgroundDrawList()->AddText(ImVec2(ScreenPosition.x - (ItemSize.x / 2), ScreenPosition.y), ImColor(ItemColor.R, ItemColor.G, ItemColor.B, 255), TextItem.c_str());
//
//						// testing
//						std::string TextDespawn = "Despawn Time (" + std::to_string(DespawnTime) + ")";
//						ImVec2 DespawnSize = ImGui::CalcTextSize(TextDespawn.c_str());
//						ImGui::GetBackgroundDrawList()->AddText(ImVec2(ScreenPosition.x - (DespawnSize.x / 2), ScreenPosition.y + 20), ImColor(255, 255, 255), TextDespawn.c_str());
//
//			
//					}
//
//				}
//
//				std::this_thread::sleep_for(std::chrono::milliseconds(800));
//
//			}
//
//		}
//
//	}
//
//}