class Expansion_HatchbackDoors_Driver: HatchbackDoors_Driver
{
	override string ExpansionMainType()
	{
		return "Expansion_HatchbackDoors_Driver";
	}
}
class Expansion_HatchbackDoors_CoDriver: HatchbackDoors_CoDriver
{
	override string ExpansionMainType()
	{
		return "Expansion_HatchbackDoors_CoDriver";
	}
}
class Expansion_HatchbackHood: HatchbackHood
{
	override string ExpansionMainType()
	{
		return "Expansion_HatchbackHood";
	}
}
class Expansion_HatchbackTrunk: HatchbackTrunk
{
	override string ExpansionMainType()
	{
		return "Expansion_HatchbackTrunk";
	}
}

modded class OffroadHatchback
{
	override bool IsVitalCarBattery()
	{
		return true;
	}
}