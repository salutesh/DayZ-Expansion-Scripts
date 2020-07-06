/**
 * ExpansionRoadflare.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlares
 * @brief		This class handle flare post processing
 **/
class ExpansionRoadflare extends Roadflare
{
	ref Timer m_FlareTimer;
	ref Timer m_FlareUpdateTimer;
	ref Timer m_FlarePhysicsTimer;
	
	Particle m_FlareSmoke;
	ExpansionPointLight m_FlareLight;

	void ExpansionRoadflare()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] Constructor start");
		#endif

		if ( IsMissionHost() )
		{
			CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			EnableDynamicCCD(true);
		}

		m_FlareTimer = new Timer();
		m_FlareTimer.Run(0.5, this, "SpawnLight", NULL, false);

		m_FlareUpdateTimer = new Timer();
		m_FlarePhysicsTimer = new Timer();

		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE);

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] Constructor end");
		#endif
	}

	void ~ExpansionRoadflare()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] Deconstructor start");
		#endif

		if (m_FlareTimer)
		{
			m_FlareTimer.Stop();
			delete m_FlareTimer;
		}

		if (m_FlareLight)
		{
			GetGame().ObjectDelete(m_FlareLight);
		}

		if (m_FlareSmoke)
		{
			m_FlareSmoke.Stop();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] Deconstructor end");
		#endif
	}

	void SpawnLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] SpawnLight start");
		#endif
		
		if ( IsMissionClient() )
		{
			m_FlareSmoke = Particle.PlayOnObject(ParticleList.EXPANSION_FLARE_SMOKE, this, "0 1 0");

			if (m_Light)
			{
				GetGame().ObjectDelete(m_Light);
			}

			if (!m_FlareLight)
			{
				m_FlareLight = ExpansionPointLight.Cast( ScriptedLightBase.CreateLight( ExpansionPointLight, Vector(0,0,0) )  ); 

				m_FlareLight.SetDiffuseColor(1, 0.1, 0.1);

				m_FlareLight.SetPosition(GetPosition());
				m_FlareLight.SetRadiusTo(300);
				m_FlareLight.SetBrightnessTo(0.85); 
				m_FlareLight.AttachOnObject(this, "0 1 0");
			}
		}

		SetOrientation("0 0 0");

		m_FlareUpdateTimer.Run(0.01, this, "UpdateLight", NULL, true);
		m_FlarePhysicsTimer.Run(5, this, "UpdatePhysics", NULL, true);

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] SpawnLight end");
		#endif
	}

	void UpdatePhysics()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] UpdatePhysics start");
		#endif

		if ( IsMissionHost() ) 
		{
			if (!IsGround(5)) 
			{	
				if (GetVelocity(this).Length() >= 0) 
				{
					CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
					EnableDynamicCCD(true);

					SetOrientation(Vector(GetOrientation()[0], 0, 0));
				}
			}			
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] UpdatePhysics end");
		#endif
	}

	void UpdateLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] UpdateLight start");
		#endif

		if (!IsGround(5)) 
		{
			SetVelocity(this, Vector(0, -0.1, 0));
		}
		else 
		{
			GetGame().ObjectDelete(this);
		}

		if (m_FlareLight)
		{
			m_FlareLight.SetPosition( GetPosition() );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] UpdateLight end");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion IsGround
	// ------------------------------------------------------------
	private bool IsGround(float height)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] IsGround start");
		#endif

		//! Ray input
		vector m_Start = this.GetPosition();
		vector m_End = this.GetPosition() - Vector(0, height, 0);
		
		//! Ray output
		vector m_Hit;
		vector m_HitPos;
		
		//! Ray hitindex output
		int m_HitIndex;

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionRoadflare] IsGround end");
		#endif

		//! Ray
		return DayZPhysics.RaycastRV(m_Start, m_End, m_HitPos, m_Hit, m_HitIndex, NULL, NULL, this);
	}
}