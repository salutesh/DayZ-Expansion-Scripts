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

		switch (NVtype)
		{
			case NVTypes.NONE:
				PPEffects.SetEVValuePP(0);
				PPEffects.SetColorizationNV(1.0, 1.0, 1.0);
				PPEffects.SetNVParams(1.0, 0.0, 2.35, 2.75); //default values
			break;
			
			case NVTypes.NV_OPTICS_ON:
				PPEffects.SetEVValuePP(7);
				PPEffects.SetColorizationNV(0.0, 1.0, 0.2); // Default: 0.0, 1.0, 0.0
				PPEffects.SetNVParams(2.0, 1.0, 9.0, 1.0);  // Default: 3.0, 2.0, 9.0, 1.0
			break;
			
			case NVTypes.NV_OPTICS_OFF:
				PPEffects.SetEVValuePP(-10);
				PPEffects.SetColorizationNV(0.0, 0.0, 0.0);
				PPEffects.SetNVParams(1.0, 0.0, 2.35, 2.75); //default values
			break;
			
			case NVTypes.NV_GOGGLES:
				PPEffects.SetEVValuePP(7);
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
