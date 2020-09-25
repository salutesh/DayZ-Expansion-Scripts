/**
 * ExpansionActionUnlockVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUnlockVehicle: ActionInteractBase
{
	// Sound
	protected PlayerBase								m_Player;
	protected EffectSound 								m_SoundLock;
	
	void ExpansionActionUnlockVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#unlock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		bool playerIsInVehicle = false;
		
		CarScript car;
		ExpansionCarKey key;

		if ( player.GetCommand_Vehicle() )
		{
			if ( Class.CastTo( car, player.GetCommand_Vehicle().GetTransport() ) )
				playerIsInVehicle = true;
			
		}

		if ( !playerIsInVehicle )
		{
			if ( !target || !Class.CastTo( car, target.GetObject() ) )
				return false;

			if ( !Class.CastTo( key, player.GetItemInHands() ) )
				return false;
		}
	
		if ( !car.HasKey() )
			return false;

		if ( car.GetLockedState() == ExpansionVehicleLockState.UNLOCKED )
			return false;
		
		return true;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		PlayLockSound();
		
		if (!GetGame().IsMultiplayer())
		{
			OnExecuteServer( action_data );
		}
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		CarScript car = CarScript.Cast( action_data.m_Target.GetObject() );
		car.UnlockCar( ExpansionCarKey.Cast( action_data.m_MainItem ) );
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}

	void PlayLockSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( m_Player )
			{
				m_SoundLock = SEffectManager.PlaySound("Expansion_Car_Lock_SoundSet", m_Player.GetPosition());
				m_SoundLock.SetSoundAutodestroy( true );
			}
		}
	}
}
