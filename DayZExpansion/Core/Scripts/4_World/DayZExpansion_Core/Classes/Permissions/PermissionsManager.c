/**
 * ExpansionPermissionsManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef JM_COT
class ExpansionPermissionsManager
{
	bool m_AdminToolsToggledOn;

	void ExpansionPermissionsManager()
	{
	}

	void ~ExpansionPermissionsManager()
	{
	}

	void RegisterPermission( string permission )
	{
	}

	bool HasPermission( string permission )
	{
		if ( !g_Game.IsDedicatedServer() )
		{
			Man player = g_Game.GetPlayer();
			if (player && player.GetIdentity() )
				return HasPermission( permission, player.GetIdentity() );
		}

		return false;
	}

	bool HasPermission( string permission, PlayerIdentity ihp )
	{
		switch ( permission )
		{
			case "Admin.Chat":
			case "Entity.Spawn":
			case "Expansion.Territories.Edit":
			case "Expansion.DayZCreature.Lobotomize":
				#ifdef VPPADMINTOOLS
				if (!g_Game.IsDedicatedServer())
					return IsAdminToolsToggledOn();
				PermissionManager pm = GetPermissionManager();
				if (!pm)
					return false;
				//! I'm lazy and so SuperAdmins as well as anyone in group Admins will get blanket permission
				if (pm.IsSuperAdmin(ihp.GetPlainId()))
					return true;
				UserGroup group = pm.GetUserGroup(ihp.GetPlainId());
				if (group && group.GetGroupName() == "Admins")
					return true;
				#endif
				break;
		}

		return false;
	}

	bool IsAdminToolsToggledOn()
	{
		#ifdef VPPADMINTOOLS
		MissionBaseWorld mission = MissionBaseWorld.Cast(g_Game.GetMission());
		return mission && mission.VPPAT_AdminToolsToggled();
		#endif

		return false;
	}
}

ref ExpansionPermissionsManager g_ExpansionPermissionsManager;

ExpansionPermissionsManager GetPermissionsManager()
{
	if ( !g_ExpansionPermissionsManager )
	{
		g_ExpansionPermissionsManager = new ExpansionPermissionsManager();
	}

	Assert_Null( g_ExpansionPermissionsManager );

	return g_ExpansionPermissionsManager;
}
#endif

#ifdef JM_COT
modded class JMPermissionManager
{
	bool IsAdminToolsToggledOn()
	{
		return GetCommunityOnlineToolsBase().IsActive();
	}

	void Expansion_RegisterPermission( string permission, JMPermissionType permType )
	{
		#ifdef JM_COT_DIAG_LOGGING
		auto trace = CF_Trace_1(this, "RegisterPermission").Add(permission);
		#endif

		Assert_Null( RootPermission );

		if (m_MissionLoaded)
			Error("Cannot register new permissions once mission is loaded!");
		else
			RootPermission.AddPermission( permission, permType, false );
	}
}
#endif
