class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Buildings
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Structures"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionQuestObjectWoodenCrate: HouseNoDestruct
	{
		scope=1;
		model="DZ\structures_bliss\Residential\Misc\Misc_WoodenCrate.p3d";
		forceFarBubble="true";
	};
	class ExpansionQuestObjectCardboardBox: HouseNoDestruct
	{
		scope=1;
		model="DZ\structures\Furniture\Decoration\box_c\box_c.p3d";
		forceFarBubble="true";
	};
	class ExpansionQuestObjectCardboardBoxMulti: HouseNoDestruct
	{
		scope=1;
		model="DZ\structures\Furniture\Decoration\box_c\box_c_multi.p3d";
		forceFarBubble="true";
	};
	class ExpansionQuestObjectGiftBox_Base: HouseNoDestruct
	{
		scope=1;
	};
	class ExpansionQuestObjectGiftBox_SmallBase: ExpansionQuestObjectGiftBox_Base
	{
		model="\DZ\gear\containers\ChristmasBox1.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox1",
			"ChristmasBox1_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_MediumBase: ExpansionQuestObjectGiftBox_Base
	{
		scope=1;
		model="\DZ\gear\containers\ChristmasBox2.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox2",
			"ChristmasBox2_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_LargeBase: ExpansionQuestObjectGiftBox_Base
	{
		scope=1;
		model="\DZ\gear\containers\ChristmasBox3.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox3",
			"ChristmasBox3_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_Small1: ExpansionQuestObjectGiftBox_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Small2: ExpansionQuestObjectGiftBox_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Small3: ExpansionQuestObjectGiftBox_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Small4: ExpansionQuestObjectGiftBox_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Medium1: ExpansionQuestObjectGiftBox_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Medium2: ExpansionQuestObjectGiftBox_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Medium3: ExpansionQuestObjectGiftBox_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Medium4: ExpansionQuestObjectGiftBox_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Large1: ExpansionQuestObjectGiftBox_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Large2: ExpansionQuestObjectGiftBox_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Large3: ExpansionQuestObjectGiftBox_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Large4: ExpansionQuestObjectGiftBox_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Base: HouseNoDestruct
	{
		scope=1;
	};
	class ExpansionQuestObjectGiftBox_Prop_SmallBase: ExpansionQuestObjectGiftBox_Prop_Base
	{
		model="\DZ\gear\containers\ChristmasBox1.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox1",
			"ChristmasBox1_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_MediumBase: ExpansionQuestObjectGiftBox_Prop_Base
	{
		scope=1;
		model="\DZ\gear\containers\ChristmasBox2.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox2",
			"ChristmasBox2_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_LargeBase: ExpansionQuestObjectGiftBox_Prop_Base
	{
		scope=1;
		model="\DZ\gear\containers\ChristmasBox3.p3d";
		hiddenSelections[]=
		{
			"ChristmasBox3",
			"ChristmasBox3_ribbon"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Small1: ExpansionQuestObjectGiftBox_Prop_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Small2: ExpansionQuestObjectGiftBox_Prop_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Small3: ExpansionQuestObjectGiftBox_Prop_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Small4: ExpansionQuestObjectGiftBox_Prop_SmallBase
	{
		displayName="Small Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Medium1: ExpansionQuestObjectGiftBox_Prop_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Medium2: ExpansionQuestObjectGiftBox_Prop_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Medium3: ExpansionQuestObjectGiftBox_Prop_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Medium4: ExpansionQuestObjectGiftBox_Prop_MediumBase
	{
		displayName="Medium Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Large1: ExpansionQuestObjectGiftBox_Prop_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper1_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_RED.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Large2: ExpansionQuestObjectGiftBox_Prop_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper2_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_SILVER.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Large3: ExpansionQuestObjectGiftBox_Prop_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper3_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
	};
	class ExpansionQuestObjectGiftBox_Prop_Large4: ExpansionQuestObjectGiftBox_Prop_LargeBase
	{
		displayName="Large Gift";
		hiddenSelectionsTextures[]=
		{
			"dz\gear\containers\data\christmas_paper4_CO.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GREEN.rvmat"
		};
	};
};
