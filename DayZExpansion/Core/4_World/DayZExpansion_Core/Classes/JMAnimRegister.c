/**
 * JMAnimRegister.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMAnimRegister
{
	override void OnRegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterHeavy( pType, pBehavior );

		pType.AddItemInHandsProfileIK("ExpansionTemporaryOwnedContainer", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,			 		"dz/anims/anm/player/ik/heavy/wooden_crate.anm");
	}
}
