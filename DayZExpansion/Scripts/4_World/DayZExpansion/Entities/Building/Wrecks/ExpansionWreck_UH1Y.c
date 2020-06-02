/**
 * ExpansionWreck_UH1Y.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*
modded class Wreck_UH1Y
{
	Particle m_ParticleEfx;
	int m_ServerMarker;
	
	protected ExpansionMapMarkerModule m_MarkerModule;

	// ------------------------------------------------------------
	// Wreck_UH1Y Constructor
	// ------------------------------------------------------------
	void Wreck_UH1Y()
	{
		m_MarkerModule = ExpansionMapMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMapMarkerModule ) );
		
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.SMOKING_HELI_WRECK, this, Vector(-0.5, 0, -1.0));
		}
	}
	
	// ------------------------------------------------------------
	// Override EEDelete
	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( m_ParticleEfx )
				m_ParticleEfx.Stop();
		}
	}
}
*/