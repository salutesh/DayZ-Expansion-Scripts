/**
 * JMItemSetSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMItemSetSettings
{
	override void Defaults()
	{
		super.Defaults();

		DefaultExpansionMarketMoney();
		DefaultExpansionVehicleTools();

		DefaultExpansionWoodWall();
		DefaultExpansionWoodStair();
		DefaultExpansionWoodFloor();
		DefaultExpansionWoodRamp();
		//DefaultExpansionWoodPillar();

		/*
		DefaultExpansionCementMixer();
		DefaultExpansionConcreteWall();
		DefaultExpansionConcreteStair();
		DefaultExpansionConcreteFloor();
		DefaultExpansionConcreteRamp();
		DefaultExpansionConcretePillar();
		*/
	}

	void DefaultExpansionMarketMoney()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverdBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverdBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverdBar", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldNugget", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldNugget", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionGoldNugget", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverNugget", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverNugget", 1, 100 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionSilverNugget", 1, 100 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Market Money";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionVehicleTools()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "TireRepairKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "PipeWrench", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "EpoxyPutty", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "CarBattery", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "CarRadiator", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "SparkPlug", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "CanisterGasoline", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "Canteen", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "Canteen", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "Canteen", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Vehicle Tools";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodWall()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionWallKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Wall";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodStair()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionStairKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 15 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 45 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Stair";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodFloor()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionFloorKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Floor";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodFoundation()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionFoundationKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 20 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 8 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 70 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Foundation";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodRamp()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionRampKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Ramp";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionWoodPillar()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionPillarKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Pillar";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionCementMixer()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionCementMixerWheel", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionCementMixerBox", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionCementMixerBody", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "CarBattery", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete BB tools";
		file.m_FileName = file.Name;
		file.ContainerClassName = "ExpansionCementMixer";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcreteWall()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionWallKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete Wall";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcreteStair()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionStairKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete Stair";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcreteFloor()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionFloorKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete Floor";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcreteFoundation()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionFoundationKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 8 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete Foundation";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcreteRamp()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionRampKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wood Ramp";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionConcretePillar()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionPillarKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Concrete Pillar";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}
}