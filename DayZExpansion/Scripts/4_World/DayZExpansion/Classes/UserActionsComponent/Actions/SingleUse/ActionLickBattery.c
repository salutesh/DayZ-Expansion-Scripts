/**
 * ActionLickBattery.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ActionLickBattery: ActionSingleUseBase
{
	ItemBase m_Battery;
	float m_Quantity;

	void ActionLickBattery()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool HasTarget()
	{
		return false;
	}

	ItemBase GetWire()
	{
		return ItemBase.Cast( m_Battery.GetAttachmentByType( MetalWire ) );
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		m_Battery = item;
		m_Quantity = item.GetQuantity();

		return true;
	}

	override string GetText()
	{
		if ( m_Battery.IsInherited( VehicleBattery ) )
		{
			if ( GetWire() )
				return "#STR_EXPANSION_LICK_VEHICLEBATTERY";
			else
				return "#STR_EXPANSION_LICK_VEHICLEBATTERY_NEEDWIRE";
		}
		else
			return "#STR_EXPANSION_LICK_BATTERY";
	}

	override void OnExecute( ActionData action_data )
	{
		bool canDo;
		if (!m_Battery.IsInherited( VehicleBattery ) || GetWire() != NULL)
			canDo = true;

		if ( m_Battery.IsInherited( VehicleBattery ) && m_Quantity > 0 && canDo )
		{
			//! What follows may not be realistic, but it sure is funny :-)

			if ( IsMissionClient() )
			{
				EffectSound sound = SEffectManager.PlaySound( "Expansion_Shocks_SoundSet", action_data.m_Player.GetPosition() );
				sound.SetSoundAutodestroy( true );
			}

			if ( IsMissionHost() )
			{
				//! Shock dmg
				action_data.m_Player.ProcessDirectDamage( DT_CUSTOM, m_Battery, "", "Bullet_Expansion_Taser", "0.5 0.5 0.5", 1 );
			}
		}

		//! Deplete the battery a teeny tiny bit
		if ( IsMissionHost() && canDo )
			m_Battery.SetQuantity( m_Quantity - m_Battery.GetQuantityMax() * 0.001 );

		if ( IsMissionClient() )
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( NotifyPlayer, 500, false, action_data.m_Player.GetIdentity() );
	}

	void NotifyPlayer( PlayerIdentity identity )
	{
		string title;
		string message;

		if ( m_Battery.IsInherited( VehicleBattery ) && !GetWire() )
		{
			title = "STR_EXPANSION_LICK_VEHICLEBATTERY_NEEDWIRE_TITLE";
			message = "STR_EXPANSION_LICK_VEHICLEBATTERY_NEEDWIRE_TEXT";
		} else if ( m_Quantity > 0 )
		{
			if ( m_Battery.IsInherited( VehicleBattery ) )
			{
				title = "STR_EXPANSION_LICKED_VEHICLEBATTERY_TITLE";
				message = "STR_EXPANSION_LICKED_VEHICLEBATTERY_TEXT";
			} else
			{
				title = "STR_EXPANSION_LICKED_BATTERY_TITLE";
				message = "STR_EXPANSION_LICKED_BATTERY_TEXT";
			}
		} else
		{
			title = "STR_EXPANSION_LICKED_BATTERY_TITLE";
			message = "STR_EXPANSION_LICKED_EMPTY_BATTERY_TEXT";
		}

		ExpansionNotification(title, message, EXPANSION_NOTIFICATION_ICON_T_Batteries, ARGB( 255, 211, 84, 0 ), 10).Create(identity);
	}
}
