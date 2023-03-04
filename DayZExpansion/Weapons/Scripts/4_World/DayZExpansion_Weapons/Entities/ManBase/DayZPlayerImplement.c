/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	protected bool m_WasRaisedIronsight;

	override void HandleWeapons( float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights )
	{
		super.HandleWeapons( pDt, pInHands, pInputs, pExitIronSights );

		if ( m_WasRaisedIronsight == ( m_MovementState.IsRaised() && m_CameraIronsight && !m_CameraOptics ) )
			return;

		m_WasRaisedIronsight = m_MovementState.IsRaised() && m_CameraIronsight && !m_CameraOptics;

		Weapon_Base weapon = Weapon_Base.Cast( pInHands );
		if ( weapon )
		{
			ItemOptics optic = weapon.GetAttachedOptics();
			if ( optic )
			{
				if ( optic.IsKindOf( "ExpansionHAMROptic" ) )
				{
					// Check for secondary optic and switch it on/off appropriately when going in/out of optics view
					ItemOptics secondaryOptic = ItemOptics.Cast( optic.FindAttachmentBySlotName( "pistolOptics" ) );
					if ( secondaryOptic && secondaryOptic.HasEnergyManager() )
					{
						if ( m_CameraIronsight && !m_CameraOptics )
						{
							secondaryOptic.GetCompEM().SwitchOn();
							secondaryOptic.HideSelection("hide");
						} else
						{
							secondaryOptic.GetCompEM().SwitchOff();
							secondaryOptic.ShowSelection("hide");
						}
					}
				}
			}
		}
	}
};
