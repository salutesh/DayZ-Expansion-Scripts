/**
 * ActionDeployObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

/*
modded class ActiondeployObjectCB
{
	override void CreateActionComponent()
	{
		super.CreateActionComponent();
		m_ActionData.m_ActionComponent = new CAContinuousTime( 0.1 );
	}
}
*/

modded class ActionDeployObject
{
	// ------------------------------------------------------------	
	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{	
		if ( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			PlaceObjectActionData poActionData;
			poActionData = PlaceObjectActionData.Cast( action_data );

			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
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

	// ------------------------------------------------------------	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;
		
		if ( ExpansionSafeZone_IsInside( player.GetPosition() ) )
			return false;
			
		if ( GetExpansionSettings().GetBaseBuilding() )
		{		
			int i;
			float size;
			float Tsize = GetExpansionSettings().GetTerritory().TerritorySize;
			float Psize = GetExpansionSettings().GetTerritory().TerritoryPerimeterSize;
			
			size = Tsize;

			if (item.IsInherited(TerritoryFlagKit) )
				size += Psize;

			if ( GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory == true )
			{
				if ( player.IsInTerritory(size) )
				{
					if ( player.IsInTerritory(Tsize) )
						return !item.IsInherited(TerritoryFlagKit);

					if ( player.IsInsideOwnTerritory(size) )
					{
						return true;
					}
					else
					{
						for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i)
						{
							if ( item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i]) )
							{
								return true;
							}
						}
						
						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				//Place stuff other than flag should be possible inside your territory
				if ( player.IsInTerritory(size) )
				{
					if ( player.IsInsideOwnTerritory(size) )
					{
						return true;
					}
					else
					{
						for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i)
						{
							if ( item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i]) )
							{
								return true;
							}
						}
						
						return false;
					}
				}
				else
				{
					for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory.Count(); ++i)
					{
						if (item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory[i]) )
						{
							return true;
						}
					}
					
					return false;
				}
			}
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