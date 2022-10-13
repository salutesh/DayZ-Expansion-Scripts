/**
 * InventoryItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionUAZWheel extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "ExpansionUAZWheel":
				newWheel = "ExpansionUAZWheel_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class ExpansionUAZWheel_Ruined extends CarWheel_Ruined{};

class ExpansionBusWheel extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "ExpansionBusWheel":
				newWheel = "ExpansionBusWheel_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class ExpansionBusWheelDouble extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "ExpansionBusWheelDouble":
				newWheel = "ExpansionBusWheelDouble_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class ExpansionBusWheel_Ruined extends CarWheel_Ruined{};
class ExpansionBusWheelDouble_Ruined extends CarWheel_Ruined{};

class ExpansionTractorFrontWheel extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "ExpansionTractorFrontWheel":
				newWheel = "ExpansionTractorFrontWheel_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class ExpansionTractorBackWheel extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "ExpansionTractorBackWheel":
				newWheel = "ExpansionTractorBackWheel_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class ExpansionTractorFrontWheel_Ruined extends CarWheel_Ruined{};
class ExpansionTractorBackWheel_Ruined extends CarWheel_Ruined{};

/*class Expansion_Landrover_Wheel extends CarWheel
{
	override void EEKilled(Object killer)
	{
		string newWheel = "";
		switch( GetType() )
		{
			case "Expansion_Landrover_Wheel":
				newWheel = "Expansion_Landrover_Wheel_Ruined";
			break;
		}
		if ( newWheel != "" )
		{
			ReplaceWheelLambda lambda = new ReplaceWheelLambda ( this, newWheel, NULL);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
};

class Expansion_Landrover_Wheel_Ruined extends CarWheel_Ruined{};*/
