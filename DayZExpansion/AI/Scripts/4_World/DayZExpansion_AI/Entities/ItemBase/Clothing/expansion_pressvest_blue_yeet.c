class Expansion_PressVest_Blue_Yeet: PressVest_ColorBase
{
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		switch (newLevel)
		{
			case GameConstants.STATE_DAMAGED:
			case GameConstants.STATE_BADLY_DAMAGED:
				Expansion_SetObjectMaterial("DayZExpansion\\AI\\Gear\\Data\\pressvest_yeet_damage.rvmat");
				break;

			case GameConstants.STATE_RUINED:
				Expansion_SetObjectMaterial("DayZExpansion\\AI\\Gear\\Data\\pressvest_yeet_destruct.rvmat");
				break;

			default:
				Expansion_SetObjectMaterial("DayZExpansion\\AI\\Gear\\Data\\pressvest_yeet.rvmat");
				break;
		}
	}

	void Expansion_SetObjectMaterial(string name)
	{
		SetObjectMaterial(0, name);
		SetObjectMaterial(1, name);
		SetObjectMaterial(2, name);
	}
}
