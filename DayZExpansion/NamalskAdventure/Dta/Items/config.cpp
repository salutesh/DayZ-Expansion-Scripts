#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Items
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class CigarettePack_Chernamorka;
	class CigarettePack_Merkur;
	class CigarettePack_Partyzanka;
	class Expansion_CigarettePack_Chernamorka: CigarettePack_Chernamorka
	{
		scope = 2;
		displayName = "Cigarette Pack - Chernamorka";
		descriptionShort = "This pack of cigarettes bears the name Chernamorka, and features a design reminiscent of Soviet-era packaging. The brand is known for its strong, unfiltered cigarettes that are said to have a distinctive and bold flavor. The pack contains 20 cigarettes, each one wrapped in brown paper with a simple red logo stamped on it. Despite their popularity among some smokers, these cigarettes are known to be harsh on the throat and lungs, and are not recommended for those who are concerned about their health.";
		canBeSplit = 1;
		count = 20;
	};
	class Expansion_CigarettePack_Merkur: CigarettePack_Merkur
	{
		scope = 2;
		displayName = "Cigarette Pack - Chernamorka";
		descriptionShort = "Merkur cigarettes come in a sleek and stylish package with a white color. The logo features a golden crescent star with the brand name in back letters. The cigarettes themselves have a smooth and mild flavor, perfect for a relaxing break in the midst of a stressful and dangerous world. Each pack contains 20 cigarettes.";
		canBeSplit = 1;
		count = 20;
	};
	class Expansion_CigarettePack_Partyzanka: CigarettePack_Partyzanka
	{
		scope = 2;
		displayName = "Cigarette Pack - Partyzanka";
		descriptionShort = "This pack of cigarettes has a distinct aroma of sweet vanilla and a hint of nutmeg. It is said to be a popular brand among rebels and revolutionaries due to its association with the Partisans. Partyzanka cigarettes are known for their smooth and mellow taste, making them a preferred choice for those who enjoy a relaxed smoking experience.";
		canBeSplit = 1;
		count = 20;
	};
	class HumanSteakMeat;
	class InfectedSteakMeat: HumanSteakMeat
	{
		scope = 2;
		displayName = "Infected Flesh";
		descriptionShort = "This piece of meat has been torn from the flesh of an infected. It's rotten and not fit for human consumption, but some desperate survivors might try to eat it anyway.";
	};
};
