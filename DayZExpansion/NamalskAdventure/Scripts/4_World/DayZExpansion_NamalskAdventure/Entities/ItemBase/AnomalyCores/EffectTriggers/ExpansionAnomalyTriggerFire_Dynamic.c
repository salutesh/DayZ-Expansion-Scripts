/**
 * ExpansionAnomalyTriggerFire_Dynamic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTriggerFire_Dynamic : ExpansionAnomalyTriggerBase_Dynamic
{
	protected Particle m_ParticleTarget;

	//! Condition checks on given entity.
	protected bool EntityConditions(Object entityObj)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		ExDebugPrint("::EntityConditions - Entity: " + entityObj.GetType());

		if (ExpansionStatic.IsAnyOf(entityObj.GetType(), s_Players) || ExpansionStatic.IsAnyOf(entityObj.GetType(), s_Vehicles))
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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnStayServerEvent - Insider: " + insider.GetObject().GetType());

		super.OnStayServerEvent(insider, deltaTime);

		if (EntityConditions(insider.GetObject()) && m_DealDamageFlag)
		{
			ProcessEntityEvents(insider.GetObject());
		}
	}

	protected void ProcessEntityEvents(Object entityObj)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::ProcessEntityEvents - Entity: " + entityObj.GetType());
	}

	/*protected void PlayFXTarget(vector pos, PlayerIdentity identity)
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
	}*/
};