/**
 * Expansion_AnomalyCore_Warper.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyCoreLightWarper extends ExpansionAnomalyCoreLightBase
{
	void ExpansionAnomalySingularityLightBase()
	{
		SetDiffuseColor(0.751, 0.196, 0.890);
		SetAmbientColor(0.751, 0.196, 0.890);
	}
};

enum Expansion_AnomalyCore_ERPCs
{
	PLAY_TARGET_FX = 3000001
};

class Expansion_AnomalyCore_Warper: Expansion_AnomalyCore_Base
{
	protected const int PARTICLE_EFFECT = ParticleList.EXPANSION_PARTICLE_ANOMALY_CORE_ICE;

	protected const float MAX_CARGODMG_INFLICTED = -5.0; //! Max. damage infliced on players gear when triggering the anomaly.
	protected const float MIN_CARGODMG_INFLICTED = -1.0; //! Min. damage infliced on players gear when triggering the anomaly.
	protected const float MAX_SHOCK_INFLICTED = -25.0; //! Max. shock damage infliced on players.
	protected const float MIN_SHOCK_INFLICTED = -20.0; //! Min. shock damage infliced on players.
	protected const float MAX_DMG_INFLICTED = -10.0; //! Max. damage infliced on players.
	protected const float MIN_DMG_INFLICTED = -5.0; //! Min. damage infliced on players.

	protected Particle m_ParticleActivated;
	protected Particle m_ParticleTarget;
	protected EffectSound m_ActivatedSound;
	
	void Expansion_AnomalyCore_Warper()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetEventMask(EntityEvent.CONTACT | EntityEvent.TOUCH);
		SetFlags(EntityFlags.TRIGGER, false);
	}

	override protected void OnExplode()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer())
		{
			GetGame().CreateObject("ExpansionAnomalyAreaWarper_Local", GetPosition());
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.ACTIVATED);
		}
	}

	override void OnActivateFinished()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer())
		{
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.DESTROYED);
		}

		super.OnActivateFinished();
	}

	//! Condition checks on given entity.
	protected bool EntityConditions(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Object entityObj;
		Object.CastTo(entityObj, other);
		if (!entityObj)
			return false;
		
		ExDebugPrint("::EntityConditions - Entity: " + entityObj.GetType());
		
		if (ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Items) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Players) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Animals) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Vehicles) || ExpansionStatic.IsAnyOf(entityObj.GetType(), m_Infected))
		{
			PlayerBase player = PlayerBase.Cast(other);
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

	protected void ProcessEntityEvents(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::ProcessEntityEvents - Entity: " + other.ToString());

		if (other)
		{
			EntityAI objectEntity = EntityAI.Cast(other);
			vector position = objectEntity.GetPosition();
			array<vector> positions = ExpansionAnomaliesModule.GetModuleInstance().GeneratePositions(position, 500, 1);
			vector randomPosition = positions[0];
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
					
					PlayTeleportSFX(randomPosition, player.GetIdentity());
				}
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
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Animals, true))
			{
                AnimalBase animal = AnimalBase.Cast(objectEntity);
				if (!animal || !animal.IsAlive())
                	return;

				animal.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the animal.

				animal.SetPosition(randomPosition);
				animal.SetOrientation(ori);
			}
			else if (ExpansionStatic.IsAnyOf(objectEntity, m_Infected, true))
			{
                ZombieBase zombie = ZombieBase.Cast(objectEntity);
				if (!zombie || !zombie.IsAlive())
                	return;

				zombie.AddHealth("", "", Math.RandomFloatInclusive(MIN_DMG_INFLICTED, MAX_DMG_INFLICTED)); //! Apply random damage to the infected.

				zombie.SetPosition(randomPosition);
				zombie.SetOrientation(ori);
			}
		}
	}
	
	protected void PlayVFX()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsDedicatedServer())
		{
			//! Ideally play a one time effect such as an explosion
			m_ParticleActivated = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPANSION_PARTICLE_WARPER_ACTIVATE, GetPosition());
			m_ParticleActivated.SetWiggle(7, 0.3);
		}
	}
	
	protected void PlaySFX()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsDedicatedServer())
			PlaySoundSet(m_ActivatedSound, "EasterEgg_Catch_SoundSet", 0, 0);
	}
	
	protected void PlayVFXTarget(vector pos)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsDedicatedServer())
		{
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

	override void EOnTouch(IEntity other, int extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (m_CoreState != ExpansionAnomalyCoreState.DESTROYED)
			return;
		
		if (GetGame().IsServer())
		{
			if (!EntityConditions(other))
				return;
	
			ProcessEntityEvents(other);
		}
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (m_CoreState != ExpansionAnomalyCoreState.ACTIVATED)
			return;

		PlayVFX();
		PlaySFX();
	}
	
	protected void PlayTeleportSFX(vector pos, PlayerIdentity identity)
	{
		Param1<vector> p = new Param1<vector>(pos);
		GetGame().RPCSingleParam(this, Expansion_AnomalyCore_ERPCs.PLAY_TARGET_FX, p, true, identity);
	}

	override int GetAnomalyCoreParticle()
	{
		return PARTICLE_EFFECT;
	}

	override typename GetAnomalyCoreLight()
	{
		return ExpansionAnomalyCoreLightWarper;
	}
	
	//! On server -> client synchronization
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
};