#pragma once

#pragma once
#include "Test/Test.h"
#include "imgui/imgui.h"
#include "OpenGL/Texture.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

struct WeaponInfo
{
	std::string Attribute;
	std::string Weapon_Name;
	std::string Current_Name;
	int Ammo = 0;
	int Clip = 0;
	int Damage = 0;

	int M203_Damage = 0;
	int M203_Ammo = 0;

	int LG_Ammo = 0;
	int LG_Damage = 0;
	unsigned int RendererID = 0;

	WeaponInfo operator=(const WeaponInfo& weapon)
	{
		this->Attribute = weapon.Attribute;
		this->Weapon_Name = weapon.Weapon_Name;
		this->Current_Name = weapon.Current_Name;

		this->Ammo = weapon.Ammo;
		this->Clip = weapon.Clip;
		this->Damage = weapon.Damage;
		this->M203_Ammo = weapon.M203_Ammo;
		this->M203_Damage = weapon.M203_Damage;
		this->LG_Ammo = weapon.LG_Ammo;
		this->LG_Damage = weapon.LG_Damage;
		this->RendererID = weapon.RendererID;
		return *this;
	}

	bool operator!=(const WeaponInfo& weapon)
	{
		if (this->Ammo != weapon.Ammo
			|| this->Clip != weapon.Clip
			|| this->Damage != weapon.Damage
			|| this->M203_Ammo != weapon.M203_Ammo
			|| this->M203_Damage != weapon.M203_Damage
			|| this->LG_Ammo != weapon.LG_Ammo
			|| this->LG_Damage != weapon.LG_Damage)
			return true;
		return false;
	}

	bool operator==(const WeaponInfo& weapon)
	{
		return !operator!=(weapon);
	}

};

enum WeaponType
{
	SUPER = 0,
	RIFLE = 1,
	PISTOL = 2,
	KNIFE = 3,
	BOM = 4
};

class Weapon : public Test
{
public:
	Weapon();
	~Weapon();

	void LoadWeaponInfo(std::string filepath);
	void LoadWareHouseInfo(std::string filepath);
	void LoadWeaponTexture(std::string filepath);


	void SaveWeaponInfo();
	void SaveWareHouseInfo();
	void SaveWeaponEdit(WeaponInfo& weapon);

	void ShowMainMenuBar();
	void ShowErrorText(std::string info);
	void SetImGuiStyle();

	void MainMenu();
	void WareHouseMenu();
	void EditWeapon(WeaponInfo& weapon);

	void OnUpdate() override;
	void OnImguiRender() override;
	void OnRender() override;
private:
	// Use map and unordered_map save weapon info
	std::map<WeaponType, std::unordered_map<std::string, WeaponInfo>> m_Weapons; 
	std::vector<std::string> m_Weapon_Position;

	std::map<WeaponType, std::unordered_map<std::string, WeaponInfo>> m_WareHouse_Weapons;
	std::string m_WareHouse_Title;
	std::map<WeaponType, std::vector<std::string>> m_WareHouse_Sperater;
	std::map<WeaponType, uint32_t> m_Warehouse_Size = {
		{ WeaponType::RIFLE, 11 },
		{ WeaponType::PISTOL, 4 },
		{ WeaponType::KNIFE, 4 },
		{ WeaponType::BOM, 5 }
	};

	std::string m_Weapon_FilePath;
	std::string m_WareHouse_FilePath;
	std::string m_Texture_FilePath;

	std::unordered_map<std::string, unsigned int> m_RendererID;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Texture;

	ImVec4 m_Color = { 0.1f, 0.1f, 0.1f, 1.0f };
	ImVec4 m_BorderColor = { 0.2f, 0.3f, 0.7f, 1.0f };
	ImVec2 m_Image_Size = { 235, 81 };

	WeaponType m_Type = WeaponType::SUPER;
	std::string m_Current_Name = "";

	bool showWeaponEdit;
	bool ShowKey = true;
	bool ShowError = false;
	std::string ErrorWeaponName;

};
