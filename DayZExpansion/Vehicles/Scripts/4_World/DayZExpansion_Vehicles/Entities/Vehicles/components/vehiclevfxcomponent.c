modded class VehicleVFXComponent
{
	override void PlayEffect(EVehicleVFXEffect effectId, EntityAI parent = null)
	{
		if (!parent)
			parent = m_Parent;

		//! Disable unwanted particle fx for Expansion helis & Expansion boats
		if (parent && !parent.IsInherited(ExpansionHelicopterScript) && !parent.IsInherited(ExpansionBoatScript))
			super.PlayEffect(effectId, parent);
	}
}
