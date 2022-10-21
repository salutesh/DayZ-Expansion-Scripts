/**
 * ExpansionQuestObjectiveTravelEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTravelEvent: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer = 0;
	protected const float UPDATE_TICK_TIME = 2.0;

	override bool OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		if (!super.OnStart())
			return false;

		TravelEventStart();

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		if (!super.OnContinue())
			return false;

		TravelEventStart();

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		super.OnRecreateClientMarkers();

		if (GetQuest().GetQuestState() == ExpansionQuestState.STARTED)
		{
			vector position = GetObjectiveConfig().GetPosition();
			string markerName = GetObjectiveConfig().GetMarkerName();

			if (markerName != string.Empty || position != vector.Zero)
				GetQuest().CreateClientMarker(position, markerName);
		}
	}
#endif

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			if (DestinationCheck() && !IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				CF_Log.Debug(ToString() + "::OnUpdate - Complete!");
			#endif
				SetCompleted(true);
				OnComplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}

	protected void TravelEventStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "TravelEventStart");
	#endif

		vector position = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

	#ifdef EXPANSIONMODNAVIGATION
		if (markerName != string.Empty || position != vector.Zero)
			GetQuest().CreateClientMarker(position, markerName);
	#endif
	}

	vector GetPosition()
	{
		return GetObjectiveConfig().GetPosition();
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TRAVEL;
	}
};