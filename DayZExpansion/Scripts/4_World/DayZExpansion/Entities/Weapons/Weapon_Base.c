/**
 * Weapon_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

void FoldOpticsDown( EntityAI item )
{
	TStringArray selectionNames = new TStringArray;
	item.ConfigGetTextArray( "simpleHiddenSelections", selectionNames );

	item.SetSimpleHiddenSelectionState( selectionNames.Find( "folding_raised" ), false );
	item.SetSimpleHiddenSelectionState( selectionNames.Find( "folding_lowered" ), true );
}

void FoldOpticsUp( EntityAI item )
{
	TStringArray selectionNames = new TStringArray;
	item.ConfigGetTextArray( "simpleHiddenSelections", selectionNames );

	item.SetSimpleHiddenSelectionState( selectionNames.Find( "folding_raised" ), true );
	item.SetSimpleHiddenSelectionState( selectionNames.Find( "folding_lowered" ), false );
}

modded class Weapon_Base
{
	private int m_ExShouldFire;
	private autoptr array< int > m_ExMuzzleIndices;
	
	void Weapon_Base()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::Weapon_Base - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::Weapon_Base - Class: " + this.ToString());
		#endif
		
		m_ExMuzzleIndices = new array< int >;

		SetFlags( EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false );
		SetEventMask( EntityEvent.SIMULATE );

		UpdateLaser();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::Weapon_Base - End");
		#endif
	}

	override void EOnSimulate( IEntity other, float dt )
	{
		//super.EOnSimulate( other, dt );

		UpdateLaser();
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit( player );

		UpdateLaser();
	}

	override void UpdateLaser()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::UpdateLaser - Start");
		#endif
		
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return;

		if ( !GetInventory() )
			return;

		ItemBase laser = ItemBase.Cast( FindAttachmentBySlotName( "weaponFlashlight" ) );

		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::UpdateLaser - Laser: " + laser.ToString());
		#endif

		if ( laser )
		{
			laser.UpdateLaser();
		}
	}

	float CalculateBarrelLength()
	{
		vector usti_hlavne_position = GetSelectionPositionLS( "usti hlavne" );
		vector konec_hlavne_position = GetSelectionPositionLS( "konec hlavne" );
		return vector.Distance( usti_hlavne_position, konec_hlavne_position );
	}
	
	void ExpansionSetNextFire( int muzzleIndex )
	{
		m_ExShouldFire++;
		m_ExMuzzleIndices.Insert( muzzleIndex );
	}

	vector GetFirePosition( DayZPlayerImplement player )
	{
		if ( !player.IsAlive() )
			return "0 0 0";
		return player.GetBonePositionWS( player.GetBoneIndexByName( "Weapon_Bone_06" ) );
	}

	void ExpansionFire( PlayerBase player, vector direction )
	{
		if ( m_ExShouldFire <= 0 )
			return;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::ExpansionFire Start");
		#endif

		if ( player )
		{
			int muzzleIndex = m_ExMuzzleIndices[0];

			vector position = GetFirePosition( player ) + ( direction * CalculateBarrelLength() );

			ExpansionWeaponFireBase fireBase = ExpansionWeaponFireBase.Cast( GetExpansionFireType().Spawn() );

			if ( fireBase )
			{
				fireBase.FireServer( this, muzzleIndex, player, position, direction );
			}
		}

		m_ExShouldFire--;
		m_ExMuzzleIndices.Remove( 0 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::ExpansionFire End");
		#endif
	}

	typename GetExpansionFireType()
	{
		return ExpansionWeaponFireBase;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionCycleOpticsMode);		
	}
}