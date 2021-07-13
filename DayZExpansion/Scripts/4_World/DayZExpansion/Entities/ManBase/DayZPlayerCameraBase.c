/**
 * DayZPlayerCameraBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCameraBase
{
	override void SetNVPostprocess(int NVtype)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCameraBase::SetNVPostprocess Start");
		#endif
		
		//! No calling super here as it would break NVG brightness on Namalsk,
		//! ours is a complete replacement for vanilla/Namalsk setup.
		//! Sorry other modders, but if you want to change NVG postprocess,
		//! you have to require DayZExpansion_Scripts
		
		switch (NVtype)
		{
			case NVTypes.NONE:
				#ifdef NAMALSK_TERRAIN
				PPEffects.SetEyeAccNV(1);
				#else
				PPEffects.SetEVValuePP(0);
				#endif
				PPEffects.SetColorizationNV(1.0, 1.0, 1.0);
				PPEffects.SetNVParams(1.0, 0.0, 2.35, 2.75); //default values
			break;
			
			case NVTypes.NV_OPTICS_ON:
				#ifdef NAMALSK_TERRAIN
				PPEffects.SetEyeAccNV(20);
				#else
				PPEffects.SetEVValuePP(7);
				#endif
				PPEffects.SetColorizationNV(0.0, 1.0, 0.2); // Default: 0.0, 1.0, 0.0
				PPEffects.SetNVParams(2.0, 1.0, 9.0, 1.0);  // Default: 3.0, 2.0, 9.0, 1.0
			break;
			
			case NVTypes.NV_OPTICS_OFF:
				#ifdef NAMALSK_TERRAIN
				PPEffects.SetEyeAccNV(-1);
				#else
				PPEffects.SetEVValuePP(-10);
				#endif
				PPEffects.SetColorizationNV(0.0, 0.0, 0.0);
				PPEffects.SetNVParams(1.0, 0.0, 2.35, 2.75); //default values
			break;
			
			case NVTypes.NV_GOGGLES:
				#ifdef NAMALSK_TERRAIN
				PPEffects.SetEyeAccNV(20);
				#else
				PPEffects.SetEVValuePP(7);
				#endif
				PPEffects.SetColorizationNV(0.0, 1.0, 0.2); // Default: 0.0, 1.0, 0.0
				PPEffects.SetNVParams(2.0, 1.0, 9.0, 1.0);  // Default: 3.0, 2.0, 9.0, 1.0
			break;
		}
		
		if (PlayerBaseClient.Cast(m_pPlayer))
		{
			PlayerBaseClient.Cast(m_pPlayer).SwitchPersonalLight(NVtype < 1); //TODO 
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCameraBase::SetNVPostprocess End");
		#endif
	}
}
