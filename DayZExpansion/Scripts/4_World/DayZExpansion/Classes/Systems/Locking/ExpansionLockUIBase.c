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