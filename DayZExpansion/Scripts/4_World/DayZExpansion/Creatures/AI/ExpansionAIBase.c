/**
 * ExpansionAIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionTargetThreat
{
	NONE = 0,
	PASSIVE,
	HOSTILE
};

enum ExpansionTargetType
{
	NONE = 0,
	PLAYER = 2,
	ZOMBIE = 4,
	ITEM = 8,
	POSITION = 16
};

enum ExpansionTargetTransportType
{
	FOOT = 0,
	VEHICLE,
	INVENTORY
};

enum ExpansionTransportType
{
	FOOT = 0,
	VEHICLE
};

enum ExpansionAIEvents
{
	START = 1,
	FALLING,
	VEHICLE,
	PICKUP_ITEM,
	WEAPON,
	END
};

class ExpansionAIBase extends DayZPlayerImplement
{
	private ref ExpansionAIWeaponManager m_WeaponManager;

  	void ExpansionAIBase() 
	{
		Init();
	}

	void Init()
	{
		m_WeaponManager = new ExpansionAIWeaponManager( NULL );
		m_WeaponManager.SetAI( this );
	}

  	void ~ExpansionAIBase()
	{
		if ( m_WeaponManager )
			delete m_WeaponManager;
	}

	ItemBase GetItemInHands()
	{
		return ItemBase.Cast( GetHumanInventory().GetEntityInHands() );
	}
};

class ExpansionSoldierBase extends ExpansionAIBase
{

};