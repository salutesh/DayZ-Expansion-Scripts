/**
 * ExpansionActionPaint.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionPaintCB : ActionContinuousBaseCB
{	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 0.1 );
	}
};

class ExpansionActionPaint: ActionContinuousBase
{
	private string m_TargetName;
	private string m_TargetDisplayName;

	private ExpansionSkinModule m_SkinModule;

	void ExpansionActionPaint()
	{
		m_CallbackClass = ExpansionActionPaintCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;

		Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) );
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_PAINT " + m_TargetDisplayName;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !m_SkinModule )
			return false;

		ExpansionSpraycanBase spraycan;

		//! Print( player.GetItemInHands() );
		if ( !Class.CastTo( spraycan, player.GetItemInHands() ) )
			return false;

		m_TargetName = target.GetObject().GetType();

		string spraySkin;
		GetGame().ConfigGetText( "cfgVehicles " + spraycan.GetType() + " skinName", spraySkin );

		int skinIndex = m_SkinModule.GetSkinIndex( m_TargetName, spraySkin );

		if ( skinIndex == -1 )
			return false;

		m_TargetDisplayName = target.GetObject().GetDisplayName();
		//! Print( m_TargetDisplayName );

		// Is he in a territory ?
		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory(); // show the prompt if it's his territory

		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionActionPaint::OnStartServer - Start m_Car : " + m_Car);
		#endif

		super.OnStartServer( action_data );

		CarScript car;
		ItemBase item;

		string spraySkin;
		GetGame().ConfigGetText( "cfgVehicles " + action_data.m_MainItem.GetType() + " skinName", spraySkin );

		int skinIndex = m_SkinModule.GetSkinIndex( action_data.m_Target.GetObject().GetType(), spraySkin );
		if ( skinIndex == -1 )
			return;
		
		if ( Class.CastTo( car, action_data.m_Target.GetObject() ) )
		{
			car.ExpansionSetSkin( skinIndex );
		} else if ( Class.CastTo( item, action_data.m_Target.GetObject() ) ) 
		{
			item.ExpansionSetSkin( skinIndex );
		}

		action_data.m_MainItem.AddQuantity( -10 );
		
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() )
		{
			SEffectManager.PlaySound( "Expansion_Spraying_SoundSet", action_data.m_Target.GetObject().GetPosition() );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionActionPaint::OnStartServer - End m_Car : " + m_Car);
		#endif
	}

	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		SEffectManager.PlaySound("Expansion_Spraying_SoundSet", action_data.m_Target.GetObject().GetPosition() );
	}
};