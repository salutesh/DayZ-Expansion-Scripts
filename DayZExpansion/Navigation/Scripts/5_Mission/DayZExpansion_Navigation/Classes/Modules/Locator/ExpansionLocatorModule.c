/**
 * ExpansionLocatorModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLocatorModule
 * @brief		This class handle expansion locator system
 **/
[CF_RegisterModule(ExpansionLocatorModule)]
class ExpansionLocatorModule: CF_ModuleGame
{
	protected autoptr array<ref ExpansionLocatorArray> m_AreaArray;
	protected string m_CurrentAreaName;
	float m_Time;
	
	override void OnInit()
	{
		super.OnInit();

		EnableMissionLoaded();
		EnableUpdate();
	}

	// ------------------------------------------------------------
	// ExpansionLocatorModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		m_CurrentAreaName = "";

		if ( !m_AreaArray )
		{
			m_AreaArray = ExpansionLocatorStatic.GetWorldLocations();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule IsServer
	// ------------------------------------------------------------
	override bool IsServer()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		auto update = CF_EventUpdateArgs.Cast(args);

		m_Time += update.DeltaTime;

		if ( m_Time > 5 )
		{
			m_Time = 0;
			
			if ( !GetGame() )
				return;

			if ( !GetExpansionSettings().GetMap(false).IsLoaded() )
				return;

			if ( !GetExpansionSettings().GetMap().PlayerLocationNotifier )
				return;

			CheckPlayer();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionLocatorModule CheckPlayer
	// ------------------------------------------------------------
	protected void CheckPlayer()
	{
		Mission mission = GetGame().GetMission();
		if ( !mission )
			return;

		IngameHud hud = IngameHud.Cast( mission.GetHud() );
		if ( !hud || !hud.GetExpanisonLocatorUI() )
			return;

		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
			return;
		
		ExpansionLocatorArray shortestLocation = null;
		float shortestDistanceSq = int.MAX;
		vector myPos = player.GetPosition();
		vector myPos2D = Vector( myPos[0], 0, myPos[2] );

		foreach (ExpansionLocatorArray loc: m_AreaArray)
		{
			float distanceSq = vector.DistanceSq( myPos2D, loc.position );

			if ( distanceSq <= shortestDistanceSq )
			{
				shortestDistanceSq = distanceSq;
				shortestLocation = loc;
			}
		}

		if ( shortestLocation )
		{
			float radius = ExpansionLocatorStatic.GetRadius( shortestLocation.type );
			if (shortestDistanceSq <= radius * radius )
			{
				if ( m_CurrentAreaName != shortestLocation.name )
				{
					m_CurrentAreaName = shortestLocation.name;					
					hud.GetExpanisonLocatorUI().OnShowCityClient( m_CurrentAreaName );
				}

				return;
			}
		}

		m_CurrentAreaName = "";
	}
}
