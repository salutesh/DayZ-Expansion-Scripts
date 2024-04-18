/**
 * ExpansionPersonalStorageUpgrade.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageUpgrade: ItemBase
{
	int m_Expansion_PersonalStorageUpgradeLevel;

	void ExpansionPersonalStorageUpgrade()
	{
		string type = GetType();
		int index = type.IndexOf("_Level") + 6;
		m_Expansion_PersonalStorageUpgradeLevel = type.Substring(index, type.Length() - index).ToInt();
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUpgradePersonalStorage);
	}

	bool Expansion_CanUpgradePersonalStorage(PlayerBase player)
	{
	#ifdef EXPANSIONMODHARDLINE
		auto settings = GetExpansionSettings().GetHardline(false);
		if (settings.IsLoaded() && settings.UseReputation)
			return player.Expansion_GetPersonalStorageLevel() == m_Expansion_PersonalStorageUpgradeLevel - 1;
	#endif

		return false;
	}

	void Expansion_UpgradePersonalStorage(PlayerBase player)
	{
		if (!player.GetIdentity())
			return;

	#ifdef EXPANSIONMODHARDLINE
		if (Expansion_CanUpgradePersonalStorage(player))
		{
			player.Expansion_SetPersonalStorageLevel(m_Expansion_PersonalStorageUpgradeLevel);
			Delete();
		}
	#endif
	}
}
