/**
 * ExpansionLampLightBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLampLightBase_Lamp: House
{
	protected ScriptedLightBase m_Expansion_Light;

	protected bool m_Expansion_CastShadows; //! Client setting

	protected int m_Expansion_LampColor_R;
	protected int m_Expansion_LampColor_G;
	protected int m_Expansion_LampColor_B;

	void ExpansionLampLightBase_Lamp()
	{
		RegisterNetSyncVariableInt("m_Expansion_LampColor_R", 0, 255);
		RegisterNetSyncVariableInt("m_Expansion_LampColor_G", 0, 255);
		RegisterNetSyncVariableInt("m_Expansion_LampColor_B", 0, 255);

	#ifndef SERVER
		ExpansionSettings.SI_General.Insert(Expansion_OnClientSettingsUpdated);
		GetExpansionClientSettings().SI_UpdateSetting.Insert(Expansion_OnClientSettingsUpdated);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_OnClientSettingsUpdated, 1000);
	#endif
	}

	void ~ExpansionLampLightBase_Lamp()
	{
		if (!GetGame())
			return;

	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTS, this);
	#endif

	#ifndef SERVER
		ExpansionSettings.SI_General.Remove(Expansion_OnClientSettingsUpdated);
		GetExpansionClientSettings().SI_UpdateSetting.Remove(Expansion_OnClientSettingsUpdated);

		if (m_Expansion_Light)
			GetGame().ObjectDelete(m_Expansion_Light);
	#endif
	}

	override void AfterStoreLoad()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTS, this);
	#endif
		
		super.AfterStoreLoad();

		Delete();
	}

	void Expansion_SetLampColor(vector color)
	{
		m_Expansion_LampColor_R = color[0] * 255;
		m_Expansion_LampColor_G = color[1] * 255;
		m_Expansion_LampColor_B = color[2] * 255;

		SetSynchDirty();
	}

	void Expansion_OnClientSettingsUpdated()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTS, this);
	#endif

		m_Expansion_CastShadows = GetExpansionClientSettings().CastLightShadows;

	#ifdef EXTRACE
		EXTrace.Add(trace, m_Expansion_CastShadows);
	#endif

		Expansion_OnEnable();
	}

	protected void Expansion_OnEnable()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTS, this);
	#endif

		if (!Expansion_IsEnabled() && !m_Expansion_Light)
		{
			m_Expansion_Light = ScriptedLightBase.CreateLight(ExpansionStreetLight, "0 0 0");
			m_Expansion_Light.AttachOnObject(this, "0 0 0", "0 -90 0");

			m_Expansion_Light.SetDiffuseColor(m_Expansion_LampColor_R / 255, m_Expansion_LampColor_G / 255, m_Expansion_LampColor_B / 255);
			m_Expansion_Light.SetAmbientColor(m_Expansion_LampColor_R / 255, m_Expansion_LampColor_G / 255, m_Expansion_LampColor_B / 255);
			m_Expansion_Light.FadeIn(0.3);
			m_Expansion_Light.SetFlareVisible(true);
			m_Expansion_Light.SetEnabled(true);
		}

		if (Expansion_IsEnabled())
			m_Expansion_Light.SetCastShadow(m_Expansion_CastShadows);
	}

	void Expansion_OnDisable()
	{
		if (m_Expansion_Light)
		{
			m_Expansion_Light.FadeOut(0.3);
			m_Expansion_Light.SetEnabled(false);
		}
	}

	bool Expansion_IsEnabled()
	{
		if (m_Expansion_Light)
			return m_Expansion_Light.IsEnabled();

		return false;
	}
}

/**@class		ExpansionLampLightBase
 * @brief		
 **/
modded class ExpansionLampLightBase
{
	static ref array<ExpansionLampLightBase> s_Lamps = {};

	protected ExpansionLampLightBase_Lamp m_Lamp;

	protected bool m_CanBeEnabled; //! Setting
	protected bool m_ShouldBeEnabled; //! Setting

	protected vector m_LampColor;
	protected vector m_LampPosition;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionLampLightBase()
	{
		m_LampColor = ConfigGetString("color").ToVector();
		m_LampPosition = ConfigGetString("position").ToVector();

		if (GetGame().IsServer())
			s_Lamps.Insert(this);
	}

	void LateInit()
	{
		if (GetExpansionSettings().GetGeneral(false).IsLoaded())
			OnSettingsUpdated();

		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
		ExpansionWorldObjectsModule.SI_LampEnable.Insert( Enable );
		ExpansionWorldObjectsModule.SI_LampDisable.Insert( Disable );

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.LIGHTS, this, string.Format("%1 %2", GetDebugName(), GetPosition().ToString(false)));
	#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionLampLightBase()
	{
		if (!g_Game)
			return;

		int index = s_Lamps.Find(this);
		if (index != -1)
		{
			s_Lamps.Remove(index);
			ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
			ExpansionWorldObjectsModule.SI_LampEnable.Remove( Enable );
			ExpansionWorldObjectsModule.SI_LampDisable.Remove( Disable );
		}
	}

	static void LateInitLamps()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTS, ExpansionLampLightBase);
	#endif

		auto settings = GetExpansionSettings().GetGeneral();

		int n = settings.LampAmount_OneInX;

		if (n <= 0)
		{
			EXError.Warn(null, string.Format("GeneralSettings: Invalid LampAmount_OneInX value %1 (must be >= 1). Disabling lamps.", n), {});
			return;
		}

		ExpansionLampSelectionMode lampSelectionMode;

		if (!ExpansionStatic.StringToEnumEx(ExpansionLampSelectionMode, settings.LampSelectionMode, lampSelectionMode))
		{
			TStringArray modes = {};

			typename e = ExpansionLampSelectionMode;
			int count = e.GetVariableCount();

			for (int i = 0; i < count; ++i)
			{
				if (e.GetVariableType(i) == int)
					modes.Insert(e.GetVariableName(i));
			}

			EXError.Warn(null, string.Format("GeneralSettings: Invalid LampSelectionMode value %1 (must be one of [%2]). Disabling lamps.", settings.LampSelectionMode, ExpansionString.JoinStrings(modes)), {});
			return;
		}

		int amount = s_Lamps.Count() / n;

		if (amount > 0)
		{
			if (amount < s_Lamps.Count())
			{
				switch (lampSelectionMode)
				{
					case ExpansionLampSelectionMode.RANDOM:
						SelectRandomLamps(amount);
						break;

					case ExpansionLampSelectionMode.FARTHEST:
						SelectFarthestLamps_Optimized(amount, false);
						break;

					case ExpansionLampSelectionMode.FARTHEST_RANDOM:
						SelectFarthestLamps_Optimized(amount, true);
						break;
				}
			}
			else
			{
				SelectAllLamps();
			}
		}
		else
		{
			s_Lamps.Clear();
		}
	}

	static void SelectRandomLamps(int amount)
	{
		array<ExpansionLampLightBase> selected = {};
		
		while (selected.Count() < amount)
		{
			int index = s_Lamps.GetRandomIndex();
			ExpansionLampLightBase lamp = s_Lamps[index];
			selected.Insert(lamp);
			s_Lamps.Remove(index);
			lamp.LateInit();
		}

		s_Lamps.Copy(selected);
	}

	//! WARNING: Quadratic runtime. Leaving this here as an example of what NOT to do.
	//! NEVER use this one. Use optimized version (below) instead.
	static void SelectFarthestLamps_Simple(int amount, bool randomize = true)
	{
		if (s_Lamps.Count() == 0 || amount <= 0)
			return;

		array<ExpansionLampLightBase> selected = {};

		int startingIndex;

		//! Pick an arbitrary starting point
		if (randomize)
			startingIndex = s_Lamps.GetRandomIndex();

		selected.Insert(s_Lamps[startingIndex]);
		s_Lamps.Remove(startingIndex);

		while (selected.Count() < amount && s_Lamps.Count() > 0)
		{
			float bestDistSq = -1;
			int bestIndex;
			ExpansionLampLightBase bestPick;

			//! For each candidate, compute distance to nearest selected point
			foreach (int i, ExpansionLampLightBase candidate: s_Lamps)
			{
				float minDistSq = float.MAX;
				foreach (ExpansionLampLightBase chosen: selected)
				{
					float distSq = vector.DistanceSq(candidate.GetPosition(), chosen.GetPosition());

					if (distSq < minDistSq)
						minDistSq = distSq;
				}

				//! Keep the farthest
				if (minDistSq > bestDistSq)
				{
					bestDistSq = minDistSq;
					bestIndex = i;
					bestPick = candidate;
				}
			}

			selected.Insert(bestPick);
			s_Lamps.Remove(bestIndex);
			bestPick.LateInit();
		}

		s_Lamps.Copy(selected);
	}

	//! Nice and (comparatively) fast. About 66 times faster than simple version (above).
	//! ALWAYS use this one.
	static void SelectFarthestLamps_Optimized(int amount, bool randomize = true)
	{
		if (s_Lamps.Count() == 0 || amount <= 0)
			return;

		array<ExpansionLampLightBase> selected = {};

		array<float> minDistSqCache = {};

		int startingIndex;

		//! Pick an arbitrary starting point
		if (randomize)
			startingIndex = s_Lamps.GetRandomIndex();

		ExpansionLampLightBase start = s_Lamps[startingIndex];
		selected.Insert(start);
		s_Lamps.Remove(startingIndex);

		float distSq;
		int i;

		//! Initialize cache
		vector startPos = start.GetPosition();
		foreach (ExpansionLampLightBase lamp: s_Lamps)
		{
			distSq = vector.DistanceSq(lamp.GetPosition(), startPos);
			minDistSqCache.Insert(distSq);
		}

		while (selected.Count() < amount && s_Lamps.Count() > 0)
		{
			float bestDistSq = -1;
			int bestIndex = -1;

			//! Find the candidate with max cached distance
			for (i = 0; i < minDistSqCache.Count(); ++i)
			{
				distSq = minDistSqCache[i];

				if (distSq > bestDistSq)
				{
					bestDistSq = distSq;
					bestIndex = i;
				}
			}

			//! Select the best pick
			ExpansionLampLightBase bestPick = s_Lamps[bestIndex];
			selected.Insert(bestPick);
			s_Lamps.Remove(bestIndex);
			minDistSqCache.Remove(bestIndex);
			bestPick.LateInit();

			//! Update cache
			vector pos = bestPick.GetPosition();
			for (i = 0; i < s_Lamps.Count(); ++i)
			{
				distSq = vector.DistanceSq(s_Lamps[i].GetPosition(), pos);

				if (distSq < minDistSqCache[i])
					minDistSqCache[i] = distSq;
			}
		}

		s_Lamps.Copy(selected);
	}

	static void SelectAllLamps()
	{
		foreach (ExpansionLampLightBase lamp: s_Lamps)
		{
			lamp.LateInit();
		}
	}

	void DeleteLamp()
	{
		if (m_Lamp)
			GetGame().ObjectDelete(m_Lamp);
	}

	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	void OnSettingsUpdated()
	{
		if (GetGame().IsServer())
		{
			if (ExpansionWorldObjectsModule.s_RemovedObjects[this])
				return;

			//! https://feedback.bistudio.com/T173348
			if (GetExpansionSettings().GetGeneral().EnableLamps != LampModeEnum.Disabled)
				m_CanBeEnabled = true;
			else
				m_CanBeEnabled = false;
	
			if (m_CanBeEnabled)
				OnEnable();
			else
				OnDisable();
		}
	}

	// ------------------------------------------------------------
	// GetLampPosition
	// ------------------------------------------------------------
	vector GetLampPosition()
	{
		return m_LampPosition;
	}

	// ------------------------------------------------------------
	// GetLampColor
	// ------------------------------------------------------------
	vector GetLampColor()
	{
		return m_LampColor;
	}

	protected bool LampInGenerator( vector generator )
	{
		return vector.DistanceSq(GetPosition(), generator) <= 500 * 500;
	}

	// ------------------------------------------------------------
	// Expansion Enable
	// ------------------------------------------------------------
	void Enable( vector position )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTS, this, "Enable");
#endif

		if (GetExpansionSettings().GetGeneral().EnableLamps != LampModeEnum.AlwaysOnEverywhere && !LampInGenerator( position ))
		{
			return;
		}

		m_ShouldBeEnabled = true;
		
		OnEnable();
	}
	
	// ------------------------------------------------------------
	// Expansion OnEnable
	// ------------------------------------------------------------
	protected void OnEnable()
	{
		if (GetGame().IsServer() && !IsEnabled() && !m_Lamp && m_CanBeEnabled)
		{
			vector modelPos;

			if (MemoryPointExists("light"))
				modelPos = GetMemoryPointPos("light");
			else
				modelPos = m_LampPosition;

			int flags = ECE_NOLIFETIME | ECE_KEEPHEIGHT | ECE_NOPERSISTENCY_WORLD;

			Object obj = GetGame().CreateObjectEx("ExpansionLampLightBase_Lamp", ModelToWorld(modelPos), flags);

			if (Class.CastTo(m_Lamp, obj))
			{
				m_Lamp.SetOrientation("0 0 0");
				m_Lamp.Expansion_SetLampColor(m_LampColor);
			}
			else
			{
				GetGame().ObjectDelete(obj);
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void ExpansionAttachOnMemoryPoint(string memory_point_start, vector memory_point_target = "0 0 0")
	{
		Error("DEPRECATED");
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void ExpansionAttachOnObject(vector memory_point_start, vector memory_point_target = "0 0 0")
	{
		Error("DEPRECATED");
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void Disable( vector position )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTS, this, "Disable");
#endif

		if (GetExpansionSettings().GetGeneral().EnableLamps != LampModeEnum.AlwaysOnEverywhere && !LampInGenerator( position ))
		{
			return;
		}

		m_ShouldBeEnabled = false;

		OnDisable();
	}
	
	// ------------------------------------------------------------
	// Expansion OnDisable
	// ------------------------------------------------------------
	protected void OnDisable()
	{
		DeleteLamp();
	}

	// ------------------------------------------------------------
	// Expansion IsEnabled
	// ------------------------------------------------------------
	bool IsEnabled()
	{
		return (m_Lamp && m_Lamp.Expansion_IsEnabled());
	}
	
	// ------------------------------------------------------------
	// Expansion CanBeEnabled
	// ------------------------------------------------------------
	bool CanBeEnabled()
	{
		return m_CanBeEnabled;
	}	
};