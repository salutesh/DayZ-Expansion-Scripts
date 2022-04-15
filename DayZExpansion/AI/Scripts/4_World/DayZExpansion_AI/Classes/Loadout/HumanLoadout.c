class HumanLoadout {	
	autoptr TStringArray Shirts = {"HikingJacket_Blue"};
	autoptr TStringArray Pants = {"SlacksPants_Blue"}; 							
	autoptr TStringArray Shoes = {"HikingBootsLow_Blue"};			
	autoptr TStringArray BackPacks = {"TaloonBag_Blue"};					
	autoptr TStringArray Vests = {"PressVest_Blue"};		
	autoptr TStringArray Headgear = {"BaseballCap_Blue"};
	autoptr TStringArray Gloves = {"SurgicalGloves_Blue"};	
	autoptr TStringArray Misc = {"CivilianBelt"};																			
	autoptr TIntArray	 ClothesHealth = {10,100}; 						//Item health given. 10%->100%
	
	autoptr TStringArray WeaponMelee = {"BaseballBat"}; 	
	autoptr TStringArray WeaponRifle = {"AKM"}; 	
	autoptr TIntArray	 WeaponRifleMagCount = {1,3};
	
	autoptr TStringArray WeaponHandgun = {"MakarovIJ70"}; 		
	autoptr TIntArray	 WeaponHandgunMagCount = {1,3}; 	
	autoptr TIntArray	 WeaponHealth = {10,100}; 						//Weapon health given. 10%->100%

	autoptr TStringArray Loot = {"SodaCan_Cola"};  						//These are added always
	autoptr TStringArray LootRandom = {"Screwdriver"};  				//Added with a LootRandomChance%
	autoptr TIntArray	 LootRandomChance = {30};						//Add item from Loot array
	autoptr TIntArray	 LootHealth = {10,100}; 						//Item health given. 10%->100%
	
	bool Locked;

	ref map<string, TStringArray> Attachments;

	void HumanLoadout()
	{
		Attachments = new map<string, TStringArray>;
	}

	//----------------------------------------------------------------
	//	HumanLoadout.Apply
	//
	//	Usage: HumanLoadout.Apply(pb_AI, "LoadOut.json");
	//
	//	The order of searching for loadouts:
	//	1) (TBD) From full path under profile. You can define the path to you mod's loadout files "\mymod\loadout.json"
	//	2) From default eAI config dir "eAI\loadout\*"
	//	3) Copied from the mod to the config dir. Two default loadouts exists: "SoldierLoadout.json" , "PoliceLoadout.json".
	//	4) Create a dummy loadout (blue clothes) with the given LoadoutFile filename.  
	
	static void Apply(PlayerBase h, string LoadoutFile) 
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "Apply");
		#endif

//		static string LoadoutSave = "SoldierLoadout.json";
	
		HumanLoadout Loadout = LoadData(LoadoutFile);
		HumanLoadout.AddClothes(h, Loadout);

		string weapon = Loadout.WeaponRifle.GetRandomElement();
		if (weapon)
		{
			HumanLoadout.AddWeapon(h, weapon, Loadout.WeaponHealth[0], Loadout.WeaponHealth[1], Loadout.Attachments[weapon]);
			HumanLoadout.AddMagazine(h, weapon, Loadout.WeaponRifleMagCount[0], Loadout.WeaponRifleMagCount[1]);
		}
		
		string sidearm = Loadout.WeaponHandgun.GetRandomElement();
		if (sidearm)
		{
			HumanLoadout.Add(h, sidearm, Loadout.WeaponHealth[0], Loadout.WeaponHealth[1], Loadout.Attachments[sidearm]);
			HumanLoadout.AddMagazine(h, sidearm, Loadout.WeaponHandgunMagCount[0], Loadout.WeaponHandgunMagCount[1]);
		}
		
		string melee = Loadout.WeaponMelee.GetRandomElement();
		if (melee)
		{
			HumanLoadout.Add(h, melee, Loadout.WeaponHealth[0], Loadout.WeaponHealth[1], Loadout.Attachments[melee]);
		}
		
		int i;

		for (i = 0; i < Loadout.Loot.Count(); i++) {
			Add(h, Loadout.Loot[i], Loadout.LootHealth[0], Loadout.LootHealth[1], Loadout.Attachments[Loadout.Loot[i]]);
		}

		for (i = 0; i < Loadout.LootRandom.Count(); i++) {
			if (Loadout.LootRandomChance.Count() <= i)
				Loadout.LootRandomChance.Insert(100);
			if (Math.RandomInt(0, 100) < Loadout.LootRandomChance[i])
				Add(h, Loadout.LootRandom[i], Loadout.LootHealth[0], Loadout.LootHealth[1], Loadout.Attachments[Loadout.LootRandom[i]]);
		}

		//! LOCK_FROM_SCRIPT = 4
		//! HIDE_INV_FROM_SCRIPT = 8
		//! 10134 = 2 | 4 | 16 | 128 | 256 | 512 | 1024 | 8192
		if (Loadout.Locked) {
			int attcount = h.GetInventory().AttachmentCount();
			for (int att = 0; att < attcount; att++) { 
				EntityAI attachment = h.GetInventory().GetAttachmentFromIndex(att);
				if (attachment) attachment.GetInventory().LockInventory(10134);
			}
			h.GetInventory().LockInventory(10134);
		}
	}

	//----------------------------------------------------------------
	//	HumanLoadout.AddClothes
	//
	//	Adds a clothes to.
	
	static void AddClothes(PlayerBase h, HumanLoadout Loadout) {
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "AddClothes");
		#endif

		int minhealth = Loadout.ClothesHealth[0];
		int maxhealth = Loadout.ClothesHealth[1];
		
		array<TStringArray> clothes();
		clothes.Insert(Loadout.Pants);
		clothes.Insert(Loadout.Shirts);
		clothes.Insert(Loadout.Shoes);
		clothes.Insert(Loadout.Headgear);
		clothes.Insert(Loadout.Gloves);
		clothes.Insert(Loadout.BackPacks);
		clothes.Insert(Loadout.Vests);
		clothes.Insert(Loadout.Misc);

		foreach (TStringArray types: clothes)
		{
			if (!types.Count())
				continue;
			string type = types.GetRandomElement();
			Add(h, type, minhealth, maxhealth, Loadout.Attachments[type]);
		}
	}

	//----------------------------------------------------------------
	//	HumanLoadout.AddWeapon
	//
	//	Adds a weapon to AI hands.
	//
	//	Usage: HumanLoadout.AddWeapon(pb_AI, "AKM");			//A pristine AKM is added
	//	       HumanLoadout.AddWeapon(pb_AI, "AKM", 10, 80);	//An AKM with 10%-80% health

	static void AddWeapon(PlayerBase h, string weapon, int minhealth = 100, int maxhealth = 100, TStringArray attachments = null) {
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "AddWeapon");
		#endif

		Add(h, weapon, minhealth, maxhealth, attachments, true);
	}
	
	static void Add(PlayerBase h, string type, int minhealth = 100, int maxhealth = 100, TStringArray attachments = null, bool tohands = false)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "Add");
		#endif

		EntityAI item;
		if (tohands)
			item = h.GetHumanInventory().CreateInHands(type);
		else
			item = h.GetInventory().CreateInInventory(type);

		if (!item)
		{
			CF_Log.Error("HumanLoadout: Add: Could not create " + type );
			return;
		}

		float HealthModifier = (Math.RandomIntInclusive(minhealth, maxhealth)) / 100;
		item.SetHealth(item.GetMaxHealth() * HealthModifier);

		if (attachments)
		{
			//! Yes this is needed, otherwise you get NULL pointers if there is more than one attachment :-(
			TStringArray attachmentsTmp();
			attachmentsTmp.Copy(attachments);
			foreach (string attachment: attachmentsTmp)
				item.GetInventory().CreateAttachment(attachment);
		}

		CF_Log.Debug("HumanLoadout: Add: " + type + " (" + HealthModifier + ")" );
	}
	
	//----------------------------------------------------------------
	//	HumanLoadout.AddMagazine
	//
	//	Adds magazines to AI inventory.
	//
	//	Usage: HumanLoadout.AddMagazine(pb_AI, "AKM", 3);		//Three random AKM magazines are added
	//	       HumanLoadout.AddMagazine(pb_AI, "AKM", 1, 4);	//1 to 4 random AKM magazines are added

	static void AddMagazine(PlayerBase h, string weapon, int mincount = 1, int maxcount = 0) {
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "AddMagazine");
		#endif

        TStringArray magazines = {};
        GetGame().ConfigGetTextArray("CfgWeapons " + weapon + " magazines", magazines);		
		string mag = magazines.GetRandomElement();

		int i;
		int count = mincount;
		
		if (maxcount > 0)
		{
			count = Math.RandomIntInclusive(mincount, maxcount);	
		}
		
		if (count < 1)
		{
			//Maxcount probably was larger than mincount
			count = 1;
			CF_Log.Error("HumanLoadout: ERROR: Please check you Weapon___MagCount. Giving 1 mag.");
		}
		
		for( i = 0; i < count; i++)
		{
			h.GetHumanInventory().CreateInInventory(mag);
		}

		CF_Log.Debug("HumanLoadout: Add " + count + " of " + mag + " magazines for weapon " + weapon);
	}
	
	//----------------------------------------------------------------
	//	HumanLoadout.LoadData

	static HumanLoadout LoadData(string fileName)
    {
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("HumanLoadout", "LoadData");
		#endif

        HumanLoadout data = new HumanLoadout;

		string loadoutPath;

		array<string> loadoutDirectories = eAISettings.GetLoadoutDirectories();
		foreach (string loadoutDirectory : loadoutDirectories)
		{
			loadoutPath = loadoutDirectory + fileName;
			if (FileExist(loadoutPath))
			{
				CF_Log.Info("HumanLoadout: Loading '" + loadoutPath + "'.");
				JsonFileLoader<HumanLoadout>.JsonLoadFile(loadoutPath, data);
				return data;
			}
		}

        CF_Log.Error("HumanLoadout: Couldn't find a loadout matching '" + fileName + "', loading script defaults.");

		if (loadoutDirectories.Count() == 0)
		{
            CF_Log.Error("HumanLoadout: Couldn't find a directory to save the loadout. Please set 'LoadoutDirectories' in 'eAI/eAISettings.json'");
			return data;
		}

		data.Attachments.Insert("AKM", {"AK_PlasticBttstck", "AK_PlasticHndgrd"});

		loadoutPath = loadoutDirectories[0] + fileName;

        SaveData(loadoutPath, data);

        return data;
    }

	//----------------------------------------------------------------
	//	HumanLoadout.SaveData
	
    static void SaveData(string loadoutPath, HumanLoadout data)
    {
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("HumanLoadout", "SaveData").Add(loadoutPath);
		#endif
		
		CF_Log.Info("HumanLoadout: Saving '" + loadoutPath + "'.");
        JsonFileLoader<HumanLoadout>.JsonSaveFile(loadoutPath, data);
    }	
	
};

/*
class SoldierLoadout : HumanLoadout {
	static string SoldierLoadoutSave = "SoldierLoadout.json";
	
	override static void Apply(PlayerBase h)	
	{
		HumanLoadout Loadout = LoadData(SoldierLoadoutSave);
		HumanLoadout.AddClothes(h, Loadout);

		string weapon = Loadout.WeaponRifle.GetRandomElement();
//		HumanLoadout.AddWeapon(h, weapon);
		HumanLoadout.AddWeapon(h, weapon, Loadout.WeaponHealth[0], Loadout.WeaponHealth[1]);
		HumanLoadout.AddMagazine(h, weapon, Loadout.WeaponRifleMagCount[0], Loadout.WeaponRifleMagCount[1]);
	}
}	

class PoliceLoadout : HumanLoadout {
	static string PoliceLoadoutSave = "PoliceLoadout.json";
	
	override static void Apply(PlayerBase h)
	{
		HumanLoadout Loadout = LoadData(PoliceLoadoutSave);
		HumanLoadout.AddClothes(h, Loadout);

		string weapon = Loadout.WeaponRifle.GetRandomElement();
//		HumanLoadout.AddWeapon(h, weapon);
		HumanLoadout.AddWeapon(h, weapon, Loadout.WeaponHealth[0], Loadout.WeaponHealth[1]);
		HumanLoadout.AddMagazine(h, weapon, 2);
	}
}
*/