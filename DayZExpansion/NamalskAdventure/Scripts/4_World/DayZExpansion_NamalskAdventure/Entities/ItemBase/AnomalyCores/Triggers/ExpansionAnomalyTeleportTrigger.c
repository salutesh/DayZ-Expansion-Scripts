/**
 * ExpansionAnomalyTeleportTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTeleportTrigger: ExpansionAnomalyTriggerBase
{
	override void OnEnterAnomalyServer(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnEnterAnomalyServer - Entity: " + other.ToString());

		super.OnEnterAnomalyServer(other);

		ProcessEntityEvents(other);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	protected void ProcessEntityEvents(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::ProcessEntityEvents - Entity: " + other.ToString());

		if (other)
		{
			EntityAI objectEntity = EntityAI.Cast(other);
			vector position = objectEntity.GetPosition();
			float offsetX = Math.RandomFloatInclusive(5.0, 10.0);
			float offsetY = Math.RandomFloatInclusive(5.0, 10.0);
			vector randomPosition = Vector(position[0] + offsetX, position[1], position[2] + offsetY);
			randomPosition[1] = GetGame().SurfaceY(randomPosition[0], randomPosition[2]);
			vector ori = objectEntity.GetOrientation();

			//! Get wind values and use it to let the entity fly - TEST
			float mass;
			vector wind = GetGame().GetWeather().GetWind();
			wind[0] = ((wind[0] + 0.1) * 2) / 100;
			wind[1] = 12.0; //! Let the entity fly 12 meters into the air.
			wind[2] = ((wind[2] + 0.1) * 2) / 100;

			if (ExpansionStatic.IsAnyOf(objectEntity, m_Items, true))
			{
				ItemBase item = ItemBase.Cast(objectEntity);
				if (!item)
                	return;

				item.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the item.
				ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(item, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED);

				item.SetPosition(randomPosition);
				item.SetOrientation(ori);

				//! Let the entity fly - TEST
				mass = dBodyGetMass(item);
				item.dBodyApplyImpulse(item, mass * wind);

				m_IsActive = false;
			}
			else
			if (ExpansionStatic.IsAnyOf(objectEntity, m_Players, true))
			{
				PlayerBase player = PlayerBase.Cast(objectEntity);
				if (!player || !player.IsAlive())
                	return;

				if (ExpansionAnomaliesModule.GetModuleInstance().HasActiveLEHSSuit(player))
					return;

				if (!player.IsInTransport())
				{
					player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_PAIN_LIGHT); //! Let the character feel the pain :)
					player.GiveShock(Math.RandomFloatInclusive(MIN_SHOCK_INFLICTED, MAX_SHOCK_INFLICTED)); //! Apply random shock damage to the player.
					player.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the player.
					ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(player, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED);	//! Apply random damage to the players gear items.
					DayZPlayerSyncJunctures.ExpansionTeleport(player, randomPosition, ori);
				}

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Vehicles, true))
			{
				Transport transport = Transport.Cast(objectEntity);
				if (!transport)
					return;

				CarScript car = CarScript.Cast(transport);
				if (!car)
					return;

				car.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED));	//! Apply random damage to the vehicle.
				ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(car, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED); //! Apply random damage to the vehicle cargo items.

			#ifdef JM_COT
				car.COT_PlaceOnSurfaceAtPosition(randomPosition);
			#endif

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Animals, true))
			{
                AnimalBase animal = AnimalBase.Cast(objectEntity);
				if (!animal || !animal.IsAlive())
                	return;

				animal.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the animal.

				animal.SetPosition(randomPosition);
				animal.SetOrientation(ori);

				m_IsActive = false;
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Infected, true))
			{
                ZombieBase zombie = ZombieBase.Cast(objectEntity);
				if (!zombie || !zombie.IsAlive())
                	return;

				zombie.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the infected.

				zombie.SetPosition(randomPosition);
				zombie.SetOrientation(ori);

				m_IsActive = false;
			}
		}
	}
};