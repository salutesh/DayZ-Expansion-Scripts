/**
 * ExpansionBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionBaseBuilding
{
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		EntityAI killerEntity = EntityAI.Cast(killer);
		if (!killerEntity)
			return;
		
		PlayerBase player;
		player = PlayerBase.Cast(killer);
		if (!player)
			player = PlayerBase.Cast(killerEntity.GetHierarchyRootPlayer());

		if (!player)
			return;

		ExpansionHardlineModule hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
		hardlineModule.OnPlayerAction(player, GetExpansionSettings().GetHardline().HumanityDestroyBaseBuilding);
	}
};