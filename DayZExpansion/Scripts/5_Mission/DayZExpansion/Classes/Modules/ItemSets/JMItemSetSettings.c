modded class JMItemSetSettings
{
	override void Defaults()
	{
		super.Defaults();

		DefaultExpansionWall();
		DefaultExpansionStair();
		DefaultExpansionFloor();
		DefaultExpansionRamp();
	}

	void DefaultExpansionWall()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionWallKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Wall";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionStair()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionStairKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 15 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 45 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Stair";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionFloor()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionFloorKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 4 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Floor";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}

	void DefaultExpansionRamp()
	{
		ref array< ref JMItemSetItemInfo > attArr = new array< ref JMItemSetItemInfo >;

		attArr.Insert( new JMItemSetItemInfo( "ExpansionRampKit", 1, 1 ) );
		attArr.Insert( new JMItemSetItemInfo( "WoodenPlank", 1, 10 ) );
		attArr.Insert( new JMItemSetItemInfo( "ExpansionLumber3", 1, 2 ) );
		attArr.Insert( new JMItemSetItemInfo( "Nail", 1, 30 ) );
		attArr.Insert( new JMItemSetItemInfo( "Hatchet", 1, 1 ) );

		ref JMItemSetSerialize file = new JMItemSetSerialize;

		file.Name = "Expansion Ramp";
		file.m_FileName = file.Name;
		file.ContainerClassName = "SeaChest";
		file.Items = attArr;

		ItemSets.Insert( file.Name, file );
	}
}