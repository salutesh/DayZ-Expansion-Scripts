modded class MiscGameplayFunctions
{
	override static void TeleportCheck(notnull PlayerBase player, notnull array<ref array<float>> safe_positions)
	{
        if ( player.IsAI() )
            return;
            
		if( player.GetSimulationTimeStamp() < 20 && !player.IsPersistentFlag(PersistentFlag.AREA_PRESENCE) )
		{
			//simulation time is bellow a threshold, which means the player has recently connected,
			//the player does not have the AREA_PRESENCE flag set, which means they were not inside the area when they disconnected,
			//that means they just spawned into a contaminated area, lets move them somewhere safe
			vector player_pos = player.GetPosition();
			vector closest_safe_pos = MiscGameplayFunctions.GetClosestSafePos(player_pos, safe_positions);
			
			if (player_pos!=closest_safe_pos)
			{
				closest_safe_pos[1] = GetGame().SurfaceY(closest_safe_pos[0], closest_safe_pos[2]);
				
				player.SetPosition( closest_safe_pos );//...so lets teleport them somewhere safe
				//DeveloperTeleport.SetPlayerPosition(player, closest_safe_pos);
				GetGame().RPCSingleParam(player, ERPCs.RPC_WARNING_TELEPORT, null, true, player.GetIdentity());
			}
			
			player.SetPersistentFlag(PersistentFlag.AREA_PRESENCE, false);
		}
	}
};