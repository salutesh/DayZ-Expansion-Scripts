/**
 * ExpansionActionDestroyLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		bool useContainerRaidSettings;

		if ( targetObject.IsInherited( TentBase ) )
		{
			m_Time =  GetExpansionSettings().GetRaid().LockOnTentRaidToolTimeSeconds;
		}
		else if ( targetObject.IsInherited( ExpansionBaseBuilding ) )
		{
			m_Time = GetExpansionSettings().GetRaid().LockOnWallRaidToolTimeSeconds;
		}
		else if ( targetObject.IsInherited( BaseBuildingBase ) )
		{
			m_Time =  GetExpansionSettings().GetRaid().LockOnFenceRaidToolTimeSeconds;
		}
		else
		{
			m_Time = GetExpansionSettings().GetRaid().LockOnContainerRaidToolTimeSeconds;
			useContainerRaidSettings = true;
		}

		m_MinHealth01 = 0.01;  //! 1% health

		if ( !useContainerRaidSettings )
		{
			m_Cycles = GetExpansionSettings().GetRaid().LockRaidToolCycles;
			m_ToolDamagePercent = GetExpansionSettings().GetRaid().LockRaidToolDamagePercent;
		}
		else
		{
			m_Cycles = GetExpansionSettings().GetRaid().LockOnContainerRaidToolCycles;
			m_ToolDamagePercent = GetExpansionSettings().GetRaid().LockOnContainerRaidToolDamagePercent;
		}
	}

	override bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		if ( !super.DestroyCondition( player, target, item, camera_check ) )
			return false;

		Object targetObject = target.GetParentOrObject();

		ItemBase targetItem = ItemBase.Cast( targetObject );

		if ( !targetItem )
			return false;

		Object actualTargetObject = GetActualTargetObject( targetObject );

		if ( !actualTargetObject )
			return false;

		if ( !actualTargetObject.IsInherited( CombinationLock ) && !actualTargetObject.IsInherited( ExpansionCodeLock ) )
			return false;

		if ( !targetItem.ExpansionIsLocked() )
			return false;

		bool useContainerRaidSettings;

		if ( targetObject.IsInherited( TentBase ) )
		{
			m_Time =  GetExpansionSettings().GetRaid().LockOnTentRaidToolTimeSeconds;
		}
		else if ( targetObject.IsInherited( ExpansionBaseBuilding ) )
		{
			m_Time = GetExpansionSettings().GetRaid().LockOnWallRaidToolTimeSeconds;
		}
		else if ( targetObject.IsInherited( BaseBuildingBase ) )
		{
			m_Time =  GetExpansionSettings().GetRaid().LockOnFenceRaidToolTimeSeconds;
		}
		else
		{
			m_Time = GetExpansionSettings().GetRaid().LockOnContainerRaidToolTimeSeconds;
			useContainerRaidSettings = true;
		}

		TStringArray raidTools;
		if ( !useContainerRaidSettings )
			raidTools = GetExpansionSettings().GetRaid().LockRaidTools;
		else
			raidTools = GetExpansionSettings().GetRaid().LockOnContainerRaidTools;

		if ( !ExpansionStatic.IsAnyOf(item, raidTools) )
			return false;

		if ( targetItem.IsInherited( TentBase ) && IsMissionClient() )
		{
			targetObject = target.GetObject();  //! Only on client, NULL on server as we cannot use proxies here

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

		m_TargetName = actualTargetObject.GetDisplayName();

		return true;
	}

	override bool CanBeDestroyed( Object targetObject )
	{
		auto settings = GetExpansionSettings().GetRaid();

		bool raidableNow = settings.IsRaidableNow();

		if ( targetObject.IsInherited( Container_Base ) )
			return settings.CanRaidLocksOnContainers && (!settings.LockOnContainerRaidUseSchedule || raidableNow);
	
		if (!raidableNow)
			return false;

		if ( targetObject.IsInherited( TentBase ) )
			return settings.CanRaidLocksOnTents;

		if ( targetObject.IsInherited( Fence ) )
			return settings.CanRaidLocksOnFences;

		ExpansionWallBase wall;
		if (Class.CastTo(wall, targetObject) && (wall.HasDoor() || wall.HasGate()))
		{
			if ( wall.HasDoor() && settings.CanRaidLocksOnWalls == RaidLocksOnWallsEnum.OnlyDoor )
				return true;
			else if ( wall.HasGate() && settings.CanRaidLocksOnWalls == RaidLocksOnWallsEnum.OnlyGate )
				return true;
			return settings.CanRaidLocksOnWalls == RaidLocksOnWallsEnum.Enabled;
		}

		return super.CanBeDestroyed( targetObject );
	}

	override Object GetActualTargetObject( Object targetObject )
	{
		ItemBase targetItem = ItemBase.Cast( targetObject );

		if ( !targetItem || !targetItem.GetInventory() )
			return NULL;

		Object lockObj = targetItem.FindAttachmentBySlotName( "Att_CombinationLock" );
		if ( !lockObj )
			lockObj = targetItem.ExpansionGetCodeLock();

		return lockObj;
	}
}
