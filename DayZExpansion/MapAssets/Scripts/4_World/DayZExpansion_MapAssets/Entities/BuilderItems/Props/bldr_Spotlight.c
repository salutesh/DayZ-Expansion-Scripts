class bldr_Spotlight: Spotlight
{
	SpotlightLightProp m_LightProp;

	void bldr_Spotlight()
	{
		SetAllowDamage(false);
		Unfold();

		#ifndef SERVER
			m_LightProp = SpotlightLightProp.Cast( ScriptedLightBase.CreateLight( SpotlightLightProp, "0 0 0") );
			m_LightProp.AttachOnObject(this, m_LightLocalPosition, m_LightLocalOrientation);
		#endif

		UpdateAllSelections();
	}

	override void UpdateAllSelections()
	{
		HideAllSelections();

		ShowSelection( SEL_PLACING );
		ShowSelection( SEL_REFLECTOR_U );
		ShowSelection( SEL_GLASS_U );
		ShowSelection( SEL_REFLECTOR_COMP_U );
		ShowSelection( SEL_CORD_PLUGGED_U );
		SetObjectMaterial(ID_GLASS_UNFOLDED, LIGHT_ON_GLASS);
		SetObjectMaterial(ID_REFLECTOR_UNFOLDED, LIGHT_ON_REFLECTOR);
	}

	override void OnEnergyConsumed() {};

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}

	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}

	override bool CanReleaseFromHands(EntityAI handheld)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}

	override bool IsTakeable()
	{
		return false;
	}

	void CreateLight()
	{
		#ifndef SERVER
			m_LightProp = SpotlightLightProp.Cast(ScriptedLightBase.CreateLight( SpotlightLightProp, "0 0 0"));
			m_LightProp.AttachOnObject(this, m_LightLocalPosition, m_LightLocalOrientation);
		#endif
	}
};