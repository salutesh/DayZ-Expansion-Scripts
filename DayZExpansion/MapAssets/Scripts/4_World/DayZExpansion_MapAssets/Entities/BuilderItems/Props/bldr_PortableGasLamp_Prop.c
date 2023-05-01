class bldr_PortableGasLamp_Prop: House
{
	protected ScriptedLightBase m_Light;

	override void EEInit()
	{
		super.EEInit();
		
		#ifndef SERVER
			if (!m_Light)
			{
			#ifdef EXPANSIONMODCORE
				m_Light = ScriptedLightBase.CreateLight(ExpansionPointLight, "0 0 0");
			#else
				//! TODO: Use some kind of light
				return;
			#endif
		
				m_Light.SetVisibleDuringDaylight(false);
				m_Light.SetRadiusTo(15);
				m_Light.SetBrightnessTo(5.0);
				m_Light.SetCastShadow(true);
				m_Light.FadeIn(0.5);
				m_Light.SetFadeOutTime(0.1);
				m_Light.SetDiffuseColor(1.0, 0.7, 0.5);
				m_Light.SetAmbientColor(1.0, 0.7, 0.5);
				m_Light.SetFlickerAmplitude(0.3);
				m_Light.SetFlickerSpeed(0.75);
				m_Light.SetDancingShadowsMovementSpeed(0.005);
				m_Light.SetDancingShadowsAmplitude(0.003);
				m_Light.AttachOnMemoryPoint(this, "light");
			}
		#endif

		SetObjectMaterial(0, "dz\\gear\\cooking\\data\\GasLightOn.rvmat");
	}

	override void OnEnergyConsumed() {};

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutAsAttachment(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return false;
	}

	override bool CanLoadAttachment(EntityAI attachment)
	{
		return false;
	}

	override bool CanBePlaced(Man player, vector position)
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

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanLoadItemIntoCargo(EntityAI item)
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

	override bool CanDisplayAttachmentCategory(string category_name)
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