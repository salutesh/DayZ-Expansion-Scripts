class ExpansionActionDismantleFlagCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 30 );
	}
}

class ExpansionActionDismantleFlag: ActionContinuousBase
{
	void ExpansionActionDismantleFlag()
	{
		m_CallbackClass = ExpansionActionDismantleFlagCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}
	
	override string GetText()
	{
		return "#fold";
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		bool canDismantle = GetExpansionSettings().GetBaseBuilding().CanDismantleFlag;

		if ( targetObject )
		{
			ExpansionFlagBase flag;
			if ( Class.CastTo( flag, targetObject ) )
			{
				return canDismantle;
			}
		}
		
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		EntityAI flag = EntityAI.Cast(action_data.m_Target.GetObject() );

		if ( GetExpansionSettings().GetBaseBuilding().DestroyFlagOnDismantle )
			GetGame().CreateObject("ExpansionFlagKitExpansion", flag.GetPosition() );

		flag.Delete();
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " folded " + action_data.m_Target.GetObject().GetDisplayName();
	}
}