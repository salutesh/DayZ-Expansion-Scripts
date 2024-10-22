/**
 * StaticObj_Lamp_Harbour.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class StaticObj_Lamp_Harbour extends House 
{
    protected ScriptedLightBase m_Light;
    protected bool m_LightState;
	protected bool m_LightStateClient;

    void StaticObj_Lamp_Harbour()
    {
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
        RegisterNetSyncVariableBool("m_LightState");
    }
	
	void SetLightState(bool state)
	{
		m_LightState = state;
		
		SetSynchDirty();
	}

    void SpawnLight()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		m_Light = ScriptedLightBase.CreateLight(ExpansionStreetLight, GetPosition());
        m_Light.SetDiffuseColor(1.0, 0.7, 0.4);
        m_Light.SetAmbientColor(1.0, 0.7, 0.4);
        m_Light.FadeIn(0.3);
        m_Light.SetCastShadow(true);
        m_Light.SetFlareVisible(true);
        m_Light.SetEnabled(true);
		
		if (MemoryPointExists("light"))
		{
			AttachOnMemoryPoint("light", "0 -1 0");
		}
		else
		{		
			m_Light.AttachOnObject(this, "0.279880 3.047814 0.003933");
		}
	}

	void AttachOnMemoryPoint(string memory_point_start, vector memory_point_target = "0 0 0")
	{
		vector localPos = GetMemoryPointPos(memory_point_start);
		vector localOri;
		if (memory_point_target != vector.Zero)
		{
			vector targetPos = vector.Direction(localPos, memory_point_target);
			localOri = targetPos.VectorToAngles();
		}
		
		m_Light.AttachOnObject(this, localPos, localOri);
	}
	
	protected void UpdateLightState(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (m_LightState)
		{
			if (!m_Light)
				SpawnLight();
		}
		else
		{
			if (m_Light)
			{
				m_Light.SetEnabled(false);
				m_Light.Destroy();
			}
		}
		
		m_LightStateClient = m_LightState;
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_LightState != m_LightStateClient)
			UpdateLightState(m_LightState);
	}
};