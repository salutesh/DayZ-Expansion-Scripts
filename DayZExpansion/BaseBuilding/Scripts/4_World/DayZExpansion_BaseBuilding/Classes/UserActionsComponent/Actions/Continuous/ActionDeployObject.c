/**
 * ActionDeployObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlaceObjectActionReciveData
{
	int m_PlacingObject;

	Object m_PlacingParent;
}

modded class PlaceObjectActionData
{
	int m_PlacingObject;

	Object m_PlacingParent;
}

modded class ActionDeployObject
{
	static ref ExpansionHash s_Expansion_GardenPlot_Hash = new ExpansionHash("gardenplot");

	// ------------------------------------------------------------	
	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{	
		if ( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			PlaceObjectActionData poActionData;
			poActionData = PlaceObjectActionData.Cast( action_data );

			if ( !GetGame().IsDedicatedServer() )
			{
				Hologram hologram = poActionData.m_Player.GetHologramLocal();
				if ( hologram )
				{
					poActionData.m_PlacingObject = hologram.GetPlacingTypeChosen();
					poActionData.m_PlacingParent = hologram.GetPlacingParent();

					if ( poActionData.m_PlacingParent )
					{
						poActionData.m_Position = hologram.GetPlacingPositionMS();
						poActionData.m_Orientation = hologram.GetPlacingOrientationMS();

						SetLocalProjectionTransform( poActionData.m_PlacingParent, poActionData.m_Position, poActionData.m_Orientation, poActionData.m_Position, poActionData.m_Orientation );
					}

					poActionData.m_Player.SetLocalProjectionPosition( poActionData.m_Position );
					poActionData.m_Player.SetLocalProjectionOrientation( poActionData.m_Orientation );

					return true;
				}
			} else
			{
				return true;
			}
		}

		return false;
	}

	override string GetText()
	{
		string text = super.GetText();

		if (GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}

	// ------------------------------------------------------------	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;
		
		return Expansion_CheckDeploy(player, target, item, GetGame().IsDedicatedServer());
	}

	static bool Expansion_CheckDeploy(PlayerBase player, ActionTarget target, ItemBase item, bool notify = false)
	{
		if ( player.Expansion_IsInSafeZone() )
			return false;

		if ( !GetExpansionSettings().GetBaseBuilding(false).IsLoaded() || !GetExpansionSettings().GetTerritory(false).IsLoaded() )
			return false;

		bool isDisallowedOutsideBuildZone;
		bool isDisallowedOutsideTerritory;
		bool isDisallowedInEnemyTerritory;
		bool isDisallowedNearEnemyTerritory;
		string title;
		string text;

		//! @note zones are not sent to client (intentional)
		if ( GetGame().IsServer() && GetExpansionSettings().GetBaseBuilding().Zones.Count() )
		{
			ExpansionBuildNoBuildZone zone = player.GetBuildNoBuildZone();

			if ( zone )
			{
				//! Player is inside zone

				bool isItemAllowed = !zone.IsWhitelist;

				foreach ( string item_name : zone.Items )
				{
					if ( ( item.CanMakeGardenplot() && item_name == "GardenPlot" ) || item.IsKindOf( item_name ) )
					{
						isItemAllowed = zone.IsWhitelist;
						break;
					}
				}

				if ( !isItemAllowed )
				{
					text = zone.CustomMessage;

					if ( GetExpansionSettings().GetBaseBuilding().ZonesAreNoBuildZones )
					{
						title = "STR_EXPANSION_NOBUILD_ZONE_TITLE";
						if ( !text )
							text = "STR_EXPANSION_NOBUILD_ZONE_ITEM_DISALLOWED";
					} else
					{
						title = "STR_EXPANSION_BUILD_ZONE_TITLE";
						if ( !text )
							text = "STR_EXPANSION_BUILD_ZONE_ITEM_DISALLOWED";
					}

					ExpansionNotification(new StringLocaliser(title, zone.Name), new StringLocaliser(text)).Error(player.GetIdentity());

					return false;
				}
			} else if ( !GetExpansionSettings().GetBaseBuilding().ZonesAreNoBuildZones )
			{
				//! Zones are "build" zones, so if player isn't inside one, disallow placing if not whitelisted
				isDisallowedOutsideBuildZone = true;
			}
		}

		if ( GetExpansionSettings().GetTerritory().EnableTerritories )
		{
			if ( GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory == true && !isDisallowedOutsideBuildZone )
			{
				//! Flag can be placed if outside any territory/perimeter or if inside own territory/perimeter
				//! Other items can be placed if not in enemy territory or if item is whitelisted

				if ( player.IsInTerritory() )
				{
					isDisallowedInEnemyTerritory = !CanDeployInTerritory( player, item );
				}
				else if ( item.IsInherited( TerritoryFlagKit ) && player.IsInPerimeter() && !player.IsInsideOwnPerimeter() )
				{
					isDisallowedNearEnemyTerritory = true;
				}
			}
			else
			{
				//! Flag can be placed if outside any territory/perimeter and is whitelisted, if inside enemy territory and is whitelisted, or if inside own territory/perimeter
				//! Other items can only be placed if item is whitelisted

				if ( player.IsInTerritory() )
				{
					isDisallowedInEnemyTerritory = !CanDeployInTerritory( player, item );
				}
				else if (GetGame().IsServer())
				{
					//! @note deployables are not sent to client (intentional)
					isDisallowedOutsideTerritory = !GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory;
					foreach (string deployable: GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory)
					{
						if ( ( item.CanMakeGardenplot() && deployable == "GardenPlot" ) || item.IsKindOf( deployable ) )
						{
							isDisallowedOutsideBuildZone = false;
							isDisallowedOutsideTerritory = false;
							if ( item.IsInherited( TerritoryFlagKit ) && player.IsInPerimeter() && !player.IsInsideOwnPerimeter() )
							{
								isDisallowedNearEnemyTerritory = true;
							}
							break;
						}
					}
				}
			}
		}
		
		if (isDisallowedOutsideTerritory)
		{
			title = "STR_EXPANSION_TERRITORY_TITLE";
			text = "STR_EXPANSION_TERRITORY_TERRITORY_REQUIRED";
		}
		else if (isDisallowedInEnemyTerritory)
		{
			title = "STR_EXPANSION_TERRITORY_TITLE";
			text = "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY";
		}
		else if (isDisallowedNearEnemyTerritory)
		{
			title = "STR_EXPANSION_TERRITORY_TITLE";
			text = "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY_NEAR";
		}
		else if (isDisallowedOutsideBuildZone)
		{
			title = "STR_EXPANSION_BUILD_ZONE_REQUIRED_TITLE";
			text = GetExpansionSettings().GetBaseBuilding().BuildZoneRequiredCustomMessage;
			if ( !text )
				text = "STR_EXPANSION_BUILD_ZONE_REQUIRED";
		}
		else
		{
			return true;
		}

		if (notify)
			ExpansionNotification(title, text).Error(player.GetIdentity());

		return false;
	}

	static bool CanDeployInTerritory( PlayerBase player, ItemBase item )
	{
		if (player.IsInsideOwnTerritory())
			return true;

		if (!item)
			return false;

		//! @note deployables are sent to client as hashes
		foreach (ExpansionHash hash: GetExpansionSettings().GetBaseBuilding().m_DeployableInEnemyTerritory_Hashes)
		{
			if ((item.CanMakeGardenplot() && hash.IsEqual(s_Expansion_GardenPlot_Hash)) || hash.ObjectIsKindOf(item))
				return true;
		}

		return false;
	}

	// ------------------------------------------------------------	
	void SetLocalProjectionTransform( Object parent, vector posms, vector orims, out vector posws, out vector oriws )
	{
		Transform snapTrans = Transform.GetObject( parent );
		Transform localTrans = Transform.YawPitchRoll( orims, posms );
		Transform worldTrans = new Transform();

		Math3D.MatrixMultiply4( snapTrans.data, localTrans.data, worldTrans.data );
		worldTrans.UpdateUnion();

		posws = worldTrans.GetOrigin();
		oriws = worldTrans.GetYawPitchRoll();
	}
	
	// ------------------------------------------------------------	
	override void OnStartServer( ActionData action_data )
	{
		PlaceObjectActionData poActionData;
		poActionData = PlaceObjectActionData.Cast( action_data );

		ItemBase entity_for_placing = action_data.m_MainItem;
		ExpansionKitBase kit = ExpansionKitBase.Cast( entity_for_placing );

		if ( kit && kit.IsInherited( ExpansionKitBase ) )
		{
			if ( GetGame().IsMultiplayer() )
			{
				if ( !poActionData )
					return;

				if ( kit )
				{
					kit.SetPlacingIndex( poActionData.m_PlacingObject );
				}

				if ( poActionData.m_PlacingParent )
				{
					SetLocalProjectionTransform( poActionData.m_PlacingParent, poActionData.m_Position, poActionData.m_Orientation, poActionData.m_Position, poActionData.m_Orientation );
				}

				poActionData.m_Player.SetLocalProjectionPosition( poActionData.m_Position );
				poActionData.m_Player.SetLocalProjectionOrientation( poActionData.m_Orientation );

				if ( entity_for_placing )
				{
					poActionData.m_Player.PlacingStartServer( entity_for_placing );
					if ( kit.GetPlacingTypeChosen() )
					{
						if ( action_data.m_Player.GetHologramServer() )
						{
							action_data.m_Player.GetHologramServer().SetPlacingTypeChosen( kit.GetPlacingTypeChosen() );
						}
					}
			
					GetGame().AddActionJuncture( action_data.m_Player, entity_for_placing, 10000 );
					entity_for_placing.SetIsBeingPlaced( true );
				}
			} else
			{
				//! Local singleplayer
				if ( kit && poActionData )
				{
					kit.SetPlacingIndex( poActionData.m_PlacingObject );
				}

				action_data.m_Player.PlacingStartServer( entity_for_placing );
				action_data.m_Player.GetHologramServer().SetPlacingTypeChosen( kit.GetPlacingTypeChosen() );
				action_data.m_Player.GetHologramLocal().SetUpdatePosition( false );
				entity_for_placing.SetIsBeingPlaced( true );
			}
		}
		else 
		{
			super.OnStartServer( action_data );
		}
	}

	// ------------------------------------------------------------	
	override void OnEndServer( ActionData action_data )
	{
		ItemBase entity_for_placing = action_data.m_MainItem;
		ExpansionKitBase kit = ExpansionKitBase.Cast( entity_for_placing );

		if ( kit && kit.IsInherited( ExpansionKitBase ) )
		{
			PlaceObjectActionData poActionData;		
			if ( !action_data.m_MainItem || !Class.CastTo( poActionData, action_data ) )
				return;

			if ( !poActionData.m_AlreadyPlaced )
			{
				GetGame().ClearJuncture( action_data.m_Player, entity_for_placing );
				action_data.m_MainItem.SetIsBeingPlaced( false );

				if ( GetGame().IsMultiplayer() )
				{
					action_data.m_Player.PlacingCancelServer();
					action_data.m_Player.ServerTakeEntityToHands( entity_for_placing );
					action_data.m_MainItem.SoundSynchRemoteReset();
				} else
				{
					//! Local singleplayer
					action_data.m_Player.PlacingCancelLocal();
					action_data.m_Player.PlacingCancelServer();
					action_data.m_Player.LocalTakeEntityToHands( entity_for_placing );
				}

				GetGame().ClearJuncture( action_data.m_Player, action_data.m_MainItem );
			} else
			{
				//TODO: make OnEND placement event and move there

				action_data.m_MainItem.SetIsDeploySound( false );
				action_data.m_MainItem.SetIsPlaceSound( false );
				action_data.m_MainItem.SoundSynchRemoteReset();
				if ( action_data.m_MainItem.IsKindOf( "FenceKit" ) || action_data.m_MainItem.IsKindOf( "WatchtowerKit" ) || action_data.m_MainItem.IsKindOf( "ExpansionKitBase" ) )
				{
					action_data.m_MainItem.Delete();
				} else
				{
					GetGame().ClearJuncture( action_data.m_Player, action_data.m_MainItem );
				}
			}
		}	
		else
		{
			super.OnEndServer( action_data );
		}
	}

	// ------------------------------------------------------------	
	override void MoveEntityToFinalPosition( ActionData action_data, vector position, vector orientation )
	{
		if ( action_data.m_MainItem.IsInherited( ExpansionKitBase ) )
			return;

		super.MoveEntityToFinalPosition( action_data, position, orientation );
	}

	// ------------------------------------------------------------	
	override void WriteToContext( ParamsWriteContext ctx, ActionData action_data )
	{
		super.WriteToContext( ctx, action_data );
		
		PlaceObjectActionData poActionData;		
		if ( !Class.CastTo( poActionData, action_data ) )
			return;

		ctx.Write( poActionData.m_PlacingObject );
		ctx.Write( poActionData.m_PlacingParent );
	}
	
	// ------------------------------------------------------------	
	override bool ReadFromContext( ParamsReadContext ctx, out ActionReciveData action_recive_data )
	{
		if ( !super.ReadFromContext( ctx, action_recive_data ) )
			return false;
		
		PlaceObjectActionReciveData poActionData;		
		if ( !Class.CastTo( poActionData, action_recive_data ) )
			return false;
			
		if ( !ctx.Read( poActionData.m_PlacingObject ) )
			return false;
		if ( !ctx.Read( poActionData.m_PlacingParent ) )
			return false;

		return true;
	}
	
	// ------------------------------------------------------------	
	override void HandleReciveData( ActionReciveData action_recive_data, ActionData action_data )
	{
		super.HandleReciveData( action_recive_data, action_data );

		PlaceObjectActionReciveData recive_data_po = PlaceObjectActionReciveData.Cast( action_recive_data );
		PlaceObjectActionData action_data_po = PlaceObjectActionData.Cast( action_data );

		action_data_po.m_PlacingObject = recive_data_po.m_PlacingObject;
	}
}