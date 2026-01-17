modded class ExpansionHumanLoadout
{
	static void DefaultFreshSpawnLoadout()
	{
		if (FileExist(ExpansionLoadout.GetPath("FreshSpawnLoadout")))
			return;

		auto loadout = ExpansionLoadout.Create("FreshSpawnLoadout");

		if (!loadout)
			return;

		loadout = loadout.BeginAttachment("TShirt_Beige", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_Black", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_Blue", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_Green", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_Grey", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_OrangeWhiteStripes", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_Red", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_RedBlackStripes", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("TShirt_White", "Body");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		
		loadout = loadout.BeginAttachment("CanvasPantsMidi_Beige", "Legs");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("CanvasPantsMidi_Blue", "Legs");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("CanvasPantsMidi_Grey", "Legs");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("CanvasPantsMidi_Red", "Legs");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("CanvasPantsMidi_Violet", "Legs");
		loadout.SetHealth(0.45, 0.65);
		loadout = loadout.End();
		
		loadout = loadout.BeginAttachment("AthleticShoes_Black", "Feet");
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("AthleticShoes_Blue", "Feet");
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("AthleticShoes_Brown", "Feet");
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("AthleticShoes_Green", "Feet");
		loadout = loadout.End();
		loadout = loadout.BeginAttachment("AthleticShoes_Grey", "Feet");
		loadout = loadout.End();

		loadout = loadout.BeginCargo("BandageDressing");
		loadout = loadout.End();

		loadout = loadout.BeginSet("FOOD");
			loadout = loadout.BeginCargo("Apple");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();
		loadout = loadout.BeginSet("FOOD");
			loadout = loadout.BeginCargo("Pear");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();
		loadout = loadout.BeginSet("FOOD");
			loadout = loadout.BeginCargo("Plum");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();

		loadout = loadout.BeginSet("CHEMLIGHT");
			loadout = loadout.BeginCargo("Chemlight_White");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();
		loadout = loadout.BeginSet("CHEMLIGHT");
			loadout = loadout.BeginCargo("Chemlight_Yellow");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();
		loadout = loadout.BeginSet("CHEMLIGHT");
			loadout = loadout.BeginCargo("Chemlight_Green");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();
		loadout = loadout.BeginSet("CHEMLIGHT");
			loadout = loadout.BeginCargo("Chemlight_Red");
			loadout.SetHealth(0.45, 0.65);
			loadout = loadout.End();
		loadout = loadout.End();

		loadout.Save();
	}
}
