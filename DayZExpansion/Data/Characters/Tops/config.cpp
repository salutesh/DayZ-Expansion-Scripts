#define _ARMA_

class CfgPatches
{
	class Expansion_Tops
	{
		units[] = {"ExpansionTee","ExpansionRidersJacketBrown","ExpansionRidersJacketDarkBlue","ExpansionRidersJacketFogEaters","ExpansionTommy"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters"};
	};
};
class CfgVehicles
{
	class Armband_ColorBase;
	class BomberJacket_ColorBase;
	class BDUJacket;
	class TShirt_ColorBase;
	class TacticalShirt_ColorBase;
	class M65Jacket_ColorBase;
	class MiniDress_ColorBase;
	class Hoodie_ColorBase;
	class Sweater_ColorBase;
	class TrackSuitJacket_ColorBase;
	class QuiltedJacket_ColorBase;
	class HuntingJacket_ColorBase;
	class GorkaEJacket_ColorBase;
	class Shirt_ColorBase;
	class RidersJacket_ColorBase;
	class ExpansionTee: TShirt_ColorBase
	{
		scope = 2;
		displayName = "Expansion Dark Tee";
		descriptionShort = "Expansion Dark Tee";
		visibilityModifier = 0.85;
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\tops\data\expansion_shirt.paa","DayZExpansion\Data\characters\tops\data\expansion_shirt.paa","DayZExpansion\Data\characters\tops\data\expansion_shirt.paa"};
	};
	class ExpansionRidersJacketBrown: RidersJacket_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.7;
		model = "\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_brown_co.paa"};
	};
	class ExpansionRidersJacketDarkBlue: RidersJacket_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.7;
		model = "\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_dark_blue_co.paa"};
	};
	class ExpansionRidersJacketFogEaters: RidersJacket_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.7;
		model = "\DZ\characters\tops\riders_jacket_g.p3d";
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa","DayZExpansion\Data\characters\tops\data\riders_jacket_fog_eaters_co.paa"};
	};
	class ExpansionTommy: Shirt_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.7;
		model = "\DZ\characters\tops\shirt_m.p3d";
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa","DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa","DayZExpansion\Data\characters\tops\data\LongSleveShirtTommy_co.paa"};
	};
};
