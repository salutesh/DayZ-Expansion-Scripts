/**
 * ExpansionActionLockVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/** @class		ExpansionCodeLockUI
 * @brief Handles the UI for the CodeLock		
 */

class ExpansionCodeLockUI extends ExpansionLockUIBase
{	
	protected ButtonWidget m_Button0;
	protected TextWidget m_Button0_Text;
	protected ButtonWidget m_Button1;
	protected TextWidget m_Button1_Text;
	protected ButtonWidget m_Button2;
	protected TextWidget m_Button2_Text;
	protected ButtonWidget m_Button3;
	protected TextWidget m_Button3_Text;
	protected ButtonWidget m_Button4;
	protected TextWidget m_Button4_Text;
	protected ButtonWidget m_Button5;
	protected TextWidget m_Button5_Text;
	protected ButtonWidget m_Button6;
	protected TextWidget m_Button6_Text;
	protected ButtonWidget m_Button7;
	protected TextWidget m_Button7_Text;
	protected ButtonWidget m_Button8;
	protected TextWidget m_Button8_Text;
	protected ButtonWidget m_Button9;
	protected TextWidget m_Button9_Text;
	
	protected ButtonWidget m_ButtonOK;
	protected TextWidget m_ButtonOK_Text;
	protected ButtonWidget m_ButtonClear;
	protected TextWidget m_ButtonClear_Text;
	protected ButtonWidget m_ButtonHash;
	protected TextWidget m_ButtonHash_Text;
	
	protected ImageWidget m_LockStateIcon;
	
	protected int m_HighlightColor = ARGB(255, 255, 255, 255);
	protected int m_NormalColor = ARGB(255, 88, 88, 88);
	protected int m_LockedColor = ARGB(255, 231, 76, 60);
	protected int m_UnlockedColor = ARGB(255, 46, 204, 113);
	
	override void ShowLockState()
	{
		if ( m_Target )
		{
			if ( m_Target.IsLocked() )
			{
				m_LockStateIcon.SetColor(m_LockedColor);
			} else
			{
				m_LockStateIcon.SetColor(m_UnlockedColor);
			}
		}
	}

	override Widget Init()
	{
		SetID( MENU_EXPANSION_CODELOCK_MENU );
		
		m_Code = "";
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/expansion_codelock.layout");
		
		m_Button0 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode0") );
		m_Button0_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode0_Text") );
		m_Button1 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode1") );
		m_Button1_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode1_Text") );
		m_Button2 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode2") );
		m_Button2_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode2_Text") );
		m_Button3 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode3") );
		m_Button3_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode3_Text") );
		m_Button4 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode4") );
		m_Button4_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode4_Text") );
		m_Button5 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode5") );
		m_Button5_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode5_Text") );
		m_Button6 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode6") );
		m_Button6_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode6_Text") );
		m_Button7 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode7") );
		m_Button7_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode7_Text") );
		m_Button8 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode8") );
		m_Button8_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode8_Text") );		
		m_Button9 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode9") );
		m_Button9_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCode9_Text") );	
		
		m_ButtonOK = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeOK") );	
		m_ButtonOK_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeOK_Text") );
		
		m_ButtonHash = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeHash") );	
		m_ButtonHash_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeHash_Text") );
		m_ButtonHash_Text.SetText("#STR_EXPANSION_HASHTAG");
		
		m_ButtonClear = ButtonWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeClear") );
		m_ButtonClear_Text = TextWidget.Cast( layoutRoot.FindAnyWidget("ButtonCodeClear_Text") );		
		
		m_TextCodePanel = TextWidget.Cast( layoutRoot.FindAnyWidget("CodeText") );
		m_TextCodePanel.SetText( m_Code );
		
		m_LockStateIcon = ImageWidget.Cast( layoutRoot.FindAnyWidget("LockState") );
		
		RefreshCode();
		ShowLockState();
		
		return layoutRoot;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);
		
		if ( w == m_Button0 )
		{
			m_Button0_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button1 )
		{
			m_Button1_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button2 )
		{
			m_Button2_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button3 )
		{
			m_Button3_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button4 )
		{
			m_Button4_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button5 )
		{
			m_Button5_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button6 )
		{
			m_Button6_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button7 )
		{
			m_Button7_Text.SetColor( m_HighlightColor );
			
			return true;
		}	
		else if ( w == m_Button8 )
		{
			m_Button8_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_Button9 )
		{
			m_Button9_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_ButtonOK )
		{
			m_ButtonOK_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_ButtonClear )
		{
			m_ButtonClear_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_ButtonHash )
		{
			m_ButtonHash_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		
	 	return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		if ( w == m_Button0 )
		{
			m_Button0_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button1 )
		{
			m_Button1_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button2 )
		{
			m_Button2_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button3 )
		{
			m_Button3_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button4 )
		{
			m_Button4_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button5 )
		{
			m_Button5_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button6 )
		{
			m_Button6_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button7 )
		{
			m_Button7_Text.SetColor( m_NormalColor );
			
			return true;
		}	
		else if ( w == m_Button8 )
		{
			m_Button8_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_Button9 )
		{
			m_Button9_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_ButtonOK )
		{
			m_ButtonOK_Text.SetColor( m_HighlightColor );
			
			return true;
		}
		else if ( w == m_ButtonClear )
		{
			m_ButtonClear_Text.SetColor( m_NormalColor );
			
			return true;
		}
		else if ( w == m_ButtonHash )
		{
			m_ButtonHash_Text.SetColor( m_NormalColor );
			
			return true;
		}
		
	 	return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (m_Code.Length() < m_CodeLength)
		{
			switch(w)
			{
				case m_Button0:
				{
					m_Code += "0";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button1:
				{
					m_Code += "1";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button2:
				{
					m_Code += "2";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button3:
				{
					m_Code += "3";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button4:
				{
					m_Code += "4";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button5:
				{
					m_Code += "5";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button6:
				{
					m_Code += "6";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button7:
				{
					m_Code += "7";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button8:
				{
					m_Code += "8";
					RefreshCode();
					SoundOnclick();
					break;
				}
				case m_Button9:
				{
					m_Code += "9";
					RefreshCode();
					SoundOnclick();
					break;
				}
			}
		}

		if ( w == m_ButtonOK || w == m_ButtonHash )
		{
			RefreshCode();
			SoundOnclick();

			Process();

			return true;
		}

		if ( w == m_ButtonClear )
		{
			m_Code = "";
			
			RefreshCode();
			SoundOnReset();

			return true;
		}

		return false;
	}
	
	override void OnKeyPress( int key )
	{
		if ( m_Code.Length() < m_CodeLength )
		{
			// We can't do that because key are not sorted, wtf DayZ ? :(
			// if ( key >= KeyCode.KC_NUMPAD0 && key <= KeyCode.KC_NUMPAD9 )
			switch ( key )
			{
				case KeyCode.KC_0:
				case KeyCode.KC_NUMPAD0:
					m_Code += "0";
					SoundOnclick();
					break;
				case KeyCode.KC_1:
				case KeyCode.KC_NUMPAD1:
					m_Code += "1";
					SoundOnclick();
					break;
				case KeyCode.KC_2:
				case KeyCode.KC_NUMPAD2:
					m_Code += "2";
					SoundOnclick();
					break;
				case KeyCode.KC_3:
				case KeyCode.KC_NUMPAD3:
					m_Code += "3";
					SoundOnclick();
					break;
				case KeyCode.KC_4:
				case KeyCode.KC_NUMPAD4:
					m_Code += "4";
					SoundOnclick();
					break;
				case KeyCode.KC_5:
				case KeyCode.KC_NUMPAD5:
					m_Code += "5";
					SoundOnclick();
					break;
				case KeyCode.KC_6:
				case KeyCode.KC_NUMPAD6:
					m_Code += "6";
					SoundOnclick();
					break;
				case KeyCode.KC_7:
				case KeyCode.KC_NUMPAD7:
					m_Code += "7";
					SoundOnclick();
					break;
				case KeyCode.KC_8:
				case KeyCode.KC_NUMPAD8:
					m_Code += "8";
					SoundOnclick();
					break;
				case KeyCode.KC_9:
				case KeyCode.KC_NUMPAD9:
					m_Code += "9";
					SoundOnclick();
					break;
			}
			
			RefreshCode();
		}
		
		if ( key == KeyCode.KC_RETURN || key == KeyCode.KC_NUMPADENTER )
		{
			SoundOnclick();
			Process();
		}
		
		if ( key == KeyCode.KC_BACK )
		{
			if ( m_Code.Length() > 0 && !m_Code.Length() == 0 )
			{
				m_Code = ExpansionString.RemoveLastChar( m_Code );
				RefreshCode();
				SoundOnReset();
			}
		}
	}
}

