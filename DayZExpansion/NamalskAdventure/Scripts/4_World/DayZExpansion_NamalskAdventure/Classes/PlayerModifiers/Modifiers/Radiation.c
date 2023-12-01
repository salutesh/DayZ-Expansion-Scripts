/**
 * Raditation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Radiation extends ModifierBase
{
	private const float	RADIATION_CLEANUP_PER_SEC = 0.1;
	private const float VOMIT_THRESHOLD = 70;
	private const float DAMAGE_THRESHOLD = 30;
	private const float HEALTH_LOSS = 0.1;
	
	override void Init()
	{
		m_TrackActivatedTime 	= false;
		m_ID 					= eModifiersNamalskAdventure.NA_MDF_RADIATION;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
		//DisableDeactivateCheck();
	}

	override bool ActivateCondition( PlayerBase player )
	{
		return true;
	}

	override void OnActivate( PlayerBase player )
	{
	}

	override void OnReconnect( PlayerBase player )
	{

	}

	override bool DeactivateCondition( PlayerBase player )
	{
		return false;
	}

	override void OnTick( PlayerBase player, float deltaT )
	{
		//! Clean up a bit radiation each second
		float radiationCleanup = -RADIATION_CLEANUP_PER_SEC * deltaT;	
	#ifdef NAMALSKADVENTURE_RADIATION_DEBUG
		Print(ToString() + "::OnTick - Radiation cleanup: " + radiationCleanup);	
	#endif
		player.GetStatRadiation().Add(radiationCleanup);
		player.ForceUpdateRadiation();
		
		float radiation = player.GetStatRadiation().Get();
	#ifdef NAMALSKADVENTURE_RADIATION_DEBUG
		Print(ToString() + "::OnTick - Radiation: " + radiation);
	#endif
		
		if(radiation > DAMAGE_THRESHOLD)
		{
			float healthLoss = radiation * HEALTH_LOSS;
			player.AddHealth("","Health", -healthLoss);
		}

		if(radiation > VOMIT_THRESHOLD)
		{
			SymptomBase symptom = player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);
			if( symptom )
			{
				symptom.SetDuration(Math.RandomIntInclusive(4,8));
			}
		}
	}
};