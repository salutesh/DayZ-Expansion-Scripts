/**
 * JMAnimRegister.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterOneHanded( pType, pBehavior );

		pType.AddItemInHandsProfileIK("ExpansionMoneyNugget_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior,	"dz/anims/anm/player/ik/gear/SmallStone.anm");
		pType.AddItemInHandsProfileIK("ExpansionMoneyBar_Base", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_m4_carryhandle.anm");
		pType.AddItemInHandsProfileIK("ExpansionMoneyBanknote_Base", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, 	"dz/anims/anm/player/ik/attachments/magazines/magazine_cz527.anm");
	}
}
