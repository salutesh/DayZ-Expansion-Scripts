/**
 * ExpansionAnomalySingularityTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalySingularityTrigger: ExpansionAnomalyTriggerBase
{
	override void OnEnterAnomalyServer(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnEnterAnomalyServer - Entity: " + other.ToString());

		super.OnEnterAnomalyServer(other);

		ProcessEntityEvents(other);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	protected void ProcessEntityEvents(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::ProcessEntityEvents - Entity: " + other.ToString());
		
		EntityAI objectEntity = EntityAI.Cast(other);
		if (objectEntity)
		{
			/*if (ExpansionStatic.IsAnyOf(objectEntity, s_Items, true))
			{
				ItemBase item = ItemBase.Cast(objectEntity);
				if (!item)
                	return;

				item.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED));	//! Apply random damage to the item.
				ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(item, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED);	//! Apply random damage to the players gear items.
				
				m_IsActive = false;
			}
			else*/ if (ExpansionStatic.IsAnyOf(objectEntity, s_Players, true))
			{
				PlayerBase player = PlayerBase.Cast(objectEntity);
				if (!player || !player.IsAlive())
                	return;
				
			#ifdef NAMALSK_SURVIVAL
				if (ExpansionAnomaliesModule.GetModuleInstance().HasActiveLEHSSuit(player))
					return;
			#endif

				if (!player.IsInTransport())
				{
					player.GetStatHeatComfort().Add(-1000.0); //! Let the character feel the cold.
					player.GetStatRadiation().Add(10.0); //! Let the character get some radiation.
					player.ForceUpdateRadiation();
					
					player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_PAIN_LIGHT); //! Let the character feel the pain :)
					player.GiveShock(Math.RandomFloatInclusive(MIN_SHOCK_INFLICTED, MAX_SHOCK_INFLICTED)); //! Apply random shock damage to the player.
					player.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the player.
					player.GetBleedingManagerServer().AttemptAddBleedingSource(1); //! Add a bleeding source to the character.

					ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(player, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED);	//! Apply random damage to the players gear items.
				}

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, s_Vehicles, true))
			{
				CarScript car = CarScript.Cast(objectEntity);
				if (!car)
                	return;

				car.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED));	//! Apply random damage to the vehicle.

				ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(car, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED); //! Apply random damage to the vehicle cargo items.

				m_IsActive = false;
			}
			/*else if (ExpansionStatic.IsAnyOf(objectEntity, m_Animals, true))
			{
                AnimalBase animal = AnimalBase.Cast(objectEntity);
				if (!animal || !animal.IsAlive())
                	return;

				animal.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the animal.

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Infected, true))
			{
                ZombieBase zombie = ZombieBase.Cast(objectEntity);
				if (!zombie || !zombie.IsAlive())
                	return;

				zombie.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the infected.

				m_IsActive = false;
			}*/
		}
	}
};