/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class InGameMenu
{
	void InGameMenu()
	{
		if (GetExpansionClientSettings())
			GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);
	}

	void ~InGameMenu()
	{
		if (GetExpansionClientSettings())
			GetExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
	}

	override Widget Init()
	{
		layoutRoot = super.Init();

		if (GetExpansionClientSettings().StreamerMode)
		{
			Widget serverInfo = layoutRoot.FindAnyWidget("server_info");
			serverInfo.Show(false);
		}

		return layoutRoot;
	}

	void OnSettingChanged()
	{
		if (GetExpansionClientSettings())
		{
			Widget serverInfo = layoutRoot.FindAnyWidget("server_info");
			serverInfo.Show(!GetExpansionClientSettings().StreamerMode);
		}
	}

	override protected void SetGameVersion()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "SetGameVersion");
	#endif

		TextWidget version_widget = TextWidget.Cast(layoutRoot.FindAnyWidget("version"));

		GetDayZGame().Expansion_SetGameVersionText(version_widget);
	}
};