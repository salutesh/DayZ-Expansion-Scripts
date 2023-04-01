/**
 * ExpansionAnomalyTrigger_Dynamic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTrigger_Dynamic : ExpansionAnormalAreaTrigger
{
	protected int m_AreaState; //! Used to get the state from linked area and update effects

	void ExpansionAnomalyTrigger_Dynamic()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		RegisterNetSyncVariableInt("m_AreaState");

		//! Register in the trigger effect manager
		m_Manager = TriggerEffectManager.GetInstance();
		m_Manager.RegisterTriggerType( this );
	}

	void SetAreaState(int state)
	{
		m_AreaState = state;
		SetSynchDirty();
	}

	override void OnEnterClientEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterClientEvent - Insider: " + insider.GetObject().ToString());

		if (insider.GetObject().IsInherited(PlayerBase))
		{
			PlayerBase playerInsider = PlayerBase.Cast(insider.GetObject());

			//! We will only handle the controlled player, as effects are only relevant to this player instance
			if (playerInsider.IsControlledPlayer())
			{
				//! If it is first entrance, we fire the following
				if (!m_Manager.IsPlayerInTriggerType(playerInsider, this))
				{
					//! We check if we are not in the standard LIVE state
					bool nonDefaultState = m_AreaState > eAreaDecayStage.LIVE;
					if (nonDefaultState)
					{
						//! if not we check the specific state and update local effect values
						int localPartBirthRate;
						if (m_AreaState == eAreaDecayStage.DECAY_START)
							localPartBirthRate = 20;
						else
							localPartBirthRate = 10;

						playerInsider.SetContaminatedEffectEx(true, m_PPERequester, m_AroundPartId, m_TinyPartId, GetAmbientSoundsetName(), nonDefaultState, localPartBirthRate);
					}
					else
					{
						playerInsider.SetContaminatedEffectEx(true, m_PPERequester, m_AroundPartId, m_TinyPartId, GetAmbientSoundsetName());
					}
				}

				//! We then handle the update of player trigger state in manager
				m_Manager.OnPlayerEnter(playerInsider, this);
			}
		}
	}

	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();

		//! Is the controlled player inside when trigger is synchronized
		for (int i = 0; i < GetInsiders().Count(); i++)
		{
			PlayerBase playerInsider = PlayerBase.Cast(m_insiders.Get(i).GetObject());
			if (playerInsider)
			{
				if (playerInsider.IsControlledPlayer())
				{
					//! Update Local particle effects
					bool nonDefaultState = m_AreaState > eAreaDecayStage.LIVE;
					if (nonDefaultState)
					{
						int localPartBirthRate;
						if (m_AreaState == eAreaDecayStage.DECAY_START)
							localPartBirthRate = 20;
						else
							localPartBirthRate = 10;

						//! Update the local effects
						playerInsider.SetContaminatedEffectEx(true, m_PPERequester, m_AroundPartId, m_TinyPartId, GetAmbientSoundsetName(), nonDefaultState, localPartBirthRate);
					}
				}
			}
		}
	}
};