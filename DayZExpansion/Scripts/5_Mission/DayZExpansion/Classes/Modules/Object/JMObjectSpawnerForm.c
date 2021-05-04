modded class JMObjectSpawnerForm
{
	private static ref array< string > items =
	{
		"quickiebow",
		"recurvebow",
		"gp25base",
		"gp25",
		"gp25_standalone",
		"m203base",
		"m203",
		"m203_standalone",
		"red9",
		"pvcbow",
		"crossbow",
		"augsteyr",
		"m249",
		"undersluggrenadem4",
		"groza",
		"pm73rak",
		"trumpet",
		"lawbase",
		"law",
		"rpg7base",
		"rpg7",
		"dartgun",
		"shockpistol",
		"shockpistol_black",
		"derringer_black",
		"derringer_pink",
		"derringer_grey",
		"fnx45_arrow",
		"longhorn",
		"p1",
		"makarovpb",
		"mp133shotgun_pistolgrip",

		"largetentbackpack",
		"splint_applied",
		"leatherbelt_natural",
		"leatherbelt_beige",
		"leatherbelt_brown",
		"leatherbelt_black",
		"leatherknifeshealth",

		"itemoptics",
		"fx",
		"expansionsoundproxybase",
		"expansionvehiclebasesoundproxybase",
		"expansionhelicopterscript",
		"expansionvodnikaudio",
		"expansionlhdaudio",
		"expansionutilityaudio",
		"expansionzodiacaudio",
		"expansiongyrocopteraudio",
		"expansionmerlinaudio",
		"expansionmh6audio",
		"expansionuh1haudio",
		"expansionvehicleplanebase",
		"expansionvehiclehelicopterbase",
		"expansionvehicleboatbase",
		"expansionvehiclebikebase",
		"expansionvehiclecarbase",
		"expansionvehiclevehiclebase"
	};

	private static ref array< string > itemsList =
	{
		"placing",
		"debug",
		"bldr_",
		#ifndef EXPANSION_COT_VEHICLE_JACOB
		"vehicle_",
		#endif
		"proxy"
	};

	private override bool CheckItemCrash( string name )
	{
		if ( items.Find( name ) > -1 )
		{
			return true;
		}

		for ( int i = 0; i < itemsList.Count(); ++i )
		{
			if ( name.Contains( itemsList[i] ) )
			{
				return true;
			}
		}
		return false;
	}
};