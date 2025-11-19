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
		g_Game.GetUpdateQueue( CALL_CATEGORY_GUI ).Insert( Update );

		m_CodeLength = 4;

		m_Target = NULL;
		m_Selection = "";

		m_Sound = new EffectSound;
		
		if (g_Game && (!g_Game.IsDedicatedServer())) 
		{
			m_Player = PlayerBase.Cast( g_Game.GetPlayer() );
		}
	}

	void ~ExpansionLockUIBase()
	{
		g_Game.GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( Update );
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
				m_TextCodePanel.SetText( "Settings Error" );
				EXError.Error(null, "Expansion BaseBuildingSettings.json - CodeLockLength is less than 1! Validate the file or delete it for it to be re-generated on next server start", {});
			}
		} else {
			m_TextCodePanel.SetText( "Critical Error" );
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
		int rpcType = ItemBase.s_Expansion_Unlock_RPCID;
		if ( !m_HasPin )
			rpcType = ItemBase.s_Expansion_SetCode_RPCID;
		if ( m_RpcChange )
			rpcType = ItemBase.s_Expansion_ChangeCode_RPCID;

		auto rpc = ExpansionScriptRPC.Create(rpcType);

		rpc.Write( m_Code );
		rpc.Write( m_Selection );
		rpc.Expansion_Send(m_Target, true);
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
		if ( g_Game.GetInput().LocalPress( "UAUIBack", false ) )
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
		
		g_Game.GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);

		SetFocus( layoutRoot );
	}
	
	override void OnHide()
	{
		super.OnHide();

		RefreshCode();

		g_Game.GetMission().PlayerControlEnable(true);
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
		if ( !g_Game.IsDedicatedServer() )
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
		if ( !g_Game.IsDedicatedServer() )
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
		if ( !g_Game.IsDedicatedServer() )
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
		if ( !g_Game.IsDedicatedServer() )
		{
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_Succes_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
}