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

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttachOnSelection);
	}
};

class ExpansionUAZWheel_Ruined extends CarWheel{};

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

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttachOnSelection);
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

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttachOnSelection);
	}
};

class ExpansionBusWheel_Ruined extends CarWheel{};
class ExpansionBusWheelDouble_Ruined extends CarWheel{};

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

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttachOnSelection);
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

	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttachOnSelection);
	}
};

class ExpansionTractorFrontWheel_Ruined extends CarWheel{};
class ExpansionTractorBackWheel_Ruined extends CarWheel{};