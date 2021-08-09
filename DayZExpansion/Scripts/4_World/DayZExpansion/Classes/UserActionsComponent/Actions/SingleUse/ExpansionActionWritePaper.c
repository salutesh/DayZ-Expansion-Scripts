/**
 * ActionOpenBook.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionWritePaperCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT);
	}
}

class ExpansionActionWritePaper: ActionContinuousBase
{
	const float TARGET_DISTANCE = 2;
	
	void ExpansionActionWritePaper()
	{
		m_CallbackClass = ActionWritePaperCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()
	{	
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined(TARGET_DISTANCE);
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ((item.ConfigIsExisting("writingColor") && target.GetObject().GetType() == "Paper") || (target.GetObject().ConfigIsExisting("writingColor") && item.GetType() == "Paper"))
		{
			return true;
		}
		return false;
	}
	
	override void OnStartClient( ActionData action_data )
	{
		WriteNote( action_data );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		WriteNote( action_data );
	}

	override string GetText()
	{
		return "#write_note";
	}

	override void OnFinishProgressClient( ActionData action_data )
	{
		//which is pen and which paper
		//if (action_data.m_Target.GetObject().ConfigIsExisting("writingColor"))
		//{
		//	action_data.m_Player.m_writingImplement = EntityAI.Cast(action_data.m_Target.GetObject());
		//	action_data.m_Player.m_paper = action_data.m_MainItem;
		//}
		//else
		//{
		//	action_data.m_Player.m_writingImplement = action_data.m_MainItem;
		//	action_data.m_Player.m_paper = EntityAI.Cast(action_data.m_Target.GetObject());
		//}
		//action_data.m_Player.enterNoteMenuWrite = true;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	void WriteNote( ActionData action_data )
	{
		//Pen_ColorBase pen = Pen_ColorBase.Cast(action_data.m_Target.GetObject());
		//Paper paper = Paper.Cast( action_data.m_MainItem );
		//paper.SetNote();
	}
}