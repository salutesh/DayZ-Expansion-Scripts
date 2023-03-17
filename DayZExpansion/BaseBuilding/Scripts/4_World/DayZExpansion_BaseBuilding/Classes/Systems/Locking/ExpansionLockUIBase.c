/**
 * ExpansionLockUIBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLockUIBase: ExpansionUIScriptedMenu
{
	protected ref EffectSound m_Sound;
	PlayerBase m_Player;

	protected ItemBase m_Target;
	protected string m_Selection;

	protected string m_Code;
	protected int m_CodeLength;
	protected bool m_HasPin;
	protected bool m_RpcChange;
	protected bool m_Confirm;
	protected string m_ConfirmCode;

	protected TextWidget m_TextCodePanel;

	void ExpansionLockUIBase()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Insert( Update );

		m_CodeLength = 4;

		m_Target = NULL;
		m_Selection = "";

		m_Sound = new EffectSound;
		
		if (GetGame() && (!GetGame().IsDedicatedServer())) 
		{
			m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		}
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
		if ( m_Target )
		{
			m_HasPin = m_Target.HasCode();

			if ( !m_RpcChange && m_HasPin )
				m_CodeLength = m_Target.GetCodeLength();

			if ( m_Code != "" )
			{
				if ( !HidePinCode() )
				{
					m_TextCodePanel.SetText( m_Code );
				} else
				{
					string replaced_code;
					for ( int i = 0; i < m_Code.ToLower(); ++i )
					{
						replaced_code = replaced_code + "*";
					}
					
					m_TextCodePanel.SetText( replaced_code );
				}
			}
			else if ( m_CodeLength > 0 )
			{
				if ( m_ConfirmCode )
					m_TextCodePanel.SetText( "Confirm PIN" );
				else if ( !m_HasPin )
					m_TextCodePanel.SetText( "Set PIN(" + m_CodeLength + ")" );
				else
					m_TextCodePanel.SetText( "Enter PIN(" + m_CodeLength + ")" );
			} else {
				m_TextCodePanel.SetText( "Error BaseBuildingSettings" );
				Error("Expansion BaseBuildingSettings.json - CodeLockLength is inferior to 1 or can't be read !!! Validate the file or compare your file with this link https://github.com/ExpansionModTeam/DayZ-Expansion-Settings/blob/master/BaseBuildingSettings.json");
			}
		} else {
			m_TextCodePanel.SetText( "Error #1" );
		}
	}

	void SetChangeCodelock( bool state )
	{
		m_RpcChange = state;
	}

	void SetConfirm( bool state )
	{
		m_Confirm = state;
	}
	
	void SetTarget( ItemBase target, string selection )
	{
		m_Target = target;
		m_Selection = selection;

		RefreshCode();
	}

	void ShowLockState()
	{
	}

	protected void Process()
	{
		if ( m_Code.Length() == 0 || m_Code.Length() > m_CodeLength )
		{
			SoundOnError();
		} else if ( m_Confirm )
		{
			if ( m_ConfirmCode == "" )
			{
				m_ConfirmCode = m_Code;
				m_Code = "";
				RefreshCode();
			} else
			{
				if ( m_Code == m_ConfirmCode )
				{
					SendRPC();
				} else
				{
					SoundOnError();
				}
			}
		} else
		{
			SendRPC();
		}
	}
	
	protected void SendRPC()
	{		
		auto rpc = ExpansionScriptRPC.Create();

		int rpcType = ExpansionLockRPC.UNLOCK;
		if ( !m_HasPin )
			rpcType = ExpansionLockRPC.SET;
		if ( m_RpcChange )
			rpcType = ExpansionLockRPC.CHANGE;

		rpc.Write( m_Code );
		rpc.Write( m_Selection );
		rpc.Send( m_Target, rpcType, true );
	}
	
	void OnServerResponse( bool success, bool injurePlayer )
	{
		if ( injurePlayer )
		{
			OnInjurePlayer();
		}

		if ( success )
		{
			SoundOnSuccess();
			Close();
		} else
		{
			SoundOnError();
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

		if ( GetExpansionSettings().GetBaseBuilding() )
			m_CodeLength = GetExpansionSettings().GetBaseBuilding().CodeLockLength;

		RefreshCode();
		
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);

		SetFocus( layoutRoot );
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

	protected void SoundOnclick()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_Click_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
	
	protected void SoundOnReset()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound( "Expansion_ClickBeeps_SoundSet", m_Player.GetPosition() );
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}

	protected void SoundOnError()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_Denied_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}

	protected void SoundOnSuccess()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_Succes_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
}