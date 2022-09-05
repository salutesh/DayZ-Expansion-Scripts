/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{
	// ------------------------------------------------------------
	// ZombieBase EEKilled
	// ------------------------------------------------------------
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		EntityAI killSource = EntityAI.Cast(killer);

		if (!killSource)
			return;

		ExpansionQuestModule questModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
		PlayerBase player;

		questModule.OnEntityKilled(this, killSource, killSource.GetHierarchyRootPlayer());
	}
};