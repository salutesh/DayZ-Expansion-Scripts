class ExpansionAirdropContainerFX
{
	EntityAI m_Parent;

	ExpansionPointLight m_Light;
	Particle m_Particle;

	void ExpansionAirdropContainerFX(EntityAI parent)
	{
		m_Parent = parent;
	}

	void ~ExpansionAirdropContainerFX()
	{
		if (g_Game)
			Destroy();
	}

	void Destroy()
	{
		if (m_Light)
			m_Light.Destroy();

		if (m_Particle)
		{
			m_Particle.Stop();
			m_Particle = null;
		}
	}

	void SetupLight()
	{
		m_Light.SetDiffuseColor(1, 0.1, 0.1);
		m_Light.SetRadiusTo(20);
		m_Light.SetBrightnessTo(0.6);
		m_Light.SetFlareVisible(true);
	}

	void Enable(bool enable)
	{
		if (!g_Game.IsDedicatedServer())
		{
			if (enable)
			{
				if (!m_Light)
				{
					m_Light = ExpansionPointLight.Cast(ScriptedLightBase.CreateLight(ExpansionPointLight));

					SetupLight();

					m_Light.AttachOnObject(m_Parent, m_Parent.GetMemoryPointPos("light"));
				}

				if (!m_Particle)
					m_Particle = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, m_Parent, m_Parent.GetMemoryPointPos("light"));
			}
			else
			{
				if (m_Light)
				{
					m_Light.FadeOut();
					m_Light = null;
				}

				if (m_Particle)
				{
					m_Particle.Stop();
					m_Particle = null;
				}
			}
		}
	}
}
