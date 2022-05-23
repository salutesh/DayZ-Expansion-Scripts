typedef ExpansionPrefab ExpansionLoadout;

class ExpansionHumanLoadout
{
	static const string FILETYPE = ".json";

	static void Init()
	{
		if (FileExist(EXPANSION_LOADOUT_FOLDER))
			return;

		//! Move previous loadouts to new location
		string loadoutFolderAI = EXPANSION_FOLDER + "AI\\Loadouts\\";
		if (FileExist(loadoutFolderAI))
		{
			if (ExpansionSettingBase.MoveSettings(loadoutFolderAI, EXPANSION_LOADOUT_FOLDER))
				return;
		}

		MakeDirectory(EXPANSION_LOADOUT_FOLDER);

		// Armed Loadouts
		DefaultHumanLoadout();
		DefaultPoliceLoadout();
		DefaultFireFighterLoadout();
		DefaultTTSKOLoadout();
		DefaultGorkaLoadout();
		
		// Default faction loadouts
		DefaulEastLoadout();
		DefaulWestLoadout();
		DefaultSurvivorLoadout();
		DefaultBanditLoadout();

		// unnarmed Loadouts
		DefaultPlayerSurviorLoadout();
		DefaultPlayerMaleSuitLoadout();
		DefaultPlayerFemaleSuitLoadout();

		//DefaultPlayerFemaleSurviorLoadout();

		//DefaultPlayerUniversalMedicLoadout();
		//DefaultPlayerMaleMedicLoadout();
		//DefaultPlayerFemaleMedicLoadout();

		//DefaultPlayerFireFighterLoadout();
		//DefaultPlayerMecanicLoadout();
		//DefaultPlayerFisherLoadout();
	}

	static void DefaultPlayerFemaleSuitLoadout()
	{		
		// Generate the default PlayerFemaleSuitLoadout.json file
		auto loadout = ExpansionLoadout.Create("PlayerFemaleSuitLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("WomanSuit_Blue", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Blue", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WomanSuit_Brown", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Brown", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WomanSuit_DarkGrey", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_DarkGrey", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WomanSuit_Khaki", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Khaki", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WomanSuit_LightGrey", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_LightGrey", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WomanSuit_White", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_White", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
            
			loadout = loadout.BeginAttachment("WomanSuit_Black", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Black", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("DressShoes_White", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Sunburst", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaultPlayerMaleSuitLoadout()
	{		
		// Generate the default PlayerMaleSuitLoadout.json file
		auto loadout = ExpansionLoadout.Create("PlayerMaleSuitLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("ManSuit_Blue", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Blue", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("ManSuit_Brown", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Brown", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("ManSuit_DarkGrey", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_DarkGrey", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("ManSuit_Khaki", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Khaki", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("ManSuit_LightGrey", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_LightGrey", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("ManSuit_White", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_White", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
            
			loadout = loadout.BeginAttachment("ManSuit_Black", "Body");
                loadout = loadout.BeginAttachment("SlacksPants_Black", "Legs");
                loadout.SetHealth(0.7, 1.0);
                loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("DressShoes_White", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DressShoes_Sunburst", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}
	
	static void DefaultPlayerSurviorLoadout()
	{
		// Generate the default PlayerSurvivorLoadout.json file
		auto loadout = ExpansionLoadout.Create("PlayerSurvivorLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("Hoodie_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Brown", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheckBright", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_GreenCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_PlaneBlack", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_RedCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_WhiteCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Jeans_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_BlueDark", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Brown", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Grey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AthleticShoes_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("AthleticShoes_Blue", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("AthleticShoes_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("AthleticShoes_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("AthleticShoes_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

		loadout.Save();
	}

	static void DefaultBanditLoadout()
	{
		// Generate the default BanditLoadout.json file
		auto loadout = ExpansionLoadout.Create("BanditLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("GorkaEJacket_Autumn", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Flat", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_PautRev", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Summer", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTsKOJacket_Camo", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Brown", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Brown", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Maroon", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Olive", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_SkyBlue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DenimJacket", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Orange", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Violet", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Yellow", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheckBright", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_GreenCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_PlaneBlack", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_RedCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_WhiteCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("GorkaPants_Autumn", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Flat", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_PautRev", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Summer", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTSKOPants", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_BlueDark", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Brown", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Grey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_LightBlue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Red", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Beige", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Grey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("TTSKOBoots", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("WorkingBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Yellow", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WorkingGloves_Beige", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Black", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Yellow", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("OMNOGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.02;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OMNOGloves_Gray", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.02;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.8;
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Balaclava3Holes_Black", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Balaclava3Holes_Beige", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Balaclava3Holes_Blue", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Balaclava3Holes_Green", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Beige", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Black", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Blackskull", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Blue", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Green", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_Pink", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BalaclavaMask_White", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WeldingMask", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Bandana_Blackpattern", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Bandana_Camopattern", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Bandana_Greenpattern", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Bandana_Polkapattern", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Bandana_Redpattern", "MASK");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("PoliceVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("UKAssVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("UKAssVest_Camo", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HighCapacityVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.2;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CivilianBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("BaseballBat", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefighterAxe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shovel", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PipeWrench", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Pipe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Pickaxe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Crowbar", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Melee");
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK101", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK101_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK74", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_Hndgrd");
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKS74U", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AKS74U_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_30Rnd");
				loadout.Chance = 0.9;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MP5K", "Hands");
				loadout = loadout.BeginAttachment("Mag_MP5_30Rnd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_MP5_15Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5k_StockBttstck");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_PlasticHndgrd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_RailHndgrd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_Compensator");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Mosin9130", "Hands");
				loadout = loadout.BeginAttachment("Ammo_762x54");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PUScopeOptic");
				loadout.Chance = 0.4;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SKS", "Hands");
				loadout = loadout.BeginAttachment("Ammo_762x39");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PUScopeOptic");
				loadout.Chance = 0.3;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Ruger1022", "Hands");
				loadout = loadout.BeginAttachment("Mag_Ruger1022_30Rnd");
				loadout.Chance = 0.4;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_Ruger1022_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ550", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ550_4rnd");
				loadout.Chance = 0.6;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CZ550_10rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("HuntingOptic");
				loadout.Chance = 0.4;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("P1", "Hands");
				loadout = loadout.BeginAttachment("Mag_P1_8Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Mp133Shotgun", "Hands");
				loadout = loadout.BeginAttachment("Ammo_12gaSlug");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ527", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ527_5rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("HuntingOptic");
				loadout.Chance = 0.5;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("UMP45", "Hands");
				loadout = loadout.BeginAttachment("Mag_UMP_25Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ61", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ61_20Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FNX45", "Hands");
				loadout = loadout.BeginAttachment("Mag_FNX45_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MKII", "Hands");
				loadout = loadout.BeginAttachment("Mag_MKII_10Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ75", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ75_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MakarovIJ70", "Hands");
				loadout = loadout.BeginAttachment("Mag_IJ70_8Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Glock19", "Hands");
				loadout = loadout.BeginAttachment("Mag_Glock_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();

		loadout.Save();
	}

	static void DefaultSurvivorLoadout()
	{
		// Generate the default SurvivorLoadout.json file
		auto loadout = ExpansionLoadout.Create("SurvivorLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("GorkaEJacket_Autumn", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Flat", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_PautRev", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Summer", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTsKOJacket_Camo", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Brown", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Hoodie_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Brown", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Maroon", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_Olive", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BomberJacket_SkyBlue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DenimJacket", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HikingJacket_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Blue", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Green", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Grey", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Orange", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Red", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Violet", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("QuiltedJacket_Yellow", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_BlueCheckBright", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_GreenCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_PlaneBlack", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_RedCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shirt_WhiteCheck", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("GorkaPants_Autumn", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Flat", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_PautRev", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Summer", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTSKOPants", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_BlueDark", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Brown", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Jeans_Grey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_LightBlue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TrackSuitPants_Red", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Beige", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Blue", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Green", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CargoPants_Grey", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("TTSKOBoots", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("WorkingBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Yellow", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("WorkingGloves_Beige", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Black", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Yellow", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("OMNOGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.02;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OMNOGloves_Gray", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.02;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.8;
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.2;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CivilianBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("BaseballBat", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefighterAxe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Shovel", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PipeWrench", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Pipe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Pickaxe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.05;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Crowbar", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Melee");
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK101", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK101_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK74", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_Hndgrd");
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKS74U", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AKS74U_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_30Rnd");
				loadout.Chance = 0.9;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MP5K", "Hands");
				loadout = loadout.BeginAttachment("Mag_MP5_30Rnd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_MP5_15Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5k_StockBttstck");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_PlasticHndgrd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_RailHndgrd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_Compensator");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Mosin9130", "Hands");
				loadout = loadout.BeginAttachment("Ammo_762x54");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PUScopeOptic");
				loadout.Chance = 0.4;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SKS", "Hands");
				loadout = loadout.BeginAttachment("Ammo_762x39");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PUScopeOptic");
				loadout.Chance = 0.3;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Ruger1022", "Hands");
				loadout = loadout.BeginAttachment("Mag_Ruger1022_30Rnd");
				loadout.Chance = 0.4;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_Ruger1022_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ550", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ550_4rnd");
				loadout.Chance = 0.6;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CZ550_10rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("HuntingOptic");
				loadout.Chance = 0.4;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("P1", "Hands");
				loadout = loadout.BeginAttachment("Mag_P1_8Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Mp133Shotgun", "Hands");
				loadout = loadout.BeginAttachment("Ammo_12gaSlug");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ527", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ527_5rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("HuntingOptic");
				loadout.Chance = 0.5;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("UMP45", "Hands");
				loadout = loadout.BeginAttachment("Mag_UMP_25Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ61", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ61_20Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FNX45", "Hands");
				loadout = loadout.BeginAttachment("Mag_FNX45_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MKII", "Hands");
				loadout = loadout.BeginAttachment("Mag_MKII_10Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ75", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ75_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MakarovIJ70", "Hands");
				loadout = loadout.BeginAttachment("Mag_IJ70_8Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Glock19", "Hands");
				loadout = loadout.BeginAttachment("Mag_Glock_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();

		loadout.Save();
	}

	static void DefaultFireFighterLoadout()
	{		
		// Generate the default FireFighterLoadout.json file
		auto loadout = ExpansionLoadout.Create("FireFighterLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("FirefighterJacket_Beige", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefighterJacket_Black", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FirefightersPants_Beige", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefightersPants_Black", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("WorkingBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingBoots_Yellow", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("ReflexVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FirefightersHelmet_Red", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefightersHelmet_White", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("FirefightersHelmet_Yellow", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("WorkingGloves_Beige", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Black", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("WorkingGloves_Yellow", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CivilianBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FirefighterAxe", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.3;
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Crowbar", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaulWestLoadout()
	{
		// Generate the default WestLoadout.json file
		auto loadout = ExpansionLoadout.Create("WestLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;			

			loadout = loadout.BeginAttachment("USMCJacket_Desert", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("USMCJacket_Woodland", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("USMCPants_Desert", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("USMCPants_Woodland", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CoyoteBag_Brown", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.6;
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CoyoteBag_Green", "Back");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PlateCarrierVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("PlateCarrierPouches");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PlateCarrierHolster");
				loadout.Chance = 0.5;
				loadout = loadout.End();
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HighCapacityVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BallisticHelmet_Black", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("BallisticHelmet_Green", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("Mich2001Helmet", "Headgear");
				loadout = loadout.BeginAttachment("NVGoggles", "Headgear");
				loadout.Chance = 0.4;
				loadout.SetHealth(0.7, 1.0);
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TacticalGloves_Beige", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TacticalGloves_Black", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TacticalGloves_Green", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Scout", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_Scout_5Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic_6x");
				loadout.Chance = 0.15;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.075;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M68Optic");
				loadout.Chance = 0.075;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("StarlightOptic");
				loadout.Chance = 0.05;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FAMAS", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_FAMAS_25Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("M4A1", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_STANAG_30Rnd");
				loadout.Chance = 0.60;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_STANAG_60Rnd");
				loadout.Chance = 0.10;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CMAG_10Rnd");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CMAG_20Rnd");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CMAG_30Rnd");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_CMAG_40Rnd");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_CQBBttstck");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_MPBttstck");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_OEBttstck");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_MPHndgrd");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_PlasticHndgrd");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_RISHndgrd");
				loadout.Chance = 0.33;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic_6x");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M68Optic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_T3NRDSOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("StarlightOptic");
				loadout.Chance = 0.01;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_CarryHandleOptic");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Aug", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_Aug_30Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AugShort", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_Aug_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic_6x");
				loadout.Chance = 0.05;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M68Optic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("M4_T3NRDSOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("StarlightOptic");
				loadout.Chance = 0.01;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("M16A2", "Hands");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("Mag_STANAG_60Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_STANAG_30Rnd");
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

	static void DefaulEastLoadout()
	{
		// Generate the default EastLoadout.json file
		auto loadout = ExpansionLoadout.Create("EastLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("GorkaEJacket_Autumn", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Flat", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_PautRev", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Summer", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTsKOJacket_Camo", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("GorkaPants_Autumn", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Flat", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_PautRev", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Summer", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TTSKOPants", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("TTSKOBoots", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TortillaBag", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.6;
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AssaultBag_Ttsko", "Back");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SmershBag", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SmershVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("SmershBag");
				loadout.Chance = 0.8;
				loadout = loadout.End();
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("UKAssVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();			
			loadout = loadout.BeginAttachment("UKAssVest_Camo", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HighCapacityVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Ssh68Helmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TankerHelmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PilotkaCap", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OfficierHat", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaHelmet", "Headgear");
				loadout = loadout.BeginAttachment("GorkaHelmetVisor", "Headgear");
				loadout.Chance = 0.4;
				loadout.SetHealth(0.7, 1.0);
				loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("OMNOGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OMNOGloves_Gray", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK101", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK101_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK74", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_Hndgrd");
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKS74U", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AKS74U_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_30Rnd");
				loadout.Chance = 0.9;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SVD", "Hands");
				loadout = loadout.BeginAttachment("Mag_SVD_10Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PP19", "Hands");
				loadout = loadout.BeginAttachment("Mag_PP19_64Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PP19_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PistolSuppressor");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FAL", "Hands");
				loadout = loadout.BeginAttachment("Mag_FAL_20Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Fal_FoldingBttstck");
				loadout.Chance = 0.5;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Fal_OeBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("Ammo_762x39");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaultGorkaLoadout()
	{
		// Generate the default GorkaLoadout.json file
		auto loadout = ExpansionLoadout.Create("GorkaLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("GorkaEJacket_Autumn", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Flat", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_PautRev", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaEJacket_Summer", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("GorkaPants_Autumn", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Flat", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_PautRev", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaPants_Summer", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Beige", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Green", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TortillaBag", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.6;
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SmershBag", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.1;
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();

			loadout = loadout.BeginAttachment("", "Back");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SmershVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginAttachment("SmershBag");
				loadout.Chance = 0.8;
				loadout = loadout.End();
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("UKAssVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("HighCapacityVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Ssh68Helmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TankerHelmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PilotkaCap", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OfficierHat", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("GorkaHelmet", "Headgear");
				loadout = loadout.BeginAttachment("GorkaHelmetVisor", "Headgear");
				loadout.Chance = 0.4;
				loadout.SetHealth(0.7, 1.0);
				loadout = loadout.End();
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("OMNOGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OMNOGloves_Gray", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK101", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK101_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AK74", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_WoodBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_Hndgrd");
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKS74U", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AKS74U_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_30Rnd");
				loadout.Chance = 0.9;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SVD", "Hands");
				loadout = loadout.BeginAttachment("Mag_SVD_10Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PP19", "Hands");
				loadout = loadout.BeginAttachment("Mag_PP19_64Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PP19_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PistolSuppressor");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("FAL", "Hands");
				loadout = loadout.BeginAttachment("Mag_FAL_20Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Fal_FoldingBttstck");
				loadout.Chance = 0.5;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Fal_OeBttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ACOGOptic");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("Ammo_762x39");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaultTTSKOLoadout()
	{
		// Generate the default TTSKOLoadout.json file
		auto loadout = ExpansionLoadout.Create("TTSKOLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("TTsKOJacket_Camo", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TTSKOPants", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("TTSKOBoots", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AssaultBag_Ttsko", "Back");
			loadout.SetHealth(0.7, 1.0);
				loadout = loadout.BeginCargo("MakarovIJ70");
				loadout.Chance = 0.34;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.3;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Mag_IJ70_8Rnd");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("BandageDressing");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("Chemlight_Green");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_Black");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				
				loadout = loadout.BeginCargo("RDG2SmokeGrenade_White");
				loadout.Chance = 0.1;
				loadout = loadout.End();

				loadout = loadout.BeginCargo("Ammo_762x39");
				loadout.Chance = 0.1;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("UKAssVest_Camo", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("UKAssVest_Olive", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Ssh68Helmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("TankerHelmet", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PilotkaCap", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OfficierHat", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("OMNOGloves_Brown", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("OMNOGloves_Gray", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MilitaryBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKS74U", "Hands");
				loadout = loadout.BeginAttachment("Mag_AK74_30Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AKS74U_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("AKM", "Hands");
				loadout = loadout.BeginAttachment("Mag_AKM_30Rnd");
				loadout.Chance = 0.9;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_AKM_Drum75Rnd");
				loadout.Chance = 0.1;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_PlasticBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK_FoldingBttstck");
				loadout.Chance = 0.3;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("AK74_WoodBttstck");
				loadout.Chance = 0.1;
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
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PSO1Optic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PP19", "Hands");
				loadout = loadout.BeginAttachment("Mag_PP19_64Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PP19_Bttstck");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("PistolSuppressor");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("KobraOptic");
				loadout.Chance = 0.2;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SKS", "Hands");
				loadout = loadout.BeginAttachment("Ammo_762x39");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("BandageDressing");
			loadout.Chance = 0.15;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("Ammo_762x39");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaultPoliceLoadout()
	{		
		// Generate the default PoliceLoadout.json file
		auto loadout = ExpansionLoadout.Create("PoliceLoadout");

		if (!loadout || FileExist(loadout.GetPath()))
			return;

			loadout = loadout.BeginAttachment("PoliceJacket", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PoliceJacketOrel", "Body");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PolicePants", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("PolicePantsOrel", "Legs");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CombatBoots_Black", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Brown", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("CombatBoots_Grey", "Feet");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PoliceVest", "Vest");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("PoliceCap", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			loadout = loadout.BeginAttachment("DirtBikeHelmet_Police", "Headgear");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("SurgicalGloves_Blue", "Gloves");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CivilianBelt", "Hips");
			loadout.SetHealth(0.7, 1.0);
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("StunBaton", "Melee");
			loadout.SetHealth(0.7, 1.0);
			loadout.Chance = 0.8;
				loadout = loadout.BeginAttachment("Battery9V");
				loadout.Chance = 0.8;
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MP5K", "Hands");
				loadout = loadout.BeginAttachment("Mag_MP5_30Rnd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("Mag_MP5_15Rnd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5k_StockBttstck");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_PlasticHndgrd");
				loadout.Chance = 0.8;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_RailHndgrd");
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("MP5_Compensator");
				loadout.Chance = 0.2;
				loadout = loadout.End();
				loadout = loadout.BeginAttachment("ReflexOptic");
				loadout.Chance = 0.1;
					loadout = loadout.BeginAttachment("Battery9V");
					loadout = loadout.End();
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Mp133Shotgun", "Hands");
				loadout = loadout.BeginAttachment("Ammo_12gaRubberSlug");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("CZ61", "Hands");
				loadout = loadout.BeginAttachment("Mag_CZ61_20Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("MakarovIJ70", "Hands");
				loadout = loadout.BeginAttachment("Mag_IJ70_8Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginAttachment("Glock19", "Hands");
				loadout = loadout.BeginAttachment("Mag_Glock_15Rnd");
				loadout = loadout.End();
			loadout = loadout.End();
			
			loadout = loadout.BeginCargo("Apple");
			loadout.Chance = 0.1;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("Handcuffs");
			loadout.Chance = 0.15;
			loadout = loadout.End();
			loadout = loadout.BeginCargo("HandcuffKeys");
			loadout.Chance = 0.15;
			loadout = loadout.End();
		loadout.Save();
	}

	static void DefaultHumanLoadout()
	{		
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

	static bool Apply(DayZPlayerImplement player, string file, bool forceReload = false)
	{
		ExpansionHumanLoadout.Init();
		
		int length = file.IndexOf(FILETYPE);
		string name = "";

		// the file doesn't contain .json at the end
		if (length == -1)
		{
			name = file;
		} else {
			name = file.Substring(0, length);
		}

		ExpansionLoadout loadout = ExpansionLoadout.Load(name, forceReload);
		if (!loadout)
		{
			Print("ERROR !!! Unknwon loadout requested ("+file+")");
			return false;
		}

		loadout.SpawnOn(player);
		return true;
	}
};
