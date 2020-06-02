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
	protected CarScript m_Car;
	protected ExpansionCarKeys m_KeysInHand;

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
		m_Car = NULL;
        m_KeysInHand = NULL;

		if ( !target || !player )
			return false;

		bool playerIsInVehicle = false;

		if ( player.GetCommand_Vehicle() )
		{
			if ( Class.CastTo( m_Car, player.GetCommand_Vehicle().GetTransport() ) )
			{
				playerIsInVehicle = true;
			}
		}

		if ( !playerIsInVehicle )
		{
			if ( !target || !Class.CastTo( m_Car, target.GetObject() ) )
			{
				return false;
			}
			if ( !Class.CastTo( m_KeysInHand, player.GetItemInHands() ) )
			{
				return false;
			}
		}
	
        if ( !m_Car.HasKey() )
			return false;

        if ( m_Car.GetLockedState() == ExpansionVehicleLockState.UNLOCKED )
        	return false;
		
		return true;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		PlayLockSound();
		
		if (!GetGame().IsMultiplayer())
		{
			OnExecuteServer(action_data);
		}
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		m_Car.UnlockCar( m_KeysInHand );
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
