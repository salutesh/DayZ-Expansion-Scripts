#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Expansion_Anomaly_Base: Inventory_Base
	{
		scope = 1;
		model = "\DayZExpansion\NamalskAdventure\Dta\Objects\Anomalies\Expansion_Anomaly.p3d";
		bounding = "BSphere";
		forceFarBubble = "true";
		openable = 1;
		itemBehaviour = 2;
		itemsCargoSize[] = {10,10};
		attachments[] = {"Att_ExpansionAnomalyCore"};
	};
	class Expansion_Anomaly_Singularity: Expansion_Anomaly_Base
	{
		scope = 2;
		displayName = "Anomaly - Singularity";
		descriptionShort = "PLACEHOLDER";
	};
};
