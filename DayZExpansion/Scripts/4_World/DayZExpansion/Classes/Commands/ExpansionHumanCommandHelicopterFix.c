/**
 * ExpansionHumanCommandHelicopterFix.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// This is the attempted fix for helicopters crashing after explosion code.
// Original code which might have been causing the crashes was written by CaminoOnPc
// This is done to prevent calling GetOutVehicle in HumanCommandVehicle
/*
class ExpansionHumanCommandHelicopterFix extends HumanCommandScript
{
	private PlayerBase m_Player;
	private ExpansionHumanST m_Table;

	void ExpansionHumanCommandHelicopterFix( Human pHuman, ExpansionHumanST pTable )
	{
		Class.CastTo( m_Player, pHuman );
		m_Table = pTable;
	}

	override void OnActivate()
	{
		m_Player.UnlinkFromLocalSpace();
		m_Table.CallVehicleGetOut( this, 0, 0 );
	}

	override bool PostPhysUpdate( float pDt )
	{
		return false; // force state to end
	}
};
*/