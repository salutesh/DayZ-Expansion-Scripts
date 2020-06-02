class ExpansionSkin
{
	autoptr array< ref ExpansionSkinHiddenSelection > HiddenSelections;
	autoptr array< ref ExpansionSkinDamageZone > DamageZones;

	string HornEXT;
	string HornINT;

	[NonSerialized()]
	string SkinFileName;
	
	void ExpansionSkin()
	{
		HiddenSelections = new array< ref ExpansionSkinHiddenSelection >;
		DamageZones = new array< ref ExpansionSkinDamageZone >;
	}
}