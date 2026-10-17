/**
 * Construction.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ConstructionBase
{
	override bool IsCollidingEx(CollisionCheckData check_data)
	{
		auto settings = GetExpansionSettings().GetBaseBuilding(false);
		if ( settings.IsLoaded() && settings.CanBuildAnywhere )
		{
			return false;
		}
		else
		{
			return super.IsCollidingEx(check_data);
		}
	}

	void Expansion_AdminBuildPartServer( notnull Man player, string part_name, int action_id )
	{
		//! Like vanilla BuildPartServer but calls Expansion_AdminLockLockableSlots instead of TakeMaterialsServer

		bsbDebugPrint("[bsb] Construction Expansion_AdminBuildPartServer | " + part_name);
		//reset DamageZone health
		string damage_zone;
		if (DamageSystem.GetDamageZoneFromComponentName(m_EntityParent,part_name,damage_zone))
		{
			m_EntityParent.SetAllowDamage(true);
			m_EntityParent.SetHealthMax(damage_zone);
			m_EntityParent.ProcessInvulnerabilityCheck(m_EntityParent.GetInvulnerabilityTypeString());
		}
		
		//on action
		Expansion_AdminLockLockableSlots( part_name );

		//call event
		BaseBuildingBase bb;
		if (Class.CastTo(bb, m_EntityParent))
			bb.OnPartBuiltServer( player, part_name, action_id );
	}

	void AdminLockLockableSlots( string part_name, bool repairing = false )
	{
		Error("DEPRECATED, use Expansion_AdminBuildPartServer");
		Expansion_AdminLockLockableSlots( part_name, repairing );
	}

	void Expansion_AdminLockLockableSlots( string part_name, bool repairing = false )
	{
		//! Like vanilla TakeMaterialsServer but does not touch materials and only locks lockable slots

		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + m_EntityParent.GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "Materials";
		
		if ( g_Game.ConfigIsExisting( cfg_path ) )
		{
			int	child_count = g_Game.ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				g_Game.ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, lockable from material
				string config_path;
				string slot_name;
				config_path = cfg_path + " " + child_name + " " + "slot_name";
				g_Game.ConfigGetText( config_path, slot_name );
				config_path = cfg_path + " " + child_name + " " + "lockable";
				bool lockable = g_Game.ConfigGetInt( config_path );
				
				ItemBase attachment = ItemBase.Cast( m_EntityParent.FindAttachmentBySlotName( slot_name ) );
				if ( attachment && lockable )
				{
					//lock attachment
					InventoryLocation inventory_location = new InventoryLocation;
					attachment.GetInventory().GetCurrentInventoryLocation( inventory_location );
			
					m_EntityParent.GetInventory().SetSlotLock( inventory_location.GetSlot(), true );
				}
			}
		}
	}
	
	override protected void UpdateConstructionParts()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "UpdateConstructionParts");
#endif

		super.UpdateConstructionParts();

		string construction_path = "cfgVehicles" + " " + m_EntityParent.GetType() + " " + "Construction";

		if ( g_Game.ConfigIsExisting( construction_path ) )
		{
			//main parts
			for ( int i = 0; i < g_Game.ConfigGetChildrenCount( construction_path ); ++i )
			{
				string main_part_name;
				g_Game.ConfigGetChildName( construction_path, i, main_part_name );
				string part_path = construction_path + " " + main_part_name;
				
				//parts
				for ( int j = 0; j < g_Game.ConfigGetChildrenCount( part_path ); ++j )
				{
					string part_name;
					g_Game.ConfigGetChildName( part_path, j, part_name );

					ConstructionPart part;
					if (  m_ConstructionParts.Find( part_name, part ) )
					{
						string snapping_show_path = part_path + " " + part_name + " snapping_show";
						if ( g_Game.ConfigIsExisting( snapping_show_path ) )
						{
							g_Game.ConfigGetIntArray( snapping_show_path, part.m_SnappingShow );
						}
						
						part.m_Order = g_Game.ConfigGetInt( part_path + " " + part_name + " order" );
						
						CF_Log.Debug("Construction::UpdateConstructionParts " + part.GetName() + " part.m_Order : " + part.m_Order);
					}
				}
			}
		}
	}

	bool IsPartBuiltForSnapPoint( int idx )
	{
		foreach (string key, ConstructionPart value: m_ConstructionParts)
		{
			if ( value.IsBuilt() && value.m_SnappingShow.Find( idx ) > -1 )
				return true;
		}

		return false;
	}
	
	//Get all construction parts that can be build (at that current time)
	override void GetConstructionPartsToBuild( string main_part_name, out array<ConstructionPart> construction_parts, ItemBase tool, out string real_constructionTarget, bool use_tool )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "GetConstructionPartsToBuild");
#endif

		super.GetConstructionPartsToBuild( main_part_name, construction_parts, tool, real_constructionTarget, use_tool );
		
		//Descending order
		for ( int j = 0; j < construction_parts.Count(); ++j )
		{
			for ( int k = 0; k < construction_parts.Count(); ++k )
			{
				if ( construction_parts[k].m_Order < construction_parts[j].m_Order )
				{
					construction_parts.SwapItems(j,k);
				}  
			}
		}
	}

	override void DismantlePartServerEx( Man player, string part_name, int action_id )
	{
		TerritoryFlag flag;
		if ( Class.CastTo(flag, m_EntityParent)  )
		{
			string damage_zone;
			DamageSystem.GetDamageZoneFromComponentName( flag, part_name, damage_zone );
			
			bsbDebugPrint("[bsb] Construction DismantlePartServer | " + part_name);

			//! If we didn't spend any, we don't get any
			if ( !GetExpansionSettings().GetBaseBuilding().SimpleTerritory )
			{
				//receive materials
				if (player)
					ReceiveMaterialsServer( player, part_name, damage_zone );
					
				//drop non-usable materials
				DropNonUsableMaterialsServer( player, part_name );
			}

			DropEssentialAttachments(player, part_name);
		
			if (!CfgGameplayHandler.GetDisableItemFalling())
				HandleEntityFalling(part_name);
			
			//call event
			flag.OnPartDismantledServer( player, part_name, action_id );
		}
		else
		{
			super.DismantlePartServerEx( player, part_name, action_id );
		}
	}
};