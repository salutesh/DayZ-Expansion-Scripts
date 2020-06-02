class ExpansionSkinDamageZone
{
	string Zone;
	
	autoptr array< string > HiddenSelections;
	autoptr array< ref ExpansionSkinHealthLevel > HealthLevels;
	
	void ExpansionSkinDamageZone()
	{
		HiddenSelections = new array< string >;
		HealthLevels = new array< ref ExpansionSkinHealthLevel >;
	}
}