modded class DayZGame
{
	void DayZGame()
	{
		TStringArray supportedGraphNames = {
			"dz\\anims\\workspaces\\player\\player_main\\player_main.agr",  //! Vanilla
			"DayZExpansion\\Animations\\Player\\player_main.agr"  //! Expansion Animations
		};

		TStringArray supportedDefaultInstances = {
			"dz\\anims\\workspaces\\player\\player_main\\player_main.asi",  //! Vanilla
			"DayZExpansion\\Animations\\Player\\player_main.asi"  //! Expansion Animations
		};

		bool supported;

		string graphName;
		string defaultInstance;

		ConfigGetText(CFG_VEHICLESPATH + " SurvivorBase enfAnimSys graphName", graphName);
		ConfigGetText(CFG_VEHICLESPATH + " SurvivorBase enfAnimSys defaultInstance", defaultInstance);

		FileHandle file = OpenFile(graphName, FileMode.READ);

		int max;

		if (file)
		{
			string line;
			bool isInsideVehicleTypeDef;

			while (FGets(file, line) >= 0)
			{
				string trimmed = line.Trim();

			#ifdef DAYZ_1_29
				if (trimmed.IndexOf("#Var VehicleType ") == 0)
			#else
				//! 1.30+
				if (trimmed.IndexOf("AnimSrcGCTVarInt VehicleType ") == 0)
					isInsideVehicleTypeDef = true;
				else if (trimmed == "}")
					isInsideVehicleTypeDef = false;

				if (isInsideVehicleTypeDef && trimmed.IndexOf("MaxValue ") == 0)
			#endif
				{
					trimmed.Replace("\t", " ");

					TStringArray split = {};
					trimmed.Split(" ", split);

				#ifdef DAYZ_1_29
					max = split[5].ToInt();

					//! Vanilla 1.29 animgraph VehicleType max
					if (max == 10)
						supported = true;
				#else
					//! 1.30+
					max = split[1].ToInt();

					//! Vanilla 1.30 animgraph VehicleType max
					if (max == 12)
						supported = true;
				#endif

					//! Expansion Animations modded animgraph VehicleType max
					if (max == 100)
						supported = true;

					break;
				}
			}
		}
		else
		{
			EXError.Warn(null, "Couldn't open animation graph: " + graphName, {});
		}

		string warnTitle = "Potentially unsupported modded animation graph/instance for Expansion AI";
		string warnText = "If you notice AI stuck in specific poses, this could be the reason.";

		if (supportedGraphNames.Find(graphName) == -1)
			EXError.Warn(null, warnTitle + ": " + graphName + "\n" + warnText, {});
		else if (!supported)
			EXError.Warn(null, warnTitle + " (VehicleType max " + max + "): " + graphName + "\n" + warnText, {});

		if (supportedDefaultInstances.Find(defaultInstance) == -1)
			EXError.Warn(null, warnTitle + ": " + defaultInstance + "\n" + warnText, {});

	#ifdef DAYZ_1_29
		EXError.InfoOnce(null, "DayZ 1.29 is not compatible with the 1.30 AI animgraph");
	#endif
	}

	override void ExplosionEffectsEx(Object source, Object directHit, int componentIndex, float energyFactor, float explosionFactor, HitInfo hitInfo)
	{
		super.ExplosionEffectsEx(source, directHit, componentIndex, energyFactor, explosionFactor, hitInfo);
		
		if (IsServer())
		{
			eAINoiseSystem.AddNoise(hitInfo.GetPosition(), 21.0, string.Format("cfgAmmo %1 NoiseExplosion", hitInfo.GetAmmoType()), hitInfo.GetSurfaceNoiseMultiplier(), eAINoiseType.EXPLOSION);
		}
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
		GetExpansionGame().FixAIFirearmFX(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
	}

	override void CloseCombatEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, bool isWater, string ammoType) 
	{
		super.CloseCombatEffects(source, directHit, componentIndex, surface, pos, surfNormal,
		 isWater, ammoType);

		if (IsServer())
		{
			float surfaceCoef = SurfaceGetNoiseMultiplier(directHit, pos, componentIndex);
			if (surfaceCoef == 0)
				surfaceCoef = 1;
		#ifdef DAYZ_1_29
			surfaceCoef *= GetWeather().GetNoiseReductionByWeather();
		#else
			//! 1.30
			surfaceCoef *= eAI_GetNoiseReductionByWeatherEx(source);
		#endif
			eAINoiseSystem.AddNoise(EntityAI.Cast(source), pos, string.Format("cfgAmmo %1 NoiseHit", ammoType), surfaceCoef);
		}
	}

	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (IsServer() && m_ExpansionGame)
			m_ExpansionGame.eAI_OnUpdate(doSim, timeslice);
	}

	override bool IsDebugMonitor()
	{
		//! m_DebugMonitorEnabled == 2 is for server stats (only for admins listed in AISettings)
		return IsServer() && m_DebugMonitorEnabled == 1;
	}

#ifndef DAYZ_1_29
	//! Vanilla Weather::GetNoiseReductionByWeatherEx method introduced in 1.30 Experimental is already marked obsolete,
	//! so we roll our own (verbatim copy)
	float eAI_GetNoiseReductionByWeatherEx(notnull Object object)
	{
		Weather weather = GetWeather();

		float rainReduction = weather.GetRain().GetActual() * GameConstants.RAIN_NOISE_REDUCTION_WEIGHT;
		float snowfallReduction = weather.GetSnowfall().GetActual() * GameConstants.SNOWFALL_NOISE_REDUCTION_WEIGHT;
		float sandstormReduction;

		SandstormController sandstormController = weather.GetSandstorm();
		if (sandstormController.IsActive())
		{
			Man player = Man.Cast(object);
			if (player)
				sandstormReduction = sandstormController.GetIntensityForPlayer(player) * GameConstants.SANDSTORM_NOISE_REDUCTION_WEIGHT;
			else
				sandstormReduction = sandstormController.GetIntensity(object.GetPosition()) * GameConstants.SANDSTORM_NOISE_REDUCTION_WEIGHT;
		}
		
		// If all reductions are zero, return 1 (no reduction)
		if (rainReduction == 0 && snowfallReduction == 0 && sandstormReduction == 0)
			return 1;
		
		float maxReduction = Math.Max(rainReduction, Math.Max(snowfallReduction, sandstormReduction));
		return 1 - maxReduction;
	}
#endif
}
