class bldr_Chemlight_ColorBase: Chemlight_ColorBase
{
	void bldr_Chemlight_ColorBase()
	{
		SetAllowDamage(false);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		CreateLight();
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

	override void CreateLight()
	{
		SetObjectMaterial(0, GetMaterialForDamageState(true)); // Must be server side!

		#ifndef SERVER
			m_Light = ChemlightLight.Cast(ScriptedLightBase.CreateLight( ChemlightLight, "0 0 0"));
			m_Light.AttachOnMemoryPoint(this, "light");

			string type = GetType();
			switch (type)
			{
				case "bldr_Chemlight_White":
					m_Light.SetColorToWhite();
					break;
				case "bldr_Chemlight_Red":
					m_Light.SetColorToRed();
					break;
				case "bldr_Chemlight_Green":
					m_Light.SetColorToGreen();
					break;
				case "bldr_Chemlight_Blue":
					m_Light.SetColorToBlue();
					break;
				case "bldr_Chemlight_Yellow":
					m_Light.SetColorToYellow();
					break;
				default: {m_Light.SetColorToWhite();};
			}
		#endif
	}
};

class bldr_Chemlight_White: bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Red: bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Green: bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Blue: bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Yellow: bldr_Chemlight_ColorBase {};