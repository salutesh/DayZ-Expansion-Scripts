#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_TripWireTrap
	{
		units[] = {"ExpansionTripwireTrap"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Traps"};
	};
};
class CfgSlots
{
	class Slot_tripWireAttachment
	{
		name = "tripWireAttachment";
		displayName = "$STR_EXPANSION_TRIP_WIRE_TRAP";
		expansionGhostIcon = "DayzExpansion\Core\GUI\icons\misc\T_Grenade_256x256.edds";
	};
};
class CfgVehicles
{
	class Trap_Base;
	class TripwireTrap;
	class ExpansionTripwireTrap: TripwireTrap
	{
		scope = 2;
		displayName = "$STR_EXPANSION_TRIP_WIRE_TRAP";
		descriptionShort = "$STR_EXPANSION_TRIP_WIRE_TRAP_DESC";
		model = "\dz\gear\traps\tripwire.p3d";
		rotationFlags = 17;
		lootCategory = "Crafted";
		attachments[] = {"tripWireAttachment"};
		slopeTolerance = 0.3;
		yawPitchRollLimit[] = {60,60,60};
		weight = 60;
		itemSize[] = {1,3};
		itemBehaviour = 1;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"dz\gear\crafting\data\rabbit_snare_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\crafting\data\rabbit_snare.rvmat"};
		hologramMaterial = "rabbit_snare";
		hologramMaterialPath = "dz\gear\crafting\data";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 150;
					healthLevels[] = {{1,{"DZ\gear\crafting\data\rabbit_snare.rvmat"}},{0.7,{"DZ\gear\crafting\data\rabbit_snare.rvmat"}},{0.5,{"DZ\gear\crafting\data\rabbit_snare_damage.rvmat"}},{0.3,{"DZ\gear\crafting\data\rabbit_snare_damage.rvmat"}},{0,{"DZ\gear\crafting\data\rabbit_snare_destruct.rvmat"}}};
				};
			};
		};
		class AnimationSources
		{
			class HiddenByDefault
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class ShownByDefault
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class inventory: ShownByDefault{};
			class placing: HiddenByDefault{};
			class TripWire_set_can: HiddenByDefault{};
			class TripWire_set_grenade: HiddenByDefault{};
			class TripWire_triggered: HiddenByDefault{};
			class s1_flashbang: HiddenByDefault{};
			class s1_frag_RGD5: HiddenByDefault{};
			class s1_grenade: HiddenByDefault{};
			class s1_Road_flare: HiddenByDefault{};
			class s1_smokegrenade: HiddenByDefault{};
			class s1_smokegrenade_rdg2: HiddenByDefault{};
			class s1_SodaCan_Used: HiddenByDefault{};
			class s2_flashbang: HiddenByDefault{};
			class s2_frag_RGD5: HiddenByDefault{};
			class s2_grenade: HiddenByDefault{};
			class s2_Road_flare: HiddenByDefault{};
			class s2_smokegrenade: HiddenByDefault{};
			class s2_smokegrenade_rdg2: HiddenByDefault{};
			class s2_SodaCan_Used: HiddenByDefault{};
			class s3_flashbang: HiddenByDefault{};
			class s3_frag_RGD5: HiddenByDefault{};
			class s3_grenade: HiddenByDefault{};
			class s3_Road_flare: HiddenByDefault{};
			class s3_smokegrenade: HiddenByDefault{};
			class s3_smokegrenade_rdg2: HiddenByDefault{};
			class s3_SodaCan_Used: HiddenByDefault{};
		};
	};
};
