/**
 * ExpansionAICampObjectiveSphereTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAICampObjectiveSphereTrigger: Trigger
{
	void ExpansionObjectiveTriggerBase()
	{
		SetEventMask(EntityEvent.ENTER);
	}

	void SetTriggerRadius(int radius)
	{
		SetCollisionSphere(radius);
	}

	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}

	//! Event called when an entity enters the trigger area.
	override void EOnEnter(IEntity other, int extra)
	{
		QuestPrint(ToString() + "::EOnEnter - Start");
		QuestPrint(ToString() + "::EOnEnter - Entity: " + other.Type());

		if (GetGame().IsClient())
			return;

		ZombieBase infected;
		if (!Class.CastTo(infected, other))
			return;

		QuestPrint(ToString() + "::EOnEnter - Delete infected: " + infected.GetType());
		GetGame().ObjectDelete(infected);
	}
	
	protected void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}
};
