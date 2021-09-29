/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	ref ExpansionHumanST m_ExpansionST;

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
			return script.GetTransport();
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
			exTrans = vehicleCommand.GetTransport();

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
		Head_Default player_head;
		int slot_id = InventorySlots.GetSlotIdFromString("Head");
		player_head = Head_Default.Cast(GetInventory().FindPlaceholderForSlot(slot_id));

		player_head.SetInvisible(invisible);

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
			ent.SetInvisible(invisible);
	}
};
