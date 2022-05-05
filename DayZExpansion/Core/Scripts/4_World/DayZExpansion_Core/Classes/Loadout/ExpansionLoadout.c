typedef ExpansionPrefab ExpansionLoadout;

class ExpansionHumanLoadout
{
	static const string FILETYPE = ".json";

	// temp logic until LMs eAI settings rework
	static void Init(array<string> prefabDirectories = null)
	{
		// If there are more than 0 directories we can assume the system is initialized
		if (ExpansionPrefab.s_Directories.Count() != 0)
		{
			return;
		}
		
		// Insert any passed in directories
		if (prefabDirectories)
		{
			foreach (string prefabDirectory : prefabDirectories)
			{
				ExpansionPrefab.s_Directories.Insert(prefabDirectory);
			}
		}

		// Generate the default HumanLoadout.json file
		auto loadout = ExpansionLoadout.Create("HumanLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("HikingJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SlacksPants_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("SlacksPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("SlacksPants_DarkGrey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("HikingBootsLow_Blue", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingBootsLow_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingBootsLow_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TaloonBag_Blue", "Back");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("SodaCan_Cola");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Screwdriver");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PressVest_Blue", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("BaseballCap_Blue", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BaseballCap_Black", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BaseballCap_Olive", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SurgicalGloves_Blue", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CivilianBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("BaseballBat", "Melee");
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticHndgrd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodHndgrd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KashtanOptic");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.2;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static bool Apply(DayZPlayerImplement player, string file, bool forceReload = false, TStringArray prefabDirectories = null)
	{
		ExpansionHumanLoadout.Init(prefabDirectories);
		
		int length = file.Length() - FILETYPE.Length();
		if (length <= 0)
		{
			return false;
		}

		string name = file.Substring(0, length);

		ExpansionLoadout loadout = ExpansionLoadout.Load(name, forceReload);
		if (!loadout)
		{
			return false;
		}

		loadout.SpawnOn(player);
		return true;
	}
};
