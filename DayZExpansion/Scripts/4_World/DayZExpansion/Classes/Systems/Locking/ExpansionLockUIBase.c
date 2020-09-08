/**
 * ExpansionLockUIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLockUIBase extends UIScriptedMenu
{
	void ExpansionLockUIBase()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Insert( Update );
	}

	void ~ExpansionLockUIBase()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( Update );
	}

	bool HidePinCode()
	{
		return !GetExpansionClientSettings().ShowPINCode;
	}

	void RefreshCode()
	{
	}
	
	void SetTarget( ItemBase target, string selection )
	{
		RefreshCode();
	}

	void ShowLockState()
	{
	}
	
	protected void SendRPC()
	{
	}
	
	void OnServerResponse( bool success, bool injurePlayer )
	{
		if ( injurePlayer )
		{
			OnInjurePlayer();
		}

		if ( success )
		{
			Close();
		} else
		{
			RefreshCode();
		}
	}

	void OnInjurePlayer()
	{

	}
	
	override void Update(float timeslice)
	{
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) )
		{
			Close();
			return;
		}

		RefreshCode();
		ShowLockState();
	}
	
	override void OnShow()
	{
		super.OnShow();

		RefreshCode();
		
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
	}
	
	override void OnHide()
	{
		super.OnHide();

		RefreshCode();

		GetGame().GetMission().PlayerControlEnable(true);
	}
	
	override bool UseKeyboard() 
	{ 
		return true; 
	}

	override bool UseMouse()
	{
		return true;
	}

	void OnKeyPress( int key )
	{
	}
}