/**
 * Weapon_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Weapon_Base
{
	void Expansion_FoldOpticsDown()
	{
		TStringArray selectionNames = new TStringArray;
		ConfigGetTextArray( "simpleHiddenSelections", selectionNames );

		SetSimpleHiddenSelectionState( selectionNames.Find( "folding_raised" ), false );
		SetSimpleHiddenSelectionState( selectionNames.Find( "folding_lowered" ), true );
	}

	void Expansion_FoldOpticsUp()
	{
		TStringArray selectionNames = new TStringArray;
		ConfigGetTextArray( "simpleHiddenSelections", selectionNames );

		SetSimpleHiddenSelectionState( selectionNames.Find( "folding_raised" ), true );
		SetSimpleHiddenSelectionState( selectionNames.Find( "folding_lowered" ), false );
	}
	
	void ExpansionHideWeaponPart(string WeaponSelectionS, bool state)
	{
		TStringArray selectionNames = new TStringArray;
		ConfigGetTextArray( "simpleHiddenSelections", selectionNames );
		SetSimpleHiddenSelectionState( selectionNames.Find( WeaponSelectionS ), !state );
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit( player );

		UpdateLaser();
	}

	override void UpdateLaser()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "UpdateLaser");
#endif
		
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return;

		if ( !GetInventory() )
			return;

		ItemBase laser = ItemBase.Cast( FindAttachmentBySlotName( "weaponFlashlight" ) );

		if ( laser )
		{
			laser.UpdateLaser();
		}
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionCycleOpticsMode);		
	}
};
