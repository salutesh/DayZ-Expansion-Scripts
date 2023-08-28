/**
 * ExpansionAnomalyTriggerWarper_Dynamic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTriggerWarper_Dynamic : ExpansionAnomalyTriggerBase_Dynamic
{
	protected Particle m_ParticleTarget;
	
	void ExpansionAnomalyTriggerWarper_Dynamic()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	//! Condition checks on given entity.
	protected bool EntityConditions(Object entityObj)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExDebugPrint("::EntityConditions - Entity: " + entityObj.GetType());
		
		if (ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Items) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Players) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Animals) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Vehicles) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Infected))
		{
			PlayerBase player = PlayerBase.Cast(entityObj);
			if (player && ExpansionAnomaliesModule.GetModuleInstance().HasActiveLEHSSuit(player))
			{
				ExDebugPrint("::EntityConditions - Return FALSE. Entity is player and has LEHS suit!");
				return false;
			}

			ExDebugPrint("::EntityConditions - Return TRUE");
			return true;
		}

		ExDebugPrint("::EntityConditions - Return FALSE");
		return false;
	}
	
	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnStayServerEvent - Insider: " + insider.GetObject().GetType());

		super.OnStayServerEvent(insider, deltaTime);

		if (EntityConditions(insider.GetObject()) && m_DealDamageFlag)
		{
			ProcessEntityEvents(insider.GetObject());
		}
	}

	protected void ProcessEntityEvents(Object entityObj)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::ProcessEntityEvents - Entity: " + entityObj.GetType());

		vector position = entityObj.GetPosition();
		float squareSize = Math.RandomFloat(200.0, 250.0);
		array<vector> positions = ExpansionAnomaliesModule.GetModuleInstance().GeneratePositions(position, squareSize, 10);
		vector randomPosition;

		foreach (vector pos: positions)
		{
			if (!ExpansionStatic.SurfaceIsWater(pos))
			{
				randomPosition = pos;
				break;
			}
			
			randomPosition = position;
		}
		
		randomPosition[1] = GetGame().SurfaceY(randomPosition[0], randomPosition[2]);
		vector ori = entityObj.GetOrientation();

		if (ExpansionStatic.IsAnyOf(entityObj, m_Items, true))
		{
			ItemBase item = ItemBase.Cast(entityObj);
			if (!item)
               	return;
			
			//! Get wind values and use it to let the entity fly - TEST
			float mass;
			vector wind = GetGame().GetWeather().GetWind();
			wind[0] = ((wind[0] + 0.1) * 2) / 100;
			wind[1] = 12.0; //! Let the entity fly 12 meters into the air.
			wind[2] = ((wind[2] + 0.1) * 2) / 100;

			item.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the item.
			ExpansionAnomaliesModule.GetModuleInstance().ProcessCargoDamage(item, MIN_CARGODMG_INFLICTED, MAX_CARGODMG_INFLICTED);

			item.SetPosition(randomPosition);
			item.SetOrientation(ori);

			//! Let the entity fly - TEST
			mass = dBodyGetMass(item);
			item.dBodyApplyImpulse(item, mass * wind);
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntityPos, 3000, false, entityObj, position);
		}
		else if (ExpansionStatic.IsAnyOf(entityObj, m_Players, true))
		{
			PlayerBase player = PlayerBase.Cast(entityObj);
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
				PlayFXTarget(randomPosition, player.GetIdentity());
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntityPos, 3000, false, entityObj, position);
			}
		}
		else if (ExpansionStatic.IsAnyOf(entityObj, m_Vehicles, true))
		{
			Transport transport = Transport.Cast(entityObj);
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
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntityPos, 3000, false, entityObj, position);
		}
		else if (ExpansionStatic.IsAnyOf(entityObj, m_Animals, true))
		{
               AnimalBase animal = AnimalBase.Cast(entityObj);
			if (!animal || !animal.IsAlive())
               	return;

			animal.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the animal.

			animal.SetPosition(randomPosition);
			animal.SetOrientation(ori);
		}
		else if (ExpansionStatic.IsAnyOf(entityObj, m_Infected, true))
		{
               ZombieBase zombie = ZombieBase.Cast(entityObj);
			if (!zombie || !zombie.IsAlive())
               	return;

			zombie.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the infected.

			zombie.SetPosition(randomPosition);
			zombie.SetOrientation(ori);
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntityPos, 3000, false, entityObj, position);
		}
	}

	protected void PlayFXTarget(vector pos, PlayerIdentity identity)
	{
		Param1<vector> p = new Param1<vector>(pos);
		GetGame().RPCSingleParam(this, Expansion_AnomalyCore_ERPCs.PLAY_TARGET_FX, p, true, identity);
	}
	
	protected void PlayVFXTarget(vector pos)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsDedicatedServer())
		{
			if (m_ParticleTarget)
				GetGame().ObjectDelete(m_ParticleTarget);
						
			//! Ideally play a one time effect such as an explosion
			m_ParticleTarget = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPANSION_PARTICLE_WARPER_ACTIVATE, pos);
			m_ParticleTarget.SetWiggle(7, 0.3);
		}
	}
	
	protected void PlaySFXTarget(vector pos)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsDedicatedServer())
		{
			EffectSound soundEffect = SEffectManager.PlaySound("Blowout_Teleport", pos, 0, 0, false);
			if (!soundEffect)
				return;
	
			soundEffect.SetParent(GetGame().GetPlayer());
			soundEffect.SetSoundAutodestroy(true);
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (!GetGame().IsDedicatedServer())
		{
			switch (rpc_type)
			{
				case Expansion_AnomalyCore_ERPCs.PLAY_TARGET_FX:
				{
				#ifndef EDITOR
					ref Param1<vector> p_pos = new Param1<vector>(vector.Zero);
					if (ctx.Read(p_pos))
					{
						PlaySFXTarget(p_pos.param1);
						PlayVFXTarget(p_pos.param1);
					}
				#endif
				}
				break;
			}
		}
	}
	
	protected void CheckEntityPos(EntityAI objectEntity, vector oldPos)
	{
		float dist = vector.Distance(objectEntity.GetPosition(), Vector(0, 0, 0));
		if (dist < 100)
		{
			if (ExpansionStatic.IsAnyOf(objectEntity, m_Players, true))
			{
				DayZPlayerSyncJunctures.ExpansionTeleport(DayZPlayer.Cast(objectEntity), oldPos, objectEntity.GetOrientation());
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Vehicles, true))
			{
				CarScript.Cast(objectEntity).COT_PlaceOnSurfaceAtPosition(oldPos);
			}
			else
			{
				objectEntity.SetPosition(oldPos);
			}
		}
	}
};