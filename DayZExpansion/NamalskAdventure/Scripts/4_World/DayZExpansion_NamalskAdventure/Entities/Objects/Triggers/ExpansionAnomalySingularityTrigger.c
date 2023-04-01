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
	protected const float MAX_CARGODMG_INFLICTED = -5.0; //! Max. damage infliced on players gear when triggering the anomaly.
	protected const float MIN_CARGODMG_INFLICTED = -1.0; //! Min. damage infliced on players gear when triggering the anomaly.
	protected const float MAX_SHOCK_INFLICTED = -25.0; //! Max. shock damage infliced on players.
	protected const float MIN_SHOCK_INFLICTED = -20.0; //! Min. shock damage infliced on players.
	protected const float MAX_DMG_INFLICTED = -10.0; //! Max. damage infliced on players.
	protected const float MIN_DMG_INFLICTED = -5.0; //! Min. damage infliced on players.

	override void OnEnterAnomalyServer(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterAnomalyServer - Entity: " + other.ToString());
		
		super.OnEnterAnomalyServer(other);
		
		ProcessEntityEvents(other);
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	/*override void OnEnterAnomalyClient(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterAnomalyClient - Entity: " + other.ToString());

		super.OnEnterAnomalyClient(other);
	}*/

	protected void ProcessCargoDamage(EntityAI parent)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::ProcessCargoDamage - Entity: " + parent.ToString());

		array<EntityAI> items = new array<EntityAI>;
		parent.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item: items)
		{
			if (item.IsInherited(SurvivorBase))
				continue;

			item.AddHealth("", "", Math.RandomFloatInclusive(MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED));
		}
	}

	protected void ProcessEntityEvents(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::ProcessEntityEvents - Entity: " + other.ToString());

		if (other)
		{
			EntityAI objectEntity = EntityAI.Cast(other);
			
			//! Get wind values and use it to let the entity fly - TEST
			/*float mass;
			vector wind = GetGame().GetWeather().GetWind();
			wind[0] = ((wind[0] + 0.1) * 2) / 100;
			wind[1] = 12.0; //! Let the entity fly 12 meters into the air.
			wind[2] = ((wind[2] + 0.1) * 2) / 100;*/

			if (ExpansionStatic.IsAnyOf(objectEntity, m_Items, true))
			{
				ItemBase item = ItemBase.Cast(objectEntity);
				if (!item)
                	return;
				
				//! Let the entity fly - TEST
				//mass = dBodyGetMass(item);
				//item.dBodyApplyImpulse(item, mass * wind);

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Players, true))
			{
				PlayerBase player = PlayerBase.Cast(objectEntity);
				if (!player || !player.IsAlive())
                	return;
				
				if (HasActiveLEHSSuit(player))
					return;

				if (!player.IsInTransport())
				{
					//! Let the entity fly - TEST
					//mass = dBodyGetMass(player);
					//player.dBodyApplyImpulse(player, mass * wind);
					
					//! Drop all items from entities inventroy/cargo - TEST
					//MiscGameplayFunctions.ThrowAllItemsInInventory(player, 0);
	
					float heatComfort = player.GetStatHeatComfort().Get();
					heatComfort = heatComfort - 1000.0;
					player.GetStatHeatComfort().Set(heatComfort); //! Let the character feel the cold.
					player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_PAIN_LIGHT); //! Let the character feel the pain :)
					player.GiveShock(Math.RandomFloatInclusive(MIN_SHOCK_INFLICTED, MAX_SHOCK_INFLICTED)); //! Apply random shock damage to the player.
					player.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the player.
					player.GetBleedingManagerServer().AttemptAddBleedingSource(1); //! Add a bleeding source to the character.
					ProcessCargoDamage(player);	//! Apply random damage to the players gear items.
				}
				
				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Vehicles, true))
			{
				CarScript car = CarScript.Cast(objectEntity);
				if (!car)
                	return;

				//! Let the entity fly - TEST
				//mass = dBodyGetMass(car);
				//car.dBodyApplyImpulse(car, mass * wind);

				car.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED));	//! Apply random damage to the vehicle.
				ProcessCargoDamage(car); //! Apply random damage to the vehicle cargo items.
				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Animals, true))
			{
                AnimalBase animal = AnimalBase.Cast(objectEntity);
				if (!animal || !animal.IsAlive())
                	return;

				//! Let the entity fly - TEST
				//mass = dBodyGetMass(animal);
				//animal.dBodyApplyImpulse(animal, mass * wind);

				animal.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the animal.
				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Infected, true))
			{
                ZombieBase zombie = ZombieBase.Cast(objectEntity);
				if (!zombie || !zombie.IsAlive())
                	return;

				//! Let the entity fly - TEST
				//mass = dBodyGetMass(zombie);
				//zombie.dBodyApplyImpulse(zombie, mass * wind);

				zombie.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the infected.
				m_IsActive = false;
			}
		}
	}
};