/**
 * eAIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
modded class eAIBase
{
	protected bool m_Expansion_IsQuestVIP;

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableBool("m_Expansion_IsQuestVIP");
	}

	void Expansion_SetQuestVIP(bool state)
	{
		m_Expansion_IsQuestVIP = state;
		SetSynchDirty();
	}

	bool Expansion_IsQuestVIP()
	{
		return m_Expansion_IsQuestVIP;
	}
}
#endif
