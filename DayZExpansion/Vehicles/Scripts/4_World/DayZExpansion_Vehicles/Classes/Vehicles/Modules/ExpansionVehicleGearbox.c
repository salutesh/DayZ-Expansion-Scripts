/*
 * ExpansionVehicleGearbox.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleGearboxType
{
	DEFAULT,
	ADVANCED
};

static ExpansionVehicleGearbox Expansion_CreateGearbox(EntityAI vehicle, string path)
{
	int type = ExpansionVehicleGearboxType.DEFAULT;
	string typePath = path + " type";

	if (GetGame().ConfigIsExisting(typePath))
	{
		switch (GetGame().ConfigGetType(typePath))
		{
		case CT_STRING:
			string typeName;
			GetGame().ConfigGetText(typePath, typeName);
			typeName.ToUpper();
			type = typename.StringToEnum(ExpansionVehicleGearboxType, typeName);
			break;
		default:
			type = GetGame().ConfigGetInt(typePath);
			break;
		}
	}

	switch (type)
	{
	case ExpansionVehicleGearboxType.ADVANCED:
		return new ExpansionVehicleGearboxAdvanced(vehicle, path);
	case ExpansionVehicleGearboxType.DEFAULT:
		return new ExpansionVehicleGearboxDefault(vehicle, path);
	}

	return null;
};

class ExpansionVehicleGearbox : ExpansionVehicleRotational
{
	int m_GearIndex;

	float m_TimeToUncoupleClutch;
	float m_TimeToCoupleClutch;
	float m_MaxClutchTorque;

	ref array<float> m_Ratios = new array<float>();
	ref array<string> m_Gears = new array<string>();

	int m_Gear;
	float m_Clutch;

	void ExpansionVehicleGearbox(EntityAI pVehicle, string rootPath)
	{
		m_Clutch = 0;
		m_Gear = 0;
		m_Ratio = 1;

		m_Position = "0.0 0.05 0.0";
	}

	int Count()
	{
		return m_Ratios.Count();
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Controller.m_Clutch[m_GearIndex] = m_Clutch;
		m_Controller.m_Gear[m_GearIndex] = m_Gear;
		m_Controller.m_Ratio[m_GearIndex] = m_Ratio;
		m_Controller.m_GearCount[m_GearIndex] = Count();

		super.PreSimulate(pState);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Clutch", m_Clutch);
		instance.Add("Gear", m_Gear);

		return true;
	}
#endif
};
