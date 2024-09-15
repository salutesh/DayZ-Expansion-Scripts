class CfgPatches
{
	class Expansion_Tops
	{
		units[]=
		{
			"Shirt_CheckRed",
			"Hoodie_Blue",
			"Hoodie_Black",
			"Hoodie_Brown",
			"Hoodie_Green",
			"Hoodie_Grey",
			"Hoodie_Red",
			"MaleTorso",
			"FemaleTorso"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters"
		};
	};
};
class CfgVehicles
{
	class TShirt_ColorBase;
	class Shirt_ColorBase;
	class RidersJacket_ColorBase;
	class ExpansionTee: TShirt_ColorBase
	{
		scope=2;
		displayName="Expansion Dark Tee";
		descriptionShort="Expansion Dark Tee";
		visibilityModifier=0.85000002;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\tops\data\expansion_shirt.paa",
			"DayZExpansion\Data\characters\tops\data\expansion_shirt.paa",
			"DayZExpansion\Data\characters\tops\data\expansion_shirt.paa"
		};
	};
	class ExpansionRidersJacketBrown: RidersJacket_ColorBase
	{
		scope=2;
		visibilityModifier=0.69999999;
		model="\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa"
		};
	};
	class ExpansionRidersJacketDarkBlue: RidersJacket_ColorBase
	{
		scope=2;
		visibilityModifier=0.69999999;
		model="\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa"
		};
	};
	class ExpansionRidersJacketFogEaters: RidersJacket_ColorBase
	{
		scope=2;
		visibilityModifier=0.69999999;
		model="\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa",
			"DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa"
		};
	};
	class ExpansionTommy: Shirt_ColorBase
	{
		scope=2;
		visibilityModifier=0.69999999;
		model="\DZ\characters\tops\shirt_m.p3d";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa",
			"DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa",
			"DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa"
		};
	};
};
