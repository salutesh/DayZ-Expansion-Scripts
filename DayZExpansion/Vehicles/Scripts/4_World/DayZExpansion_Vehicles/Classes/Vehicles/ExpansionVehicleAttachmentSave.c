/*
 * ExpansionVehicleAttachmentSave.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleAttachmentSave
{
	vector m_Position;
	vector m_Orientation;

	void ExpansionVehicleAttachmentSave(vector position = vector.Zero, vector orientation = vector.Zero)
	{
		m_Position = position;
		m_Orientation = orientation;
	}

#ifdef CF_MODULE_MODSTORAGE
	void OnWrite(CF_ModStorage ctx)
	{
		ctx.Write(m_Position);
		ctx.Write(m_Orientation);
	}

	bool OnRead(CF_ModStorage ctx)
	{
		if (!ctx.Read(m_Position))
			return false;
			
		if (!ctx.Read(m_Orientation))
			return false;

		return true;
	}
#endif
};
