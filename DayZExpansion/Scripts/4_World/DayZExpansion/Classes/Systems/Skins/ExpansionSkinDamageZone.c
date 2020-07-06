class ExpansionSkinDamageZone : Managed
{
	string Zone;
	
	ref array< string > HiddenSelections;
	ref array< ref ExpansionSkinHealthLevel > HealthLevels;
	
	void ExpansionSkinDamageZone()
	{
		HiddenSelections = new array< string >;
		HealthLevels = new array< ref ExpansionSkinHealthLevel >;
	}

	void ~ExpansionSkinDamageZone()
	{
		delete HiddenSelections;
		delete HealthLevels;
	}
}