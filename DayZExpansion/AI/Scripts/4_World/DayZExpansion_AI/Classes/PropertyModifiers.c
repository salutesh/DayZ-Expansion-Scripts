modded class PropertyModifiers
{
	protected float m_eAI_NoiseShootModifier;

	override void UpdateModifiers()
	{
		super.UpdateModifiers();

		m_eAI_NoiseShootModifier = 1.0;

		//! Bit ugly to have to do this again, but fine
		for (int i = 0; i < m_OwnerItem.GetInventory().AttachmentCount(); i++)
		{
			EntityAI att = m_OwnerItem.GetInventory().GetAttachmentFromIndex(i);

			m_eAI_NoiseShootModifier -= att.ConfigGetFloat("noiseShootModifier");
			if (m_eAI_NoiseShootModifier < 1.0)
				break;  //! Guns can only have one suppressor, ok to break
		}

		//! This should not be possible to happen, but better safe than sorry
		if (m_eAI_NoiseShootModifier < 0.0)
			m_eAI_NoiseShootModifier = 0.0;
	}

	float eAI_GetNoiseShootModifier()
	{
		return m_eAI_NoiseShootModifier;
	}
}
