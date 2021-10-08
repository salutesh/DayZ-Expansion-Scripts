/**
 * ExpansionBookMenuTabServerInfoSetting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabServerInfoSetting: ExpansionScriptView
{
	string m_Name;
	string m_Desc;
	ref ExpansionTooltipServerSettingEntry m_Tooltip;
	string m_Setting;

	ref ExpansionBookMenuTabServerInfoSettingController m_SettingEntryController;
		
	void ExpansionBookMenuTabServerInfoSetting(string name, string desc, string setting)
	{
		UpdateSetting(name, desc, setting);
		
		if (!m_SettingEntryController)
			m_SettingEntryController = ExpansionBookMenuTabServerInfoSettingController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/serverinfo/expansion_book_tab_serverinfo_setting_entry.layout";
	}
	
	void SetView()
	{
		m_SettingEntryController.SettingName = m_Name;
		m_SettingEntryController.NotifyPropertyChanged("SettingName");
		
		m_SettingEntryController.SettingState = m_Setting;	
		m_SettingEntryController.NotifyPropertyChanged("SettingState");
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabServerInfoSettingController;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == GetLayoutRoot())
		{
			m_Tooltip = new ExpansionTooltipServerSettingEntry(m_Name, m_Desc);
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == GetLayoutRoot())
		{
			delete m_Tooltip;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}

	//! Dynamically update setting
	//! Other mods can override this (calling super) to add their own settings
	void UpdateSetting(string name, string desc, string setting)
	{
		switch (name)
		{
			#ifdef EXPANSIONMOD
			//! BaseBuilding
			case "Expansion.Settings.BaseBuilding.CanCraftVanillaBasebuilding":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_CANCRAFTVANILLABASEBUILDING";
				m_Setting = BoolToString(GetExpansionSettings().GetBaseBuilding().CanCraftVanillaBasebuilding);
				break;
			case "Expansion.Settings.BaseBuilding.CanCraftExpansionBasebuilding":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_CANCRAFTEXPANSIONBASEBUILDING";
				m_Setting = BoolToString(GetExpansionSettings().GetBaseBuilding().CanCraftExpansionBasebuilding);
				break;

			//! Raid
			case "Expansion.Settings.Raid.CanRaidSafes":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_CANRAIDSAFES";
				m_Setting = BoolToString(GetExpansionSettings().GetRaid().CanRaidSafes);
				break;
			case "Expansion.Settings.Raid.SafeExplosionDamageMultiplier":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_SAFE_EXPLOSIONDAMAGEMULTIPLIER";
				m_Setting = GetExpansionSettings().GetRaid().SafeExplosionDamageMultiplier.ToString();
				break;
			case "Expansion.Settings.Raid.SafeProjectileDamageMultiplier":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_SAFE_PROJECTILEDAMAGEMULTIPLIER";
				m_Setting = GetExpansionSettings().GetRaid().SafeProjectileDamageMultiplier.ToString();
				break;
			case "Expansion.Settings.Raid.ExplosionTime":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONTIME";
				m_Setting = GetExpansionSettings().GetRaid().ExplosionTime.ToString();
				break;
			case "Expansion.Settings.Raid.ExplosionDamageMultiplier":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONDAMAGEMULTIPLIER";
				m_Setting = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier.ToString();
				break;
			case "Expansion.Settings.Raid.ProjectileDamageMultiplier":
				m_Name = "#STR_EXPANSION_SETTINGS_BB_PROJECTILEDAMAGEMULTIPLIER";
				m_Setting = GetExpansionSettings().GetRaid().ProjectileDamageMultiplier.ToString();
				break;

			//! Map
			case "Expansion.Settings.Map.NeedGPSItemForKeyBinding":
				m_Name = "#STR_EXPANSION_SETTINGS_GEN_NEEDGPSITEMFORKEYBINDING";
				m_Setting = BoolToString(GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding);
				break;
			case "Expansion.Settings.Map.NeedMapItemForKeyBinding":
				m_Name = "#STR_EXPANSION_SETTINGS_GEN_NEEDMAPITEMFORKEYBINDING";
				m_Setting = BoolToString(GetExpansionSettings().GetMap().NeedMapItemForKeyBinding);
				break;

			//! Party
			case "Expansion.Settings.Party.MaxMembersInParty":
				m_Name = "#STR_EXPANSION_SETTINGS_P_MAXINPARTY";
				m_Setting = GetExpansionSettings().GetParty().MaxMembersInParty.ToString();
				break;
			case "Expansion.Settings.Party.UseWholeMapForInviteList":
				m_Name = "#STR_EXPANSION_SETTINGS_P_USEWHOLEMAPFORINVITELIST";
				m_Setting = BoolToString(GetExpansionSettings().GetParty().UseWholeMapForInviteList);
				break;

			//! Territory
			case "Expansion.Settings.Territory.TerritorySize":
				m_Name = "#STR_EXPANSION_SETTINGS_T_TERRITORYSIZE";
				m_Setting = GetExpansionSettings().GetTerritory().TerritorySize.ToString();
				break;
			case "Expansion.Settings.Territory.UseWholeMapForInviteList":
				m_Name = "#STR_EXPANSION_SETTINGS_P_USEWHOLEMAPFORINVITELIST";
				m_Setting = BoolToString(GetExpansionSettings().GetTerritory().UseWholeMapForInviteList);
				break;
			#endif

			default:
				m_Name = name;
				m_Desc = desc;
				m_Setting = setting;
				break;
		}

		if (!m_Desc && m_Name.IndexOf("#STR_EXPANSION_") == 0)
			m_Desc = m_Name + "_DESC";
	}

	string BoolToString(bool state)
	{
		if (state)
			return "#STR_EXPANSION_BOOK_SETTINGS_ENABLED";
		return "#STR_EXPANSION_BOOK_SETTINGS_DISABLED";
	}
};

class ExpansionBookMenuTabServerInfoSettingController: ExpansionViewController 
{
	string SettingName;
	string SettingState;
};