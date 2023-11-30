modded class AlarmClock_ColorBase
{
	override void OnUpdate()
	{
		super.OnUpdate();

		if (IsRinging())
		{
			eAINoiseSystem.AddNoise(GetPosition(), UPDATE_TICK_RATE, "cfgVehicles " + GetType() + " NoiseAlarmClock");
		}
	}
}

modded class KitchenTimer
{
	override void OnUpdate()
	{
		super.OnUpdate();

		if (IsRinging())
		{
			eAINoiseSystem.AddNoise(GetPosition(), UPDATE_TICK_RATE, "cfgVehicles " + GetType() + " NoiseKitchenTimer");
		}
	}
}
