#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Gates
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionBarrierGateKit","ExpansionBarrierGateKitPlacing","ExpansionBarrierGate"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionBarrierGateKit: ExpansionKitLarge
	{
		scope = 2;
		rotationFlags = 4;
		displayName = "$STR_EXPANSION_GATE";
		descriptionShort = "$STR_EXPANSION_GATE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGateKit.p3d";
		itemSize[] = {10,10};
		physLayer = "item_large";
		itemBehaviour = 0;
		attachments[] = {};
	};
	class ExpansionBarrierGateKitPlacing: ExpansionBarrierGateKit
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGate_Preview.p3d";
		slopeTolerance = 0.85;
	};
	class ExpansionBarrierGate: ExpansionBaseBuilding
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "$STR_EXPANSION_GATE";
		descriptionShort = "$STR_EXPANSION_GATE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Gates\BarrierGate.p3d";
		allowOwnedCargoManipulation = 1;
		forceFarBubble = "true";
		carveNavmesh = 1;
		physLayer = "item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
				};
			};
		};
		class AnimationSources
		{
			class gate
			{
				source = "user";
				animPeriod = 2;
				initPhase = 0;
			};
		};
	};
};
