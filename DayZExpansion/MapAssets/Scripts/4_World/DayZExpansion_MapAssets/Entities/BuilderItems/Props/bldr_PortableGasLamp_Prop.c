class bldr_PortableGasLamp_Prop: PortableGasLamp
{
	void bldr_PortableGasLamp_Prop()
	{
		SetAllowDamage(false);

		if (GetGame().IsClient())
		{
			m_Light = PortableGasLampLight.Cast(ScriptedLightBase.CreateLight(PortableGasLampLight, "0 0 0"));
			m_Light.AttachOnMemoryPoint(this, "light");
		}

		//refresh visual
		SetObjectMaterial( 0, "dz\\gear\\cooking\\data\\GasLightOn.rvmat" );

		//sound (client only)
		SoundBurningStart();
	}

	override void OnEnergyConsumed() {};

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutAsAttachment( EntityAI parent )
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return false;
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		return false;
	}

	override bool CanLoadAttachment( EntityAI attachment )
	{
		return false;
	}

	override bool CanBePlaced( Man player, vector position )
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

	override bool CanDetachAttachment(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}

	override bool CanSaveItemInHands(EntityAI item_in_hands)
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

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		return false;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return false;
	}

	override bool CanDisplayCargo()
	{
		return false;
	}

	override bool CanAssignToQuickbar()
	{
		return false;
	}

	override bool IsTakeable()
	{
		return false;
	}
};