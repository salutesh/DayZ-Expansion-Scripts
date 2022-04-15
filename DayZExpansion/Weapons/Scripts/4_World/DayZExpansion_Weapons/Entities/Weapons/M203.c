/**
 * M203.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Expansion_M203_HE_Explosion: BuildingBase
{
	ref Timer m_Delay;
	protected Particle 		m_ParticleExplosion;

	void Expansion_M203_HE_Explosion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203_HE_Explosion");
#endif

		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false); //just simply running ExplodeNow() here doesnt work for some reason? copying explosiontest for now
	}

	void ExplodeNow()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "ExplodeNow");
#endif

		Explode(DT_EXPLOSION, "RGD5Grenade_Ammo");

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			m_ParticleExplosion = Particle.PlayInWorld(ParticleList.RGD5, this.GetPosition());
	}
};

class Expansion_M203Round_Smoke_Colorbase extends SmokeGrenadeBase
{
	const string SOUND_SMOKE_START = "SmokegGrenades_M18_start_loop_SoundSet";
	const string SOUND_SMOKE_LOOP = "SmokegGrenades_M18_active_loop_SoundSet";
	const string SOUND_SMOKE_END = "SmokegGrenades_M18_end_loop_SoundSet";

	void Expansion_M203Round_Smoke_Colorbase()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Colorbase");
#endif

		SetAmmoType("");
		SetFuseDelay(2);
		SetSoundSmokeStart(SOUND_SMOKE_START);
		SetSoundSmokeLoop(SOUND_SMOKE_LOOP);
		SetSoundSmokeEnd(SOUND_SMOKE_END);
	}

	void ~Expansion_M203Round_Smoke_Colorbase() {}

	override void SetActions()
	{
		super.SetActions();

		RemoveAction(ActionUnpin);
	}
};

class Expansion_M203Round_Smoke_White extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_White()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_White");
#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_WHITE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_WHITE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_WHITE_END);

		Activate();
	}
};

class Expansion_M203Round_Smoke_Red extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Red()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Red");
#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_RED_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_RED_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_RED_END);

		Activate();
	}
};

class Expansion_M203Round_Smoke_Green extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Green()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Green");
#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_GREEN_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_GREEN_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_GREEN_END);

		Activate();
	}
};

class Expansion_M203Round_Smoke_Yellow extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Yellow()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Green");
#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_YELLOW_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_YELLOW_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_YELLOW_END);

		Activate();
	}
};

class Expansion_M203Round_Smoke_Purple extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Purple()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Purple");
#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_PURPLE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_PURPLE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_PURPLE_END);

		Activate();
	}
};

class Expansion_M203Round_Smoke_Teargas extends Expansion_M203Round_Smoke_Colorbase
{
	protected ref ExpansionTeargasHelper m_ExpansionTeargasHelper;

	void Expansion_M203Round_Smoke_Teargas()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M203Round_Smoke_Teargas");
#endif

		m_ExpansionTeargasHelper = new ExpansionTeargasHelper( this );

		SetParticleSmokeStart(ParticleList.GRENADE_M18_WHITE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_WHITE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_WHITE_END);

		Activate();
	}

	override void OnWorkStart()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "OnWorkStart");
#endif

		super.OnWorkStart();

		m_ExpansionTeargasHelper.OnWorkStart();
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		m_ExpansionTeargasHelper.OnWorkStop();
	}
};
