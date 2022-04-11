/**
 * ActionDamageBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDamageBaseBuildingCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 1 );
	}
}

class ExpansionActionDamageBaseBuilding: ActionContinuousBase
{
	void ExpansionActionDamageBaseBuilding()
	{
		m_CallbackClass = ExpansionActionDamageBaseBuildingCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;				
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.BASEBUILDING );
	}
		
	override string GetText()
	{		
		return "#destroy"; //! placeholder, pls fix
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_obj = target.GetObject();
		if ( target_obj.IsKindOf("ExpansionBaseBuilding") && item.IsMeleeWeapon())
		{
			return true;
		}

		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		string ammoType = action_data.m_MainItem.GetMeleeCombatData().GetAmmoTypeName(0);
		string dmgPath = "CfgAmmo" + " " + ammoType + " " + "DamageApplied" + " " + "Health" + " " + "Damage";
	}
	
	protected bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			if ( target_object && target_object.CanUseConstruction() )
			{
				string part_name = target_object.GetActionComponentName( target.GetComponentIndex() );
				
				BaseBuildingBase base_building = BaseBuildingBase.Cast( target_object );
				Construction construction = base_building.GetConstruction();		
				ConstructionPart construction_part = construction.GetConstructionPartToDestroy( part_name );
				
				if ( construction_part )
				{
					//! Camera and position checks
					if ( base_building.IsFacingPlayer( player, part_name ) && !player.GetInputController().CameraIsFreeLook() )
					{
						//! Camera check 
						if ( camera_check )
						{
							if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
							{
								if ( !base_building.IsFacingCamera( part_name ) )
								{
									return false;
								}
							}
						}

						ConstructionActionData construction_action_data = player.GetConstructionActionData();
						construction_action_data.SetTargetPart( construction_part );
						
						return true;				
					}
				}
			}
		}
		
		return false;
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName();
	}
}