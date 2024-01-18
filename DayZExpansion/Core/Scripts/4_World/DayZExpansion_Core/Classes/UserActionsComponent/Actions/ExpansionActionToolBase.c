/**
 * ExpansionActionToolBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*! @brief Provides base classes for tool-based continuous actions with animation fitting the tool.
*/

class ExpansionActionToolBaseCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		ExpansionActionToolBase action = ExpansionActionToolBase.Cast( m_ActionData.m_Action );

		int time;
		int cycles;

		if ( action )
		{
			time = action.m_Time;
			cycles = action.m_Cycles;
		}

		if ( !time )
			time = 15 * 60;  //! 15 minutes

		if ( !cycles )
			cycles = 5;

		m_ActionData.m_ActionComponent = new CAContinuousRepeat( time / cycles );
	}
}

class ExpansionActionToolBase : ActionContinuousBase
{
	string m_TargetName;

	int m_Cycles;

	float m_Time;
	float m_ToolDamagePercent;  //! If tool damage is 100% and the tool does not consume energy, it WILL be ruined no matter what when the action finishes
	float m_MinHealth01;  //! Minimum health [0.0..1.0] at which target is considered destroyed, negative value = ignore

	void ExpansionActionToolBase()
	{
		m_CallbackClass = ExpansionActionToolBaseCB;
	}

	bool SetupCondition()
	{
		return true;
	}

	void Setup( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Time = 15 * 60;  //! 15 minutes
		m_Cycles = 1;
		m_ToolDamagePercent = 10;
		m_MinHealth01 = -1;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !item || !target.GetParentOrObject() )
			return false;

		if (!SetupCondition())
			return false;

		Setup( player, target, item );

		if ( m_Time <= 0 )
			return false;

		if ( item.GetCompEM() && !item.GetCompEM().IsWorking() )
			return false;

		m_TargetName = target.GetParentOrObject().GetDisplayName();

		return true;
	}

	override void CreateAndSetupActionCallback( ActionData action_data )
	{
		ItemBase item = action_data.m_MainItem;

		if ( item.GetCompEM() )
		{
			//! Powered items
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL;
		} else
		{
			string animClass;
			string suicideAnim;

			if ( item.ConfigIsExisting( "animClass" ) )
				animClass = item.ConfigGetString( "animClass" );

			if ( item.ConfigIsExisting( "suicideAnim" ) )
				suicideAnim = item.ConfigGetString( "suicideAnim" );

			if ( ( suicideAnim != "" || item.KindOf( "Axe" ) || item.IsKindOf( "Hammer" ) || item.KindOf( "Knife" ) || item.KindOf( "Pickaxe" ) || item.KindOf( "SledgeHammer" ) ) && animClass != "Knife" && suicideAnim != "onehanded" )
			{
				//! Tools & melee weapons (excluding knifes)
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
			} else
			{
				//! All other items
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
			}
		}

		super.CreateAndSetupActionCallback( action_data );
	}

	override void OnStartAnimationLoopServer( ActionData action_data )
	{
		super.OnStartAnimationLoopServer( action_data );

		ItemBase item = action_data.m_MainItem;
		if ( item.GetCompEM() && item.HasQuantity() )
		{
			//! Convert tool dmg pct to energy usage
			//! (less ~10% so fully charged/filled tool will last for the configured number of raid cycles)
			float energyMax = item.GetCompEM().GetEnergyMaxPristine();
			item.GetCompEM().SetEnergyUsage( energyMax * m_ToolDamagePercent / 110.0 / m_Time );
		}
	}

	override void OnEndAnimationLoopServer( ActionData action_data )
	{
		super.OnEndAnimationLoopServer( action_data );

		ItemBase item = action_data.m_MainItem;
		if ( item.GetCompEM() && item.HasQuantity() )
		{
			item.GetCompEM().ResetEnergyUsage();
		}
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		Object actualTargetObject = GetActualTargetObject( action_data.m_Target.GetParentOrObject() );

		if ( !actualTargetObject )
			return;

		if ( !action_data.m_MainItem.GetCompEM() || action_data.m_MainItem.HasQuantity() )
		{
			if ( !action_data.m_MainItem.HasQuantity() )
			{
				//! Damage tool if it does not have an energy manager or quantity

				float maxHealth = actualTargetObject.GetMaxHealth( "", "Health" );
				float minHealth = Math.Floor( maxHealth * m_MinHealth01 );
		
				float toolDamage = action_data.m_MainItem.GetMaxHealth() * m_ToolDamagePercent / 100.0;
				//! If it's the last cycle and tool damage is set to 100%, the tool will be ruined no matter what
				if ( actualTargetObject.GetHealth() > minHealth || m_ToolDamagePercent < 100.0 )
					toolDamage /= m_Cycles;
				action_data.m_MainItem.DecreaseHealth( toolDamage, false );
			} else
			{
				//! Deplete tool by converting tool dmg to quantity
				float quantity = action_data.m_MainItem.GetQuantityMax() * m_ToolDamagePercent / 100.0;
				//! If it's the last cycle and tool damage is set to 100%, the tool will be fully depleted no matter what
				if ( actualTargetObject.GetHealth() > minHealth || m_ToolDamagePercent < 100.0 )
					quantity /= m_Cycles;
				action_data.m_MainItem.SetQuantity( action_data.m_MainItem.GetQuantity() - quantity );
			}
		}

		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight / m_Cycles );
	}

	//! Just kept for compatibility with 3rd party mods
	ItemBase GetTargetItem( Object targetObject )
	{
		Error("DEPRECATED - please use ExpansionActionToolBase::GetActualTagetObject instead");
		return ItemBase.Cast( GetActualTargetObject( targetObject ) );
	}

	//! Override this in derived classes to return a different object than targetObject
	Object GetActualTargetObject( Object targetObject )
	{
		return targetObject;
	}
}
