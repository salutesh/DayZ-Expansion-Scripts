class ExpansionSkin : Managed
{
	ref array< ref ExpansionSkinHiddenSelection > HiddenSelections;
	ref array< ref ExpansionSkinDamageZone > DamageZones;

	string HornEXT;
	string HornINT;
	
	void ExpansionSkin()
	{
		HiddenSelections = new array< ref ExpansionSkinHiddenSelection >;
		DamageZones = new array< ref ExpansionSkinDamageZone >;
	}

	void ~ExpansionSkin()
	{
		delete HiddenSelections;
		delete DamageZones;
	}
}