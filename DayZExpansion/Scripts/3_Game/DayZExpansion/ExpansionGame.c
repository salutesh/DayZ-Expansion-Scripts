/**
 * ExpansionGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionGame
{
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if ( rpc_type == ExpansionRPC.CreateNotification )
		{
			NotificationSystem.RPC_ExpansionCreateNotification( sender, target, ctx );

			return true;
		}

		return super.OnRPC( sender, target, rpc_type, ctx );
	}
}
