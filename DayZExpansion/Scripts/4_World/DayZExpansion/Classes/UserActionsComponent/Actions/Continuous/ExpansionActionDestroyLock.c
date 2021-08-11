/**
 * ExpansionActionDestroyLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDestroyLock : ExpansionActionDestroyBase
{
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetParentOrObject();

		if ( targetObject.IsInherited( TentBase ) )
			m_Time =  GetExpansionSettings().GetRaid().LockOnTentRaidToolTimeSeconds;
		else if ( targetObject.IsInherited( Fence ) )
			m_Time =  GetExpansionSettings().GetRaid().LockOnFenceRaidToolTimeSeconds;
		else
			m_Time = GetExpansionSettings().GetRaid().LockOnWallRaidToolTimeSeconds;

		m_Cycles = GetExpansionSettings().GetRaid().LockRaidToolCycles;
		m_MinHealth01 = 0.01;  //! 1% health
		m_ToolDamagePercent = GetExpansionSettings().GetRaid().LockRaidToolDamagePercent;
	}

	override bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		if ( !super.DestroyCondition( player, target, item, camera_check ) )
			return false;

		if ( GetExpansionSettings().GetRaid().LockRaidTools.Find( item.GetType() ) == -1 )
			return false;

		ItemBase targetItem = ItemBase.Cast( target.GetParentOrObject() );

		if ( !targetItem )
			return false;

		if ( !targetItem.IsInherited( BaseBuildingBase ) && !targetItem.IsInherited( TentBase ) )
			return false;

		if ( !targetItem.IsLocked() )
			return false;

		if ( targetItem.IsInherited( TentBase ) && IsMissionClient() )
		{
			Object targetObject = target.GetObject();  //! Only on client, NULL on server as we cannot use proxies here

			if ( !targetObject )
				return false;

			//! Only allow interaction with entrances
			array< string > selections = new array< string >;
			targetObject.GetActionComponentNameList( target.GetComponentIndex(), selections );
			TentBase tent = TentBase.Cast( targetItem );

			bool canToggle;
			for ( int s = 0; s < selections.Count(); s++ )
			{
				if ( !selections[s].Contains( "entrance" ) && !selections[s].Contains( "door" ) )
					continue;

				if ( tent.CanToggleAnimations( selections[s] ) )
				{
					canToggle = true;
					break;
				}
			}

			if ( !canToggle )
				return false;
		} else
		{
			string selection = targetItem.GetActionComponentName( target.GetComponentIndex() );
			if ( !selection.Contains( "lock" ) && !targetItem.ExpansionIsOpenable( selection ) )
				return false;
		}

		ItemBase lockItem = GetTargetItem( target.GetParentOrObject() );

		if ( !lockItem )
			return false;

		if ( !lockItem.IsInherited( CombinationLock ) && !lockItem.IsInherited( ExpansionCodeLock ) )
			return false;

		m_TargetName = lockItem.GetDisplayName();

		return true;
	}

	override bool CanBeDestroyed( Object targetObject )
	{
		if ( targetObject.IsInherited( TentBase ) )
			return GetExpansionSettings().GetRaid().CanRaidLocksOnTents;

		if ( targetObject.IsInherited( Fence ) )
			return GetExpansionSettings().GetRaid().CanRaidLocksOnFences;

		ExpansionWallBase wall;
		if (Class.CastTo(wall, targetObject))
		{
			if ( wall.HasDoor() && GetExpansionSettings().GetRaid().CanRaidLocksOnWalls == RaidLocksOnWallsEnum.OnlyDoor )
				return true;
			else if ( wall.HasGate() && GetExpansionSettings().GetRaid().CanRaidLocksOnWalls == RaidLocksOnWallsEnum.OnlyGate )
				return true;
		}

		return GetExpansionSettings().GetRaid().CanRaidLocksOnWalls == RaidLocksOnWallsEnum.Enabled;
	}

	override ItemBase GetTargetItem( Object targetObject )
	{
		ItemBase targetItem = ItemBase.Cast( targetObject );

		if ( !targetItem || !targetItem.GetInventory() )
			return NULL;

		ItemBase lockItem = ItemBase.Cast( targetItem.FindAttachmentBySlotName( "Att_CombinationLock" ) );
		if ( !lockItem )
			lockItem = targetItem.ExpansionGetCodeLock();

		return lockItem;
	}
}
