/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandVehicle : ExpansionHumanCommandScript
{
	void ExpansionHumanCommandVehicle(DayZPlayerImplement player, ExpansionHumanST table, Object vehicle, int seatIdx, int seat_anim, bool fromUnconscious)
	{
	}

	void LeaveVehicle()
	{
	}

	void GetOutVehicle()
	{
	}

	void JumpOutVehicle()
	{
	}

	void SwitchSeat(int seatIdx, int seatAnimType)
	{
	}

	bool IsGettingOut()
	{
		return false;
	}

	bool IsGettingIn()
	{
		return false;
	}

	bool IsSwitchSeat()
	{
		return false;
	}

	Object GetObject()
	{
		return null;
	}

	Transport GetTransport()
	{
		return null;
	}

#ifdef EXPANSIONMODVEHICLE
	ExpansionVehicleBase GetExpansionVehicle()
	{
		return null;
	}
#endif

	int GetVehicleSeat()
	{
		return -1;
	}

	void SetVehicleType(int vehicleType)
	{
	}

	void KeepInVehicleSpaceAfterLeave(bool state)
	{
	}

	bool WasGearChange()
	{
		return false;
	}

	void SetClutchState(bool state)
	{
	}
};
