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

modded class Weapon_Base
{
	private int m_ExShouldFire;
	private autoptr array< int > m_ExMuzzleIndices;
	
	void Weapon_Base()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::Weapon_Base - Start");
		#endif
		
		m_ExMuzzleIndices = new array< int >;

		#ifdef EXPANSIONEXPRINT
		EXPrint("Weapon_Base::Weapon_Base - End");
		#endif
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