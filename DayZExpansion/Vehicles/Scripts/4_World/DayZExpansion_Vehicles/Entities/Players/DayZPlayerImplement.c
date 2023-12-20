/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	protected bool m_Expansion_IsHeadInvisible;

	ExpansionHumanCommandVehicle StartCommand_ExpansionVehicle(ExpansionVehicleBase vehicle, int seatIdx, int seat_anim)
	{
		return NULL;
	}

	ExpansionHumanCommandVehicle GetCommand_ExpansionVehicle()
	{
		return ExpansionHumanCommandVehicle.Cast(GetCommand_Script());
	}

	ExpansionVehicleBase GetExpansionTransport()
	{
		ExpansionHumanCommandVehicle script;
		if (Class.CastTo(script, GetCommand_Script()))
		{
			return script.GetExpansionVehicle();
		}

		return NULL;
	}

	void OnCommandExpansionVehicleStart()
	{
	}

	void OnCommandExpansionVehicleFinish()
	{
	}

	override bool HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		if (GetCommand_ExpansionVehicle())
		{
			m_fLastHeadingDiff = 0;

			return false;
		}

		return super.HeadingModel(pDt, pModel);
	}

	override int CameraHandler(int pCameraMode)
	{
		ExpansionVehicleBase exTrans;
		ExpansionHumanCommandVehicle vehicleCommand = GetCommand_ExpansionVehicle();
		if (vehicleCommand)
			exTrans = vehicleCommand.GetExpansionVehicle();

		if (m_Camera3rdPerson)
		{
			if (vehicleCommand && exTrans)
			{
				int cameraType = exTrans.Get3rdPersonCameraType();
				if (cameraType != -1)
					return cameraType;
			}
		}
		else
		{
			if (vehicleCommand)
			{
				return DayZPlayerCameras.DAYZCAMERA_1ST_VEHICLE;
			}
		}

		return super.CameraHandler(pCameraMode);
	}

	void SetHeadInvisible_Ex(bool invisible)
	{
		if (m_Expansion_IsHeadInvisible == invisible)
			return;

		Head_Default player_head;
		int slot_id = InventorySlots.GetSlotIdFromString("Head");
		player_head = Head_Default.Cast(GetInventory().FindPlaceholderForSlot(slot_id));

		player_head.SetInvisible(invisible);

		m_Expansion_IsHeadInvisible = invisible;

		SetAttachmentInvisible_Ex("Head", invisible);
		SetAttachmentInvisible_Ex("Headgear", invisible);
		SetAttachmentInvisible_Ex("Mask", invisible);
		SetAttachmentInvisible_Ex("Eyewear", invisible);
	}

	void SetAttachmentInvisible_Ex(string slot, bool invisible)
	{
		int slot_id = InventorySlots.GetSlotIdFromString(slot);
		EntityAI ent = GetInventory().FindAttachment(slot_id);

		if (ent)
			SetAttachmentInvisible_Ex(ent, invisible);
	}

	//! Vanilla has EntityAI::SetInvisibleRecursive, but it doesn't seem to work correctly (and it's messy, yuck)
	void SetAttachmentInvisible_Ex(EntityAI ent, bool invisible)
	{
		ent.SetInvisible(invisible);

		for (int i = 0; i < ent.GetInventory().AttachmentCount(); i++)
		{
			SetAttachmentInvisible_Ex(ent.GetInventory().GetAttachmentFromIndex(i), invisible);
		}
	}

	bool Expansion_IsHeadInvisible()
	{
		return m_Expansion_IsHeadInvisible;
	}

	override bool IsInTransport()
	{
		if (GetCommand_Vehicle())
			return true;

		if (ExpansionHumanCommandVehicle.Cast(GetCommand_Script()))
			return true;

		return false;
	}
};
