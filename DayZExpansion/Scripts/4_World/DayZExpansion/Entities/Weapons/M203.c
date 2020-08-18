/**
 * M203.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203_HE_Explosion::Expansion_M203_HE_Explosion Start");
		#endif
		
		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false); //just simply running ExplodeNow() here doesnt work for some reason? copying explosiontest for now
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203_HE_Explosion::Expansion_M203_HE_Explosion End");
		#endif
	}
	
	void ExplodeNow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203_HE_Explosion::ExplodeNow Start");
		#endif
		
		Explode(DT_EXPLOSION, "RGD5Grenade_Ammo");
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			m_ParticleExplosion = Particle.PlayInWorld(ParticleList.RGD5, this.GetPosition());
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203_HE_Explosion::ExplodeNow End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Colorbase extends SmokeGrenadeBase
{
	const string SOUND_SMOKE_START = "SmokegGrenades_M18_start_loop_SoundSet";
	const string SOUND_SMOKE_LOOP = "SmokegGrenades_M18_active_loop_SoundSet";
	const string SOUND_SMOKE_END = "SmokegGrenades_M18_end_loop_SoundSet";

	void Expansion_M203Round_Smoke_Colorbase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Colorbase::Expansion_M203Round_Smoke_Colorbase Start");
		#endif
		
		SetAmmoType("");
		SetFuseDelay(2);
		SetSoundSmokeStart(SOUND_SMOKE_START);
		SetSoundSmokeLoop(SOUND_SMOKE_LOOP);
		SetSoundSmokeEnd(SOUND_SMOKE_END);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Colorbase::Expansion_M203Round_Smoke_Colorbase End");
		#endif
	}
	
	void ~Expansion_M203Round_Smoke_Colorbase() {}
}

class Expansion_M203Round_Smoke_White extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_White()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_White::Expansion_M203Round_Smoke_White Start");
		#endif
		
		SetParticleSmokeStart(ParticleList.GRENADE_M18_WHITE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_WHITE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_WHITE_END);
		
		Activate();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_White::Expansion_M203Round_Smoke_White End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Red extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Red()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Red::Expansion_M203Round_Smoke_Red Start");
		#endif
		
		SetParticleSmokeStart(ParticleList.GRENADE_M18_RED_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_RED_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_RED_END);

		Activate();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Red::Expansion_M203Round_Smoke_Red End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Green extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Green()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Green::Expansion_M203Round_Smoke_Green Start");
		#endif
		
		SetParticleSmokeStart(ParticleList.GRENADE_M18_GREEN_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_GREEN_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_GREEN_END);

		Activate();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Green::Expansion_M203Round_Smoke_Green End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Yellow extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Yellow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Yellow::Expansion_M203Round_Smoke_Yellow Start");
		#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_YELLOW_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_YELLOW_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_YELLOW_END);

		Activate();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Yellow::Expansion_M203Round_Smoke_Yellow End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Purple extends Expansion_M203Round_Smoke_Colorbase
{
	void Expansion_M203Round_Smoke_Purple()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Purple::Expansion_M203Round_Smoke_Purple Start");
		#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_PURPLE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_PURPLE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_PURPLE_END);
		
		Activate();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Purple::Expansion_M203Round_Smoke_Purple End");
		#endif
	}
}

class Expansion_M203Round_Smoke_Teargas extends Expansion_M203Round_Smoke_Colorbase
{
	protected ref AreaDamageBase m_AreaDamage;

	bool ZoneActive = false;

	ref Timer m_Delay;
	ref Timer m_Delay2;
	ref Timer m_Delay3;

	void Expansion_M203Round_Smoke_Teargas()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::Expansion_M203Round_Smoke_Teargas Start");
		#endif

		SetParticleSmokeStart(ParticleList.GRENADE_M18_WHITE_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_WHITE_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_WHITE_END);
		Activate();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::Expansion_M203Round_Smoke_Teargas End");
		#endif
	}

	int CoughTimer = 1000;

	override void OnWorkStart()
	{
		super.OnWorkStart();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::OnWorkStart Start");
		#endif

		ZoneActive = true;

		//! m_Delay = new Timer;
		//! m_Delay.Run(2, this, "MakePlayerCough", null, false);	

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, 2000, false);
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::OnWorkStart End");
		#endif
	}
	
	void MakePlayerCough()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::MakePlayerCough Start");
		#endif
		

		CoughTimer = Math.RandomInt(1000, 5000);
		ref array<Object> nearest_objects = new array<Object>;
		ref array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), 5, nearest_objects, proxy_cargos );
		
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			Object nearest_object = nearest_objects.Get(i);
			
			if ( nearest_object.IsInherited( PlayerBase ) )
			{
				PlayerBase player = PlayerBase.Cast( nearest_object );
				if ( player )
				{
					EntityAI foundMask = player.FindAttachmentBySlotName("Mask"); 
					if ( foundMask != NULL) 
					{
						if ( !foundMask.IsKindOf("GasMask") && !foundMask.IsKindOf("GP5GasMask") && !foundMask.IsKindOf("Expansion_Gasmask")  )
						{
							player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
							player.AddHealth("","Shock", -20);

						}
					}
					else 
					{
							player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
							player.AddHealth("","Shock", -20);

					}
				}
			}	
		}

		if ( ZoneActive )
		{
			//! m_Delay3 = new Timer;
			//! m_Delay3.Run(CoughTimer, this, "MakePlayerCough", null, false);	

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, CoughTimer, false);
		}
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M203Round_Smoke_Teargas::MakePlayerCough End");
		#endif
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();
		ZoneActive = false;
	}	
}


