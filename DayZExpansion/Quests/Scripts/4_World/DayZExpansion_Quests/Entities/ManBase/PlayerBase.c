/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	// ------------------------------------------------------------
	// PlayerBase SetActions
	// ------------------------------------------------------------
	override void SetActions( out TInputActionMap InputActionMap )
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionOpenQuestMenu, InputActionMap);
	}
	
	// ------------------------------------------------------------
	// PlayerBase EEKilled
	// ------------------------------------------------------------
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		EntityAI killSource = EntityAI.Cast(killer);

		ExpansionQuestModule questModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
		if (!questModule)
			return;
		
		PlayerBase player;
		EntityAI target;

		if (killSource.GetHierarchyParent())
		{
			Object parent = killSource.GetHierarchyParent();
			if (parent.IsMan() || parent.IsKindOf("PlayerBase") || parent.IsInherited(PlayerBase))
			{
				player = PlayerBase.Cast(killSource.GetHierarchyParent());
				target = EntityAI.Cast(this);
				if (player && target)
				{
					Print(ToString() + "::EEKilled - Target: " + target);
					Print(ToString() + "::EEKilled - Kill source: " + killSource);
					Print(ToString() + "::EEKilled - Killer player UID: " + player.GetIdentity().GetId());
					questModule.OnEntityKilled(target, killSource, player.GetIdentity().GetId());
				}
			}
		}
		else if (killSource.IsMan() || killSource.IsKindOf("PlayerBase") || killSource.IsInherited(PlayerBase))
		{
			player = PlayerBase.Cast(killSource);
			target = EntityAI.Cast(this);
			if (player && target)
			{
				Print(ToString() + "::EEKilled - Target: " + target);
				Print(ToString() + "::EEKilled - Kill source: " + killSource);
				Print(ToString() + "::EEKilled - Killer player UID: " + player.GetIdentity().GetId());
				questModule.OnEntityKilled(target, killSource, player.GetIdentity().GetId());
			}
		}
	}
};
