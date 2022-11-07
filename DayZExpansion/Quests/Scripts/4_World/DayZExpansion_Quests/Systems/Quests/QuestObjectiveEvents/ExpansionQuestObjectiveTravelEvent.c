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

#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		vector position = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

		if (markerName != string.Empty || position != vector.Zero)
			GetQuest().CreateClientMarker(position, markerName);
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
				ObjectivePrint(ToString() + "::OnUpdate - Complete!");
				SetCompleted(true);
				OnComplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}

	override bool OnEventStart(bool continues = false)
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif

		ObjectivePrint(ToString() + "::OnEventStart - End");

		return true;
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