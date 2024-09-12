/**
 * Land_Lighthouse.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Lighthouse_Lamp: House
{
	protected ScriptedLightBase m_Expansion_Light_Inner;
	protected ScriptedLightBase m_Expansion_Light_Outer;

	protected bool m_Expansion_CastShadows; // setting

	protected float m_Expansion_LastRotationUpdateTime;
	protected float m_Expansion_RotationAngle;

	void Expansion_Lighthouse_Lamp()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 
		
		if (GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_Rotate, 25, true);
	
	#ifndef SERVER
		GetExpansionClientSettings().SI_UpdateSetting.Insert(Expansion_OnClientSettingsUpdated);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_OnClientSettingsUpdated, 1000);
	#endif
	}

	void ~Expansion_Lighthouse_Lamp()
	{
		if (!GetGame())
			return;

#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 
		
	#ifndef SERVER
		GetExpansionClientSettings().SI_UpdateSetting.Remove(Expansion_OnClientSettingsUpdated);

		if (m_Expansion_Light_Inner)
			GetGame().ObjectDelete(m_Expansion_Light_Inner);

		if (m_Expansion_Light_Outer)
			GetGame().ObjectDelete(m_Expansion_Light_Outer);
	#endif
	}

	override void AfterStoreLoad()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 
		
		super.AfterStoreLoad();

		Delete();
	}

	void Expansion_Rotate()
	{
		float tickTime = GetGame().GetTickTime();
		float timeSlice = tickTime - m_Expansion_LastRotationUpdateTime;
		m_Expansion_LastRotationUpdateTime = tickTime;

		m_Expansion_RotationAngle += 25 * timeSlice;
		if (m_Expansion_RotationAngle >= 360)
			m_Expansion_RotationAngle -= 360;

		SetOrientation(Vector(m_Expansion_RotationAngle, 0, 0));
	}
	
	void Expansion_OnClientSettingsUpdated()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 

	#ifndef SERVER
		m_Expansion_CastShadows = GetExpansionClientSettings().CastLightShadows;
		
		Expansion_OnEnable();
	#endif
	}

	void Expansion_Enable(vector position)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this, "" + position);
#endif 
		
		Expansion_OnEnable();
	}

	protected void Expansion_OnEnable()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 

	#ifndef SERVER
		if (!Expansion_IsEnabled())
		{
			if (!m_Expansion_Light_Inner)
			{
				m_Expansion_Light_Inner = ScriptedLightBase.CreateLight(ExpansionLighthouseLampLight_Inner, "0 0 0");
				m_Expansion_Light_Inner.AttachOnObject(this, "0 0.5 0.5", "0 0 0");
			}

			if (!m_Expansion_Light_Outer)
			{
				m_Expansion_Light_Outer = ScriptedLightBase.CreateLight(ExpansionLighthouseLampLight_Outer, "0 0 0");
				m_Expansion_Light_Outer.AttachOnObject(this, "0 0.5 1.5", "0 0 0");
			}

			m_Expansion_Light_Inner.SetEnabled(true);
			m_Expansion_Light_Inner.FadeIn(0.3);
			m_Expansion_Light_Outer.SetEnabled(true);
			m_Expansion_Light_Outer.FadeIn(0.3);
		}

		//! Lighthouse light shadows disabled, see https://feedback.bistudio.com/T179455
		//! DayZ 1.24: Lighthouse light shadows CTDs client if lighthouse light is behind camera at certain angle/distance
		//! DayZ 1.25: Got even worse
		//if (Expansion_IsEnabled())
			//m_Expansion_Light_Outer.SetCastShadow(m_Expansion_CastShadows);
	#endif
	}

	void Expansion_Disable( vector position )
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this, "" + position);
#endif 

		Expansion_OnDisable();
	}

	void Expansion_OnDisable()
	{
	#ifndef SERVER
		if (m_Expansion_Light_Inner)
		{
			m_Expansion_Light_Inner.FadeOut(0.3);
			m_Expansion_Light_Inner.SetEnabled(false);
		}

		if (m_Expansion_Light_Outer)
		{
			m_Expansion_Light_Outer.FadeOut(0.3);
			m_Expansion_Light_Outer.SetEnabled(false);
		}
	#endif
	}

	bool Expansion_IsEnabled()
	{
		if (m_Expansion_Light_Outer)
			return m_Expansion_Light_Outer.IsEnabled();

		return false;
	}
}

class Land_Lighthouse: House
{
	protected Object m_Expansion_Lighthouse_Lamp;

	void ~Land_Lighthouse()
	{
		if (!GetGame())
			return;

#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 

		//! @note destructor runs on server even if lighthouse object is not destroyed?!?
		//Expansion_DeleteLamp();
	}

	void Expansion_DeleteLamp()
	{
		if (m_Expansion_Lighthouse_Lamp)
			GetGame().ObjectDelete(m_Expansion_Lighthouse_Lamp);
	}

	override void OnSettingsUpdated()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.LIGHTHOUSE, this);
#endif 

		super.OnSettingsUpdated();

		if (!GetGame().IsServer())
			return;

		if (ExpansionWorldObjectsModule.s_RemovedObjects[this])
			return;

		if (GetExpansionSettings().GetGeneral().EnableLighthouses)
		{
			if (m_Expansion_Lighthouse_Lamp)
				return;

			vector pos = ModelToWorld(GetMemoryPointPos("cerveny pozicni blik")) - "0 4.5 0";
			int flags = ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_KEEPHEIGHT | ECE_NOPERSISTENCY_WORLD;

			m_Expansion_Lighthouse_Lamp = GetGame().CreateObjectEx("Expansion_Lighthouse_Lamp", pos, flags);
			m_Expansion_Lighthouse_Lamp.SetFlags(EntityFlags.STATIC, false);

			//GetGame().CreateObjectEx("Apple", ModelToWorld(GetMemoryPointPos("cerveny pozicni blik")), flags);
		}
		else
		{
			Expansion_DeleteLamp();
		}
	}
};
