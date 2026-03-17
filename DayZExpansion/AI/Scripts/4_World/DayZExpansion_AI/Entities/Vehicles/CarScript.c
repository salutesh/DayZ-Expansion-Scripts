#ifndef EXPANSIONMODVEHICLE
CarScript g_Expansion_Car;
#endif

modded class CarScript
{
	string m_eAI_NoiseParamsPath;
	
	override void EEInit()
	{		
		super.EEInit();

		if (g_Game.IsServer())
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
}
