/**
 * ExpansionActionDebugLobotomize.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDebugLobotomizeCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1.0);
	}
};

class ExpansionActionDebugLobotomize: ActionContinuousBase
{
	static const ref TTypenameArray EXPANSION_LOBOMTOMY_TOOLS = {Mosin_Bayonet, Screwdriver};

	bool m_Expansion_IsLobotomized;  //! Client only

	void ExpansionActionDebugLobotomize()
	{
		m_CallbackClass = ExpansionActionDebugLobotomizeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "[ADMIN] Lobotomize";
	}

	override string GetText()
	{
		if (m_Expansion_IsLobotomized)
			return "[ADMIN] Unlobotomize";

		return m_Text;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTObject;
		m_ConditionItem = new CCINone;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasTarget()
	{
		return true;
	}

	override bool HasProgress()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return Expansion_ActionCondition(player, target, item);
	}

	override protected bool ActionConditionContinue( ActionData action_data )
	{
		return Expansion_ActionCondition(action_data.m_Player, action_data.m_Target, action_data.m_MainItem, true);
	}

	bool Expansion_ActionCondition(PlayerBase player, ActionTarget target, ItemBase item, bool isContinue = false)
	{
		Object targetObject = target.GetObject();

		if (!targetObject.IsDayZCreature() || !targetObject.IsAlive())
			return false;

		auto permissionsManager = GetPermissionsManager();
	#ifdef SERVER
		if (!permissionsManager.HasPermission("Expansion.Creature.Lobotomize", player.GetIdentity()))
	#else
		if (!GetPermissionsManager().IsAdminToolsToggledOn() || !permissionsManager.HasPermission("Expansion.Creature.Lobotomize"))
	#endif
			return false;

		AnimalBase animal;
		ZombieBase zombie;

		bool isLobotomized;

		if (Class.CastTo(animal, targetObject))
			isLobotomized = animal.Expansion_IsLobotomized();
		else if (Class.CastTo(zombie, targetObject))
			isLobotomized = zombie.Expansion_IsLobotomized();
		else
			return false;

		if (!isLobotomized && (!item || item.IsDamageDestroyed() || !ExpansionStatic.IsAnyOf(item, EXPANSION_LOBOMTOMY_TOOLS)))
			return false;

	#ifdef SERVER
		if (!isContinue)
		{
			if (animal && animal.m_Expansion_LobotomyInProgress)
				return false;
			else if (zombie && zombie.m_Expansion_LobotomyInProgress)
				return false;
		}
	#else
		m_Expansion_IsLobotomized = isLobotomized;
	#endif

		return true;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
	#endif

		auto creature = DayZCreatureAI.Cast(action_data.m_Target.GetObject());

		bool isLobotomized;

		AnimalBase animal;
		ZombieBase zombie;
		if (Class.CastTo(animal, creature))
			isLobotomized = animal.Expansion_IsLobotomized();
		else if (Class.CastTo(zombie, creature))
			isLobotomized = zombie.Expansion_IsLobotomized();

		if (!isLobotomized)
			creature.ProcessDirectDamage(DT_FIRE_ARM, action_data.m_Player, "", "Bullet_Expansion_LobotomyDart", "0 0 0", 1.0);
		else
			GetDayZGame().GetExpansionGame().Lobotomize(creature);
	}
}
