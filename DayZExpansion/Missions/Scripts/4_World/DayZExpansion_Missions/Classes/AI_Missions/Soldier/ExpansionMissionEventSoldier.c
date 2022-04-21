/**
 * ExpansionMissionEventSoldier.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionMissionEventSoldier extends ExpansionMissionEventBase
{	
	autoptr array< ref ExpansionMissionEventSoldierObject > Objects;
	autoptr array< ref ExpansionMissionEventSoldierLoot > Loot;

	[NonSerialized()]
	ExpansionMissionEventSoldierContainer Container;

	[NonSerialized()]
	private autoptr array< Object > m_MissionObjects;

	[NonSerialized()]
	private autoptr array< eAIBase > m_MissionAI;

	[NonSerialized()]
	private ItemBase m_Container;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventSoldier
	// ------------------------------------------------------------
	void ExpansionMissionEventSoldier()
	{
		m_EventName = "Soldiers";
	}
	
	// handle mission start
	override void Event_OnStart()
	{
		m_Container = ItemBase.Cast( GetGame().CreateObject( Container.ContainerName, Container.Position, false, true ) );
		m_Container.SetOrientation(Container.Orientation);
		
		if ( !m_Container )
			End();

		m_Container.Open();
	}

	// handle mission cleanup
	override void Event_OnEnd()
	{
	}

	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
	}

	protected override void OnLoadMission()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoadMission");
#endif

		JsonFileLoader<ExpansionMissionEventSoldier>.JsonLoadFile( m_FileName, this );
	}

	protected override void OnSaveMission()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventSoldier::OnSaveMission");
		#endif

		JsonFileLoader<ExpansionMissionEventSoldier>.JsonSaveFile( m_FileName, this );
	}

	override int MaxDefaultMissions()
	{
		return 1;
	}

	protected override string OnDefaultMission( int index )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventSoldier::OnDefaultMission");
		#endif
		
		Enabled = false;
		
		switch ( index )
		{
		default:
		case 0:
			Weight = 5;
			MissionName = "Vybor";
			break;
		}

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
	}
}
#endif