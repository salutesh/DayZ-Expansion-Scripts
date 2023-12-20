/**
 * ExpansionGenerator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGenerator_Base: PowerGenerator
{
	void ~ExpansionGenerator_Base()
	{
		if (!GetGame())
			return;

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			ExpansionWorldObjectsModule module;
			if ( CF_Modules<ExpansionWorldObjectsModule>.Get(module) )
			{
				module.TurnOffGenerator( GetPosition() );
			}
		}
	}

	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			SetAnimationPhase("switch", 1.0);
		}
	}
	
	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			SetAnimationPhase("switch", 0.0);
		}
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			ExpansionWorldObjectsModule module;
			if ( CF_Modules<ExpansionWorldObjectsModule>.Get(module) )
			{
				module.TurnOnGenerator( GetPosition() );
			}
		}
	}
	
	override void OnWorkStop()
	{
		super.OnWorkStop();

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			ExpansionWorldObjectsModule module;
			if ( CF_Modules<ExpansionWorldObjectsModule>.Get(module) )
			{
				module.TurnOffGenerator( GetPosition() );
			}
		}
	}

	bool IsSwitch(string selection = "")
	{
		return selection == "switch";
	}

	bool IsFuelTank(string selection = "")
	{
		return selection == "fuel_tank";
	}

	override bool IsInventoryVisible()
	{
		return true;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
};

class ExpansionGenerator_Large: ExpansionGenerator_Base {};
class ExpansionGenerator_Small: ExpansionGenerator_Base {};