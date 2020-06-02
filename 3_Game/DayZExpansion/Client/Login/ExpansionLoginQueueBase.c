/**
 * ExpansionLoginQueueBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoginQueueBase
{
	protected ImageWidget m_ImageBackground;
	autoptr array<ref ExpansionLoadingScreenBackground> m_Backgrounds;
	
	void LoginQueueBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::LoginQueueBase - Start");
		#endif
		
		JsonFileLoader< ref array< ref ExpansionLoadingScreenBackground > >.JsonLoadFile( "DayZExpansion/Scripts/Data/LoadingImages.json", m_Backgrounds );
	
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::LoginQueueBase - End");
		#endif
	}
	
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::Init - Start");
		#endif
			
		super.Init();
		
		m_ImageBackground = ImageWidget.Cast( layoutRoot.FindAnyWidget("Background") );
		
		m_ImageBackground.LoadImageFile( 0, m_Backgrounds.GetRandomElement().Path );

		return layoutRoot;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoginQueueBase::Init - End");
		#endif
	}
}
