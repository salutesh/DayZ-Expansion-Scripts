/**
 * ExpansionActionDestroyBarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDestroyBarbedWire : ExpansionActionDestroyBase
{
	string m_SlotName;

	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Time = GetExpansionSettings().GetRaid().BarbedWireRaidToolTimeSeconds;
		m_Cycles = GetExpansionSettings().GetRaid().BarbedWireRaidToolCycles;
		m_MinHealth01 = 0.01;  //! 1% health
		m_ToolDamagePercent = GetExpansionSettings().GetRaid().BarbedWireRaidToolDamagePercent;
	}

	override bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		if ( !super.DestroyCondition( player, target, item, camera_check ) )
			return false;

		if ( GetExpansionSettings().GetRaid().BarbedWireRaidTools.Find( item.GetType() ) == -1 )
			return false;

		Object targetObject = target.GetObject();

		if ( targetObject.IsInherited( ExpansionBarbedWire ) )
		{
			return true;
		} else
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
						
			string selection = targetObject.GetActionComponentName( target.GetComponentIndex() );
			
			if ( selection.Length() > 0 )
			{
				int delimiter_index = selection.IndexOfFrom( 0, "_mounted" );
				if ( delimiter_index > -1 )
				{
					selection = selection.Substring( 0, delimiter_index );
					
					BarbedWire barbed_wire = BarbedWire.Cast( base_building.FindAttachmentBySlotName( selection ) );
					if ( barbed_wire && barbed_wire.IsMounted() )
					{
						m_TargetName = barbed_wire.GetDisplayName();
						m_SlotName = selection;

						return true;
					}
				}
			}
		}

		return false;
	}

	override bool CanBeDestroyed( Object targetObject )
	{
		return GetExpansionSettings().GetRaid().CanRaidBarbedWire;
	}

	override Object GetActualTargetObject( Object targetObject )
	{
		if ( targetObject.IsInherited( ExpansionBarbedWire ) )
		{
			return targetObject;
		} else
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			return base_building.FindAttachmentBySlotName( m_SlotName );
		}
	}
}
