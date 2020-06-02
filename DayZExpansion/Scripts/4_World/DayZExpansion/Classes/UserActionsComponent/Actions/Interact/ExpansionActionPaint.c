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
}

class ExpansionActionPaint: ActionContinuousBase
{
	private string m_TargetName;
	private string m_TargetDisplayName;

	private CarScript m_Car;
	private ItemBase m_Item;
	private ExpansionSpraycanBase m_SprayCan;

	private int m_SkinIndex;

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
		//! Print( m_SkinModule );
		if ( !m_SkinModule )
			return false;

		//! Print( target );
		if ( !target )
			return false;

		//! Print( player.GetItemInHands() );
		if ( !Class.CastTo( m_SprayCan, player.GetItemInHands() ) )
			return false;

		//! Print( target.GetObject() );
		if ( !Class.CastTo( m_Car, target.GetObject() ) )
			if ( !Class.CastTo( m_Item, target.GetObject() ) )
				return false;

		TStringArray objectList = new TStringArray;
		GetGame().ConfigGetTextArray( "cfgVehicles " + m_SprayCan.GetType() + " ExpansionSkin obj", objectList );

		//objectList.Debug();

		m_TargetName = target.GetObject().GetType();
		//! Print( m_TargetName );
		if ( objectList.Find( m_TargetName ) < 0 )
			return false;

		string spraySkin;
		GetGame().ConfigGetText( "cfgVehicles " + m_SprayCan.GetType() + " ExpansionSkin skin", spraySkin );

		m_SkinIndex = m_SkinModule.GetSkinIndex( m_TargetName, spraySkin );

		//! Print( spraySkin );
		//! Print( m_SkinIndex );
		if ( m_SkinIndex == -1 )
			return false;

		m_TargetDisplayName = target.GetObject().GetDisplayName();
		//! Print( m_TargetDisplayName );

		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionActionPaint::OnStartServer - Start m_Car : " + m_Car);
		#endif

		super.OnStartServer( action_data );
		
		if ( m_Car) 
		{
			m_Car.ExpansionSetSkin( m_SkinIndex );
		} else if ( m_Item ) 
		{
			m_Item.ExpansionSetSkin( m_SkinIndex );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionActionPaint::OnStartServer - End m_Car : " + m_Car);
		#endif
	}
}