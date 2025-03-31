class CfgPatches
{
	class DayZExpansion_Objects_Structures_Debug
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Gear_Tools"
		};
	};
};
class CfgVehicles
{
	class Hammer;
	class HouseNoDestruct;
	class Inventory_Base;
	class ExpansionAdminHammer: Hammer
	{
		scope=2;
		displayName="Admin Hammer";
		expansionIsAdminTool=1;
	};
	class ExpansionDebugObject: HouseNoDestruct
	{
		hiddenSelections[]=
		{
			"texture"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,0,1,co)"
		};
	};
	class ExpansionDebugArrow: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugArrow.p3d";
		class AnimationSources
		{
			class scale
			{
				source="user";
				initPhase=0;
				animPeriod=0.001;
			};
		};
	};
	class ExpansionDebugArrow_Red: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugArrow_Orange: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugArrow_Yellow: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugArrow_Blue: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugArrow_Cyan: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugArrow_Purple: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugArrow_White: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugArrow_Black: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugBox: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugBox.p3d";
	};
	class ExpansionDebugBox_Red: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugBox_Orange: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugBox_Yellow: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugBox_Blue: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugBox_Cyan: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugBox_Purple: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugBox_White: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugBox_Black: ExpansionDebugBox
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugCone: ExpansionDebugObject
	{
		scope=2;
		model="\DayZExpansion\Dta\Core\cone.p3d";
		hiddenSelections[]=
		{
			"body"
		};
	};
	class ExpansionDebugCone_Red: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugCone_Orange: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugCone_Yellow: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugCone_Blue: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugCone_Cyan: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugCone_Purple: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugCone_White: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugCone_Black: ExpansionDebugCone
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugConeSmall: ExpansionDebugCone
	{
		scope=2;
		model="\DayZExpansion\Dta\Core\coneSmall.p3d";
		hiddenSelections[]=
		{
			"body"
		};
	};
	class ExpansionDebugConeSmall_Red: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Orange: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Yellow: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Blue: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Cyan: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Purple: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugConeSmall_White: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugConeSmall_Black: ExpansionDebugConeSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugCube: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugCube.p3d";
	};
	class ExpansionDebugCube_Red: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugCube_Orange: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugCube_Yellow: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugCube_Blue: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugCube_Cyan: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugCube_Purple: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugCube_White: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugCube_Black: ExpansionDebugCube
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugCylinder: ExpansionDebugObject
	{
		scope=2;
		hiddenSelections[]=
		{
			"color"
		};
		model="DayZExpansion\Objects\Structures\Debug\DebugCylinder.p3d";
	};
	class ExpansionDebugCylinder_Red: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugCylinder_Orange: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugCylinder_Yellow: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugCylinder_Blue: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugCylinder_Cyan: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugCylinder_Purple: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugCylinder_White: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugCylinder_Black: ExpansionDebugCylinder
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugLine: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugLine.p3d";
	};
	class ExpansionDebugLine_Red: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugLine_Orange: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugLine_Yellow: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugLine_Blue: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugLine_Cyan: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugLine_Purple: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugLine_White: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugLine_Black: ExpansionDebugLine
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugNoticeMe: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugNoticeMe.p3d";
	};
	class ExpansionDebugNoticeMe_Red: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Orange: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Yellow: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Blue: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Cyan: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Purple: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_White: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugNoticeMe_Black: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugRod: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugRod.p3d";
	};
	class ExpansionDebugRod_Red: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugRod_Orange: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugRod_Yellow: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugRod_Blue: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugRod_Cyan: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugRod_Purple: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugRod_White: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugRod_Black: ExpansionDebugRod
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugRodBig: ExpansionDebugRod
	{
		model="DayZExpansion\Objects\Structures\Debug\DebugRodBig.p3d";
	};
	class ExpansionDebugRodBig_Red: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugRodBig_Orange: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugRodBig_Yellow: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugRodBig_Blue: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugRodBig_Cyan: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugRodBig_Purple: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugRodBig_White: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugRodBig_Black: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugSphereSmall: ExpansionDebugObject
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugSphereSmall.p3d";
	};
	class ExpansionDebugSphereSmall_Red: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Orange: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0.5,0,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Yellow: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,0,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Blue: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,1,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Cyan: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,1,1,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Purple: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0,1,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_White: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1,co)"
		};
	};
	class ExpansionDebugSphereSmall_Black: ExpansionDebugSphereSmall
	{
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0,0,0,1,co)"
		};
	};
	class ExpansionDebugGoat: Inventory_Base
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugGoat.p3d";
		displayName="Expansion Debug Goat";
		descriptionShort="Debug goat";
		itemBehaviour=2;
		itemSize[]={10,100};
		carveNavmesh=1;
		weight=45000;
		physLayer="item_large";
		allowOwnedCargoManipulation=1;
		overrideDrawArea="8.0";
		forceFarBubble="true";
		attachments[]=
		{
			"ExpansionDebugGoat_Attachment"
		};
		rotationFlags=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000000;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
		};
		soundImpactType="textile";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem_Light
				{
					soundSet="GoatBleat_B_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="GoatBleat_C_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="GoatBleat_D_SoundSet";
					id=898;
				};
			};
		};
	};
	class ExpansionDebugGoat_Attachment: Inventory_Base
	{
		scope=2;
		model="DayZExpansion\Objects\Structures\Debug\DebugBox.p3d";
		inventorySlots[]=
		{
			"ExpansionDebugGoat_Attachment"
		};
	};
};
class CfgSlots
{
	class Slot_ExpansionDebugGoat_Attachment
	{
		name="ExpansionDebugGoat_Attachment";
		displayName="";
		ghostIcon="";
		show="false";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyExpansionDebugGoat_Attachment: ProxyAttachment
	{
		model="\dz\animals\capra_hircus\capra_hircus.p3d";
		inventorySlot="ExpansionDebugGoat_Attachment";
	};
};
