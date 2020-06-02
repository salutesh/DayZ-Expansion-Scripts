class Expansion_CivSedanDoors_Driver: CivSedanDoors_Driver
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanDoors_Driver";
	}
}
class Expansion_CivSedanDoors_CoDriver: CivSedanDoors_CoDriver
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanDoors_CoDriver";
	}
}
class Expansion_CivSedanDoors_BackLeft: CivSedanDoors_BackLeft
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanDoors_BackLeft";
	}
}
class Expansion_CivSedanDoors_BackRight: CivSedanDoors_BackRight
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanDoors_BackRight";
	}
}
class Expansion_CivSedanHood: CivSedanHood
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanHood";
	}
}
class Expansion_CivSedanTrunk: CivSedanTrunk
{
	override string ExpansionMainType()
	{
		return "Expansion_CivSedanTrunk";
	}
}

modded class CivilianSedan
{
	override bool IsVitalCarBattery()
	{
		return true;
	}
}