/**
 * ExpansionParticles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ParticleList
{
	//! Explosions
	static const int EXPANSION_EXPLOSION_HELICOPTER = RegisterParticle( "DayZExpansion/Particles/Explosions/" , "Expansion_Explosion_Vehicle");
	static const int EXPANSION_EXPLOSION_ROCKET = RegisterParticle( "DayZExpansion/Particles/Explosions/" , "Expansion_Explosion_Rocket");
	static const int EXPANSION_EXPLOSION_WATER = RegisterParticle( "DayZExpansion/Particles/Explosions/" , "Expansion_Explosion_Water");
	static const int EXPANSION_EXPLOSION_FLARE = RegisterParticle( "DayZExpansion/Particles/Explosions/" , "Expansion_Explosion_Flare");
	static const int EXPANSION_FIRE_HELICOPTER = RegisterParticle( "DayZExpansion/Particles/Explosions/" , "Expansion_Fire_Vehicle");

	//! Boats
	static const int EXPANSION_BOAT_WATER = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Boat_Water");
	static const int EXPANSION_BOAT_DECAL = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Boat_Decal");
	static const int EXPANSION_BOAT_ENGINE = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Boat_Engine");
	
	//! Helicopters
	static const int EXPANSION_HELICOPTER_WATER = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Helicopter_Water");
	static const int EXPANSION_HELICOPTER_GROUND = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Helicopter_Ground");
	static const int EXPANSION_HELICOPTER_EXHAUST = RegisterParticle( "DayZExpansion/Particles/Vehicles/" , "Expansion_Helicopter_Exhaust");
	
	// Weapons
	static const int EXPANSION_FLARE_SMOKE = RegisterParticle( "DayZExpansion/Particles/Weapons/" , "Expansion_World_Flare");
	static const int EXPANSION_ROCKET_SMOKE = RegisterParticle( "DayZExpansion/Particles/Weapons/" , "Expansion_World_Rocket");
	static const int EXPANSION_ROCKET_DUST = RegisterParticle( "DayZExpansion/Particles/Weapons/" , "Expansion_World_Dust");

	//! Airdrop
	static const int EXPANSION_AIRDROP_SMOKE = RegisterParticle( "DayZExpansion/Particles/Airdrop/" , "Expansion_Airdrop");

	//! World
	static const int EXPANSION_CLOUD = RegisterParticle( "DayZExpansion/Particles/World/" , "Expansion_World_Cloud");
	static const int EXPANSION_FOG = RegisterParticle( "DayZExpansion/Particles/World/" , "Expansion_World_Fog");

	//! Lights
	static const int EXPANSION_LIGHT_BLUE = RegisterParticle( "DayZExpansion/Particles/Lights/" , "Expansion_World_Light_Blue");
	static const int EXPANSION_LIGHT_RED = RegisterParticle( "DayZExpansion/Particles/Lights/" , "Expansion_World_Light_Red");
	static const int EXPANSION_LIGHT_WHITE = RegisterParticle( "DayZExpansion/Particles/Lights/" , "Expansion_World_Light_White");
	static const int EXPANSION_LIGHT_YELLOW = RegisterParticle( "DayZExpansion/Particles/Lights/" , "Expansion_World_Light_Yellow");

	//! Misc
	static const int EXPANSION_PROPANE_FLAME = RegisterParticle( "DayZExpansion/Particles/Misc/" , "Expansion_Propane_Flame");
};