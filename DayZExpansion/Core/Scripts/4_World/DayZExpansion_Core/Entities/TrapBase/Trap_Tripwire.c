modded class TripwireTrap
{
	ItemBase m_Expansion_AttachedExplosive;

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		ItemBase itemBase;
		if (Class.CastTo(itemBase, item) && itemBase.Expansion_IsExplosive())
			m_Expansion_AttachedExplosive = itemBase;
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		ItemBase itemBase;
		if (Class.CastTo(itemBase, item) && itemBase.Expansion_IsExplosive())
			m_Expansion_AttachedExplosive = null;
	}

	override TStringArray Expansion_GetAmmoTypes()
	{
		if (m_Expansion_AttachedExplosive && !m_Expansion_AttachedExplosive.IsDamageDestroyed())
			return m_Expansion_AttachedExplosive.Expansion_GetAmmoTypes();

		return {"TripWireHit"};
	}

	override bool Expansion_IsMechanicalTrap()
	{
		return true;
	}

	override bool Expansion_IsExplosive()
	{
		if (m_Expansion_AttachedExplosive && !m_Expansion_AttachedExplosive.IsDamageDestroyed())
			return true;

		return false;
	}
}
