#include "WeaponEdit.h"

#include <fstream>
#include <string>
#include <sstream>

Weapon::Weapon()
	:m_Weapon_FilePath("cstrike/addons/amxmodx/configs/nst_wpn.ini"),
	m_WareHouse_FilePath("cstrike/addons/amxmodx/configs/nst_mywpn.ini"),
	m_Texture_FilePath("cstrike/gfx/vgui/")
{
	showWeaponEdit = false;

	LoadWeaponInfo(m_Weapon_FilePath);
	LoadWeaponTexture(m_Texture_FilePath);
	LoadWareHouseInfo(m_WareHouse_FilePath);
	SetImGuiStyle();
}

Weapon::~Weapon()
{

}

void Weapon::OnUpdate()
{

}

void Weapon::OnImguiRender()
{

	ImGui::DockSpaceOverViewport(ImGuiDockNodeFlags_None, ImGui::GetMainViewport());

	ShowMainMenuBar();

	if(ShowKey)
		MainMenu();
	else
		WareHouseMenu();

	if (showWeaponEdit && !m_Weapons[m_Type][m_Current_Name].Weapon_Name.empty())
		EditWeapon(m_Weapons[m_Type][m_Current_Name]);
}

void Weapon::EditWeapon(WeaponInfo& weapon)
{
	
	WeaponInfo Orig_weapon = weapon;
	ImGui::Begin("WeaponEdit");

	int borderWidth = 4;
	if (m_Type == WeaponType::SUPER)
		m_BorderColor = { 0.2f, 0.7f, 0.7f, 1.0f };
	else if (m_Type == WeaponType::RIFLE)
		m_BorderColor = { 0.2f, 0.1f, 0.8f, 1.0f };
	else if (m_Type == WeaponType::PISTOL)
		m_BorderColor = { 0.7f, 0.1f, 0.7f, 1.0f };
	else if (m_Type == WeaponType::KNIFE)
		m_BorderColor = { 0.5f, 0.3f, 0.1f, 1.0f };
	else if (m_Type == WeaponType::BOM)
		m_BorderColor = { 0.5f, 0.5f, 0.8f, 1.0f };

	ImVec2 cursorPos = ImGui::GetCursorScreenPos();

	ImGui::GetWindowDrawList()->AddRect(cursorPos, ImVec2(cursorPos.x + m_Image_Size.x, cursorPos.y + m_Image_Size.y), ImColor(m_BorderColor), 0.0f, 0, borderWidth);
	ImGui::Image((void*)(intptr_t)weapon.RendererID, m_Image_Size);

	ImGui::SameLine();
	ImGui::BeginGroup();
	if (ImGui::Button("Save"))
		SaveWeaponInfo();
	if (ImGui::Button("Close"))
		showWeaponEdit = false;
	ImGui::EndGroup();

	float windowWidth = m_Image_Size.x;
	float offsetX = (windowWidth - ImGui::CalcTextSize(weapon.Weapon_Name.c_str()).x) * 0.5f;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
	ImGui::Text(weapon.Weapon_Name.c_str());

	ImGui::BeginGroup();
	if (m_Weapons[WeaponType::BOM].find(weapon.Current_Name) != m_Weapons[WeaponType::BOM].end()
		|| m_Weapons[WeaponType::KNIFE].find(weapon.Current_Name) != m_Weapons[WeaponType::KNIFE].end())
	{
		// Knife and Bom doesn't have Clip and Ammo
		ImGui::SliderInt("Damage", &weapon.Damage, 1, 1000, "Damage = %d");
	}
	else
	{
		ImGui::SliderInt("Clip", &weapon.Clip, 1, 200, "Clip = %d");
		ImGui::SliderInt("Ammo", &weapon.Ammo, 1, 254, "Ammo = %d");
		ImGui::SliderInt("Damage", &weapon.Damage, 1, 1000, "Damage = %d");
	}

	if (weapon.M203_Ammo > 0 && weapon.M203_Damage > 0)
	{
		ImGui::SliderInt("M203-N", &weapon.M203_Ammo, 1, 200, "M203-Ammo = %d");
		ImGui::SliderInt("M203-D", &weapon.M203_Damage, 100, 200000, "M203-Damage = %d");
	}

	if (weapon.LG_Ammo > 0 && weapon.LG_Damage > 0)
	{
		ImGui::SliderInt("LG-N", &weapon.LG_Ammo, 1, 200, "LG-Ammo = %d");
		ImGui::SliderInt("LG-D", &weapon.LG_Damage, 100, 160000, "LG-Damage = %d");
	}
	ImGui::EndGroup();

	if (m_WareHouse_Weapons[m_Type].find(weapon.Current_Name) != m_WareHouse_Weapons[m_Type].end())
	{
		if (ImGui::Button("Move out from Warehouse"))
		{
			m_WareHouse_Weapons[m_Type].erase(weapon.Current_Name);
			std::vector<std::string> temp;
			temp.resize(m_WareHouse_Sperater[m_Type].size() - 1);
			int index = 0;
			for (int i = 0; i < m_WareHouse_Sperater[m_Type].size(); i++)
			{
				if (m_WareHouse_Sperater[m_Type][i] == weapon.Current_Name)
					continue;
				temp[index] = m_WareHouse_Sperater[m_Type][i];
				index++;
			}

			m_WareHouse_Sperater[m_Type].clear();
			m_WareHouse_Sperater[m_Type] = temp;
		}
	}
	else
	{
		
		// Super Weapon can't put into Weapon Warehouse
		if(m_Type != WeaponType::SUPER)
			if (ImGui::Button("Move into Warehouse"))
			{
				if (m_WareHouse_Sperater[m_Type].size() < m_Warehouse_Size[m_Type])
				{
					m_WareHouse_Weapons[m_Type].insert({ weapon.Current_Name, weapon });
					std::vector<std::string> temp;
					temp.push_back(weapon.Current_Name);
					for (const auto& warehouse : m_WareHouse_Sperater[m_Type])
						temp.push_back(warehouse);
					m_WareHouse_Sperater[m_Type].clear();
					m_WareHouse_Sperater[m_Type] = temp;
				}
				else
				{
					ShowError = true;
					ErrorWeaponName = weapon.Weapon_Name;
				}
			}
	}

	if (ShowError)
	{
		std::string info = "Failed move weapon " + ErrorWeaponName +
			"into Warehouse\n" +
			"The Warehouse is full!\n" +
			"Please remove some weapon before move into Warehouse";
		ShowErrorText(info);
	}

	if (Orig_weapon != weapon)
		SaveWeaponEdit(weapon);

	ImGui::End();
}

void Weapon::MainMenu()
{

	ImGui::Begin("Weapons");

	ImGui::Columns(5);
	int index = 0;
	for (auto& weapons : m_Weapons)
	{
		for (int i = 0; i < weapons.second.size(); i++)
		{
			float windowWidth = ImGui::GetColumnWidth();
			float imageWidth = static_cast<float>(235);
			float imageHeight = static_cast<float>(81);
			float offsetX = (windowWidth - imageWidth) * 0.5f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			int borderWidth = 4;
			if (weapons.first == WeaponType::SUPER)
				m_BorderColor = { 0.2f, 0.7f, 0.7f, 1.0f };
			else if(weapons.first == WeaponType::RIFLE)
				m_BorderColor = { 0.2f, 0.1f, 0.8f, 1.0f };
			else if (weapons.first == WeaponType::PISTOL)
				m_BorderColor = { 0.7f, 0.1f, 0.7f, 1.0f };
			else if (weapons.first == WeaponType::KNIFE)
				m_BorderColor = { 0.5f, 0.3f, 0.1f, 1.0f };
			else if (weapons.first == WeaponType::BOM)
				m_BorderColor = { 0.5f, 0.5f, 0.8f, 1.0f };

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRect(cursorPos, ImVec2(cursorPos.x + imageWidth, cursorPos.y + imageHeight), ImColor(m_BorderColor), 0.0f, 0, borderWidth);
			ImGui::Image((void*)(intptr_t)weapons.second.at(m_Weapon_Position[index]).RendererID, m_Image_Size);

			offsetX = (windowWidth - ImGui::CalcTextSize(weapons.second.at(m_Weapon_Position[index]).Weapon_Name.c_str()).x) * 0.5f;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::Text(weapons.second.at(m_Weapon_Position[index]).Weapon_Name.c_str());

			ImVec2 mousePos = ImGui::GetMousePos();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				if (mousePos.x >= cursorPos.x && mousePos.x <= cursorPos.x + m_Image_Size.x &&
					mousePos.y >= cursorPos.y && mousePos.y <= cursorPos.y + m_Image_Size.y)
				{
					m_Type = weapons.first;
					m_Current_Name = m_Weapon_Position[index];
					showWeaponEdit = true;
				}
			}
			index++;
			ImGui::NextColumn();
		}
	}
	ImGui::End();
}

void Weapon::WareHouseMenu()
{

	ImGui::Begin("WareHouse");

	ImGui::Columns(5);
	
	for (auto& warehouse : m_WareHouse_Weapons)
	{
		int index = 0;
		for (int i = 0; i < warehouse.second.size(); i++)
		{
			float windowWidth = ImGui::GetColumnWidth();
			float imageWidth = static_cast<float>(235);
			float imageHeight = static_cast<float>(81);
			float offsetX = (windowWidth - imageWidth) * 0.5f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			int borderWidth = 4;

			if (warehouse.first == WeaponType::SUPER)
				m_BorderColor = { 0.2f, 0.7f, 0.7f, 1.0f };
			else if (warehouse.first == WeaponType::RIFLE)
				m_BorderColor = { 0.2f, 0.1f, 0.8f, 1.0f };
			else if (warehouse.first == WeaponType::PISTOL)
				m_BorderColor = { 0.7f, 0.1f, 0.7f, 1.0f };
			else if (warehouse.first == WeaponType::KNIFE)
				m_BorderColor = { 0.5f, 0.3f, 0.1f, 1.0f };
			else if (warehouse.first == WeaponType::BOM)
				m_BorderColor = { 0.5f, 0.5f, 0.8f, 1.0f };

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddRect(cursorPos, ImVec2(cursorPos.x + imageWidth, cursorPos.y + imageHeight), ImColor(m_BorderColor), 0.0f, 0, borderWidth);
			ImGui::Image((void*)(intptr_t)warehouse.second.at(m_WareHouse_Sperater[warehouse.first][index]).RendererID, m_Image_Size);

			offsetX = (windowWidth - ImGui::CalcTextSize(warehouse.second.at(m_WareHouse_Sperater[warehouse.first][index]).Weapon_Name.c_str()).x) * 0.5f;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
			ImGui::Text(warehouse.second.at(m_WareHouse_Sperater[warehouse.first][index]).Weapon_Name.c_str());

			ImVec2 mousePos = ImGui::GetMousePos();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				if (mousePos.x >= cursorPos.x && mousePos.x <= cursorPos.x + m_Image_Size.x &&
					mousePos.y >= cursorPos.y && mousePos.y <= cursorPos.y + m_Image_Size.y)
				{
					m_Type = warehouse.first;
					m_Current_Name = m_WareHouse_Sperater[warehouse.first][index];
					showWeaponEdit = true;
				}
			}
			index++;
			ImGui::NextColumn();
		}
	}

	ImGui::End();
}

void Weapon::SetImGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = m_Color;
	style.Colors[ImGuiCol_Button] = { 0.7f, 0.4f, 0.7f, 1.0f };;
	style.Colors[ImGuiCol_ButtonHovered] = { 0.2f, 0.7f, 0.7f, 1.0f };
	style.Colors[ImGuiCol_ButtonActive] = { 0.2f, 0.7f, 0.7f, 1.0f };
}

void Weapon::OnRender()
{
	
}



void Weapon::LoadWeaponInfo(std::string filepath)
{
	std::ifstream file(filepath);

	if (!file.is_open())
		return;
	else
	{
		std::string line;
		std::getline(file, line);
		WeaponType type = WeaponType::SUPER;

		while (std::getline(file, line))
		{
			WeaponInfo weapon;

			if (line.size() < 30)
			{
				if (line.find(";Rifles") != std::string::npos)
					type = WeaponType::RIFLE;
				else if (line.find(";Pistols") != std::string::npos)
					type = WeaponType::PISTOL;
				else if (line.find(";Knife") != std::string::npos)
					type = WeaponType::KNIFE;
				else if (line.find(";Bom") != std::string::npos)
					type = WeaponType::BOM;
				continue;
			}
			else
			{
				weapon.Attribute = line;

				int begin = line.find("[name]");
				int end = line.find("[model]");
				std::string weaponName = line.substr(begin + 6, end - begin - 6);
				weapon.Weapon_Name = weaponName;

				begin = line.find("[model]");
				end = line.find("[wchange]");
				std::string currentName = line.substr(begin + 7, end - begin - 7);
				weapon.Current_Name = currentName;


				if (type == WeaponType::BOM || type == WeaponType::KNIFE)
				{
					if (line.find("[damage]") != std::string::npos && line.find("[gravity]") != std::string::npos)
					{
						begin = line.find("[damage]");
						end = line.find("[gravity]");
						std::string damage = line.substr(begin + 8, end - begin - 8);
						weapon.Damage = std::stoi(damage);
					}
				}
				else
				{
					if (line.find("[damage]") != std::string::npos && line.find("[speed]") != std::string::npos)
					{
						begin = line.find("[damage]");
						end = line.find("[speed]");
						std::string damage = line.substr(begin + 8, end - begin - 8);
						weapon.Damage = std::stof(damage);
					}
				}

				if (line.find("[clip]") != std::string::npos && line.find("[ammo]") != std::string::npos)
				{
					begin = line.find("[clip]");
					end = line.find("[ammo]");
					std::string clip = line.substr(begin + 6, end - begin - 6);
					weapon.Clip = std::stoi(clip);
				}

				if (line.find("[ammo]") != std::string::npos && line.find("[recoil]") != std::string::npos)
				{
					begin = line.find("[ammo]");
					end = line.find("[recoil]");
					std::string ammo = line.substr(begin + 6, end - begin - 6);
					weapon.Ammo = std::stoi(ammo);
				}

				if (line.find("[l_nade]") != std::string::npos && line.find("[l_costammo]") != std::string::npos)
				{
					begin = line.find("[l_nade]");
					end = line.find("[l_costammo]");
					std::string m203_Ammo = line.substr(begin + 8, end - begin - 8);
					weapon.M203_Ammo = std::stoi(m203_Ammo);
				}

				if (line.find("[l_damage]") != std::string::npos && line.find("[l_type]") != std::string::npos)
				{
					begin = line.find("[l_damage]");
					end = line.find("[l_type]");
					std::string m203_Damage = line.substr(begin + 10, end - begin - 10);
					weapon.M203_Damage = std::stoi(m203_Damage);
				}

				if (line.find("[lg_ammo]") != std::string::npos && line.find("[lg_costammo]") != std::string::npos)
				{
					begin = line.find("[lg_ammo]");
					end = line.find("[lg_costammo]");
					std::string lg_Ammo = line.substr(begin + 9, end - begin - 9);
					weapon.LG_Ammo = std::stoi(lg_Ammo);
				}

				if (line.find("[lg_damage]") != std::string::npos && line.find("[lg_radius]") != std::string::npos)
				{
					begin = line.find("[lg_damage]");
					end = line.find("[lg_radius]");
					std::string lg_Damage = line.substr(begin + 11, end - begin - 11);
					weapon.LG_Damage = std::stoi(lg_Damage);
				}
			}

			m_Weapons[type].insert({ weapon.Current_Name, weapon });
			m_Weapon_Position.push_back(weapon.Current_Name);

		}

		file.close();
	}
}

void Weapon::LoadWareHouseInfo(std::string filepath)
{
	std::ifstream file(filepath);

	if (!file.is_open())
	{
		ImGui::Begin("Error");
		ImGui::Text("Error: Failed to open file %s!", filepath);
		ImGui::End();
		return;
	}

	std::string line;

	WeaponType type;
	while (std::getline(file, line))
	{
		if (line.find("RIFLES") != std::string::npos)
			type = WeaponType::RIFLE;
		else if (line.find("PISTOLS") != std::string::npos)
			type = WeaponType::PISTOL;
		else if (line.find("KNIFE") != std::string::npos)
			type = WeaponType::KNIFE;
		else if (line.find("HEGRENADE") != std::string::npos)
			type = WeaponType::BOM;
		else
		{
			if(line.size() > 0 && line.size() < 12)
				m_WareHouse_Title += ("\n" + line + "\n");
			else if(line.size() >= 12)
				m_WareHouse_Title += (line + "\n");
			continue;
		}

		//Delete before "="
		uint32_t pos = line.find("=");
		line.erase(0, pos + 1);

		std::stringstream ss(line);

		std::string name;

		while (std::getline(ss, name, ','))
		{
			name.erase(0, name.find_first_not_of(' '));
			name.erase(name.find_last_not_of(' ') + 1);

			if (m_Weapons[type].find(name) != m_Weapons[type].end())
			{
				WeaponInfo weapon = m_Weapons[type][name];

				m_WareHouse_Weapons[type].insert({ name, weapon });
				m_WareHouse_Sperater[type].push_back(name);
			}
		}

	}


	file.close();

}

void Weapon::LoadWeaponTexture(std::string filepath)
{
	int index = 0;
	for (auto& weapon : m_Weapons)
		for (auto& info : weapon.second)
		{
			std::string path = filepath + info.second.Current_Name + ".tga";
			m_Texture[info.second.Current_Name] = std::make_shared<Texture>(path, false);
			info.second.RendererID = m_Texture[info.second.Current_Name]->GetRendererID();
		}
}

void Weapon::SaveWeaponInfo()
{
	std::ofstream file(m_Weapon_FilePath);

	if (!file.is_open())
	{
		std::cerr << "Failed To Open File For Save." << std::endl;
		return;
	}

	int index = 0;
	for (const auto& weapons : m_Weapons)
	{
		switch (weapons.first)
		{
		case WeaponType::SUPER:
			file << ";Super" << std::endl;
			break;
		case WeaponType::RIFLE:
			file << "\n\n\n;Rifles" << std::endl;
			break;
		case WeaponType::PISTOL:
			file << "\n\n\n;Pistols" << std::endl;
			break;
		case WeaponType::KNIFE:
			file << "\n\n\n;Knife" << std::endl;
			break;
		case WeaponType::BOM:
			file << "\n\n\n;Bom" << std::endl;
			break;
		}
		for (int i = 0; i < weapons.second.size(); i++)
		{
			file << weapons.second.at(m_Weapon_Position[index]).Attribute << std::endl;
			index++;
		}
	}
	file.close();
}
void Weapon::SaveWareHouseInfo()
{
	std::ofstream file(m_WareHouse_FilePath);

	if (!file.is_open())
	{
		std::cerr << "Failed To Open File "<< m_WareHouse_FilePath <<"For Save." << std::endl;
		return;
	}

	file << m_WareHouse_Title << std::endl;

	
	bool space = false;
	for (const auto& warehouse : m_WareHouse_Weapons)
	{
		int index = 0;
		space = false;
		if (warehouse.first == WeaponType::RIFLE)
			file << "RIFLES =";
		else if (warehouse.first == WeaponType::PISTOL)
			file << "PISTOLS =";
		else if (warehouse.first == WeaponType::KNIFE)
			file << "KNIFE =";
		else if (warehouse.first == WeaponType::BOM)
			file << "HEGRENADE =";
		for (int i = 0; i < warehouse.second.size(); i++)
		{
			if (space)
				file << ",";
			space = true;
			file << " " << m_WareHouse_Sperater[warehouse.first][index];
			index++;
		}
		file << "\n\n";
	}
}

void Weapon::SaveWeaponEdit(WeaponInfo& weapon)
{
	weapon.Attribute;
	std::string line = weapon.Attribute;
	std::string right;
	std::string left;
	std::string mid;

	int begin;
	int end;

	if (m_Weapons[WeaponType::BOM].find(weapon.Current_Name) != m_Weapons[WeaponType::BOM].end()
		|| m_Weapons[WeaponType::KNIFE].find(weapon.Current_Name) != m_Weapons[WeaponType::KNIFE].end())
	{
		if (line.find("[damage]") != std::string::npos && line.find("[gravity]") != std::string::npos)
		{
			begin = line.find("[damage]");
			end = line.find("[gravity]");
			std::string right = line.substr(0, begin + 8);
			std::string left = line.substr(end, line.size() - end);
			std::string mid = std::to_string(weapon.Damage);

			line = right + mid + left;
		}
	}
	else
	{
		if (line.find("[damage]") != std::string::npos && line.find("[speed]") != std::string::npos)
		{
			begin = line.find("[damage]");
			end = line.find("[speed]");
			right = line.substr(0, begin + 8);
			left = line.substr(end, line.size() - end);
			mid = std::to_string(weapon.Damage);

			line = right + mid + left;
		}
	}

	if (line.find("[clip]") != std::string::npos && line.find("[ammo]") != std::string::npos)
	{
		begin = line.find("[clip]");
		end = line.find("[ammo]");
		right = line.substr(0, begin + 6);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.Clip);

		line = right + mid + left;
	}

	if (line.find("[ammo]") != std::string::npos && line.find("[recoil]") != std::string::npos)
	{
		begin = line.find("[ammo]");
		end = line.find("[recoil]");
		right = line.substr(0, begin + 6);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.Ammo);

		line = right + mid + left;
	}

	if (line.find("[l_nade]") != std::string::npos && line.find("[l_costammo]") != std::string::npos)
	{
		begin = line.find("[l_nade]");
		end = line.find("[l_costammo]");
		right = line.substr(0, begin + 8);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.M203_Ammo);

		line = right + mid + left;
	}

	if (line.find("[l_damage]") != std::string::npos && line.find("[l_type]") != std::string::npos)
	{
		begin = line.find("[l_damage]");
		end = line.find("[l_type]");
		right = line.substr(0, begin + 10);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.M203_Damage);

		line = right + mid + left;
	}

	if (line.find("[lg_ammo]") != std::string::npos && line.find("[lg_costammo]") != std::string::npos)
	{
		begin = line.find("[lg_ammo]");
		end = line.find("[lg_costammo]");
		right = line.substr(0, begin + 9);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.LG_Ammo);

		line = right + mid + left;
	}

	if (line.find("[lg_damage]") != std::string::npos && line.find("[lg_radius]") != std::string::npos)
	{
		begin = line.find("[lg_damage]");
		end = line.find("[lg_radius]");
		right = line.substr(0, begin + 11);
		left = line.substr(end, line.size() - end);
		mid = std::to_string(weapon.LG_Damage);

		line = right + mid + left;
	}

	weapon.Attribute = line;

}

void Weapon::ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Show All Weapon"))
		{
			ShowKey = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Show Warehouse"))
		{
			ShowKey = false;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Weapon::ShowErrorText(std::string info)
{
	ImGui::Begin("Error");
	if (ImGui::Button("Close"))
		ShowError = false;

	ImGui::Text("%s", info.c_str());
	ImGui::End();
}
