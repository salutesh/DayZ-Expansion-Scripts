/**
 * bldr_expansion_Airdrop_Plane.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class bldr_expansion_Airdrop_Plane: House
{
	float m_RotorAnimationPosition;

	// ------------------------------------------------------------
	// bldr_expansion_Airdrop_Plane Constructor
	// ------------------------------------------------------------
	void bldr_expansion_Airdrop_Plane()
	{
		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );
	}
    
	// ------------------------------------------------------------
	// bldr_expansion_Airdrop_Plane EOnSimulate
	// ------------------------------------------------------------
	override void EOnSimulate( IEntity other, float dt )
	{		
		m_RotorAnimationPosition += 1 * dt * 20.0 / ( Math.PI * 2.0 );
		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase( "rotor", m_RotorAnimationPosition );
	}
};