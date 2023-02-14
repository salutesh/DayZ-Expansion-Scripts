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
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected bool m_DestinationReached;

#ifdef EXPANSIONMODNAVIGATION
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent OnRecreateClientMarkers
	// -----------------------------------------------------------
	override void OnRecreateClientMarkers()
	{
		vector position = m_ObjectiveConfig.GetPosition();
		string markerName = m_ObjectiveConfig.GetMarkerName();

		if (markerName != string.Empty && position != vector.Zero)
			m_Quest.CreateClientMarker(position, markerName);
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent OnEventStart
	// -----------------------------------------------------------
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

		if (!super.OnEventStart())
			return false;

	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_ObjectiveConfig.GetPosition());

		ObjectivePrint(ToString() + "::OnEventStart - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent OnContinue
	// -----------------------------------------------------------
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		if (!super.OnContinue())
			return false;

	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif
		
		if (!m_DestinationReached && !m_ObjectiveTrigger)
			CreateTrigger(m_ObjectiveConfig.GetPosition());
		
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent OnCleanup
	// -----------------------------------------------------------
	override bool OnCleanup()
	{
		if (!super.OnCleanup())
			return false;

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_DestinationReached: " + m_DestinationReached);

		bool conditionsResult = m_DestinationReached;
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent GetPosition
	// -----------------------------------------------------------
	vector GetPosition()
	{
		return m_ObjectiveConfig.GetPosition();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent SetReachedLocation
	// -----------------------------------------------------------
	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
		ObjectivePrint(ToString() + "::SetReachedLocation - Start");
		ObjectivePrint(ToString() + ":: SetReachedLocation - State: " + state);
		SetLocationState(state);
		m_Quest.QuestCompletionCheck();
		
		ObjectivePrint(ToString() + "::SetReachedLocation - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent SetLocationState
	// -----------------------------------------------------------
	void SetLocationState(bool state)
	{
		m_DestinationReached = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent GetLocationState
	// -----------------------------------------------------------
	bool GetLocationState()
	{
		return m_DestinationReached;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent CreateTrigger
	// -----------------------------------------------------------
	protected void CreateTrigger(vector pos)
	{
		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(this);
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent HasDynamicState
	// -----------------------------------------------------------
	override bool HasDynamicState()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TRAVEL;
	}
};