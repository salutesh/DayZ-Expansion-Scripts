/**
 * HandActionThrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*modded class HandActionThrow
{
	static ref ScriptInvoker OnThrow = new ScriptInvoker();
	
	// ------------------------------------------------------------
	// Override Action
	// ------------------------------------------------------------	
	override void Action( HandEventBase e )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("HandActionThrow::Action - Start");
		#endif
		
		hndDebugPrint( "[hndfsm] " + Object.GetDebugName( e.m_Player ) + " STS=" + e.m_Player.GetSimulationTimeStamp() + " action=throw" );
		HandEventThrow throwEvent = HandEventThrow.Cast( e );

		OnThrow.Invoke( e.GetSrcEntity(), e.m_Player, throwEvent.GetForce() );

		GameInventory.LocationSyncMoveEntity( e.GetSrc(), e.GetDst() );

		DayZPlayer player = DayZPlayer.Cast( e.m_Player );
		if ( player.GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_REMOTE )
		{
			InventoryItem item = InventoryItem.Cast( e.GetSrcEntity() );
			if ( item && !item.IsKindOf( "ExpansionChicken" ) )
			{
				item.ThrowPhysically( player, throwEvent.GetForce() );
			}
		}

		player.OnItemInHandsChanged();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("HandActionThrow::Action - End");
		#endif
	}
}*/
