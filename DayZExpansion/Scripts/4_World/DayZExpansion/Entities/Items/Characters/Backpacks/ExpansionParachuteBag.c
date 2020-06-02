/**
 * ExpansionParachuteBag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionParachuteBag extends Clothing
{
	bool CanDeployParachute()
	{
		ExpansionParachute parachute;
		if ( !Class.CastTo( parachute, FindAttachmentBySlotName( "expansionparachute" ) ) )
			return false;

		DayZPlayer player;
		if ( !Class.CastTo( player, GetHierarchyRootPlayer() ) )
			return false;

		return true;
	}

	ExpansionParachute DeployParachute()
	{
		ExpansionParachute parachute;
		if ( !Class.CastTo( parachute, FindAttachmentBySlotName( "expansionparachute" ) ) )
			return NULL;

		DayZPlayerImplement player;
		if ( !Class.CastTo( player, GetHierarchyRootPlayer() ) )
			return NULL;

		InventoryLocation src = new InventoryLocation;
		InventoryLocation dst = new InventoryLocation;

		parachute.GetInventory().GetCurrentInventoryLocation( src );

		vector transform[4];
		player.GetTransformWS( transform );

		transform[3] = transform[3] + parachute.GetPlayerOffset();

		dst.SetGround( parachute, transform );

		parachute.SetDeployed( this, player );

		if ( !GameInventory.LocationSyncMoveEntity( src, dst ) )
			return NULL;

		return parachute;
	}
}