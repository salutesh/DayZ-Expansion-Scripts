#ifndef EXPANSIONMODVEHICLE
CarScript g_Expansion_Car;
#endif

modded class CarScript
{
	string m_eAI_NoiseParamsPath;

	ref map<int, Human> m_Expansion_ReservedSeats = new map<int, Human>;
	
	override void EEInit()
	{		
		super.EEInit();

		if (GetGame().IsServer())
		{
			m_eAI_NoiseParamsPath = "cfgVehicles " + GetType() + " NoiseCarHorn";
		}
	}

	override protected void GenerateCarHornAINoise(int pState)
	{
		super.GenerateCarHornAINoise(pState);

		if (pState != ECarHornState.OFF)
		{
			float noiseMultiplier = 1.0;
			if (pState == ECarHornState.LONG)
				noiseMultiplier = 2.0;

			eAINoiseSystem.AddNoise(GetPosition(), 5.0, m_eAI_NoiseParamsPath, noiseMultiplier);
		}
	}

	bool Expansion_IsSeatReservedByOther(int posIdx, Human crewMember)
	{
		Human reserved = m_Expansion_ReservedSeats[posIdx];

		if (!reserved || (!reserved.IsAlive() && !CrewMember(posIdx)))
			return false;

		return reserved != crewMember;
	}

	void Expansion_ReserveSeat(int posIdx, Human crewMember)
	{
		m_Expansion_ReservedSeats[posIdx] = crewMember;
	}
}
