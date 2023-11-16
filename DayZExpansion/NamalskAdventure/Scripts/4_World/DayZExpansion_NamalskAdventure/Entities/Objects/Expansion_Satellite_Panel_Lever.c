/**
 * Expansion_Satellite_Panel_Lever.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Satellite_Panel_Lever: House
{
	const string COLOR_LED_OFF 		= "#(argb,8,8,3)color(0,0,0,1.0,co)";
	const string COLOR_LED_GREEN	= "#(argb,8,8,3)color(0,1,0,1.0,co)";
	const string COLOR_LED_RED 		= "#(argb,8,8,3)color(1,0,0,1.0,co)";
	
	const string SELECTION_NAME_LED_RED		= "LED_Red";
	const string SELECTION_NAME_LED_GREEN	= "LED_Green";
	
	bool m_PanelWasUsed;
	bool m_PanelWasUsedPrev;
	bool m_BlinkingFlipFlop;
	ref Timer m_FlipFlopTimer;
	
	EffectSound m_ActivationSound;
	ELEDState m_LedStateRed;
	ELEDState m_LedStateGreen;
	
	void Expansion_Satellite_Panel_Lever()
	{
		RegisterNetSyncVariableBool("m_PanelWasUsed");
		SetLEDState(ELEDColors.RED, ELEDState.ON);
		SetLEDState(ELEDColors.GREEN, ELEDState.OFF);
	}
	
	bool CanInteract()
	{
		bool hasPower = ExpansionNamalskModule.GetModuleInstance().HasSatelliteFacilityPower();
		if (!hasPower)
			return false;
		
		return true;
	}
	
	void Interact()
	{
		m_PanelWasUsed = !m_PanelWasUsed;
		SetSynchDirty();
	}
	
	bool GetPanelState()
	{
		return m_PanelWasUsed;
	}
	
	void SetLEDState(ELEDColors color, ELEDState state)
	{
		#ifndef SERVER
		if (color == ELEDColors.RED)
		{
			if (m_LedStateRed != state)
			{
				m_LedStateRed = state;
				OnLEDStateChanged();
			}
		}
		else
		{
			if (m_LedStateGreen != state)
			{
				m_LedStateGreen = state;
				OnLEDStateChanged();
			}
		}
		#endif
	}
	
	void SetBlinkingTimer(bool enable)
	{
		if (enable)
		{
			if (!m_FlipFlopTimer)
			{
				m_FlipFlopTimer = new Timer();
				m_FlipFlopTimer.Run(0.1, this, "ToggleFlipFlop", NULL, true);
			}
		}
		else
		{
			if (m_FlipFlopTimer)
			{
				m_FlipFlopTimer = null;
			}
		}
	}
	
	void OnLEDStateChanged()
	{
		if (m_LedStateRed == ELEDState.BLINKING || m_LedStateGreen == ELEDState.BLINKING)
		{
			SetBlinkingTimer(true);
		}
		else
		{
			SetBlinkingTimer(false);
		}
		
		//! Red
		if (m_LedStateRed == ELEDState.ON)
		{
			UpdateLED(SELECTION_NAME_LED_RED, COLOR_LED_RED);
		}
		else if (m_LedStateRed == ELEDState.OFF)
		{
			UpdateLED(SELECTION_NAME_LED_RED, COLOR_LED_OFF);
		}
		else if (m_LedStateRed == ELEDState.BLINKING)
		{
			if (m_BlinkingFlipFlop)
			{
				UpdateLED(SELECTION_NAME_LED_RED, COLOR_LED_RED);
			}
			else
			{
				UpdateLED(SELECTION_NAME_LED_RED, COLOR_LED_OFF);
			}
			
		//! Green
		}
		if (m_LedStateGreen == ELEDState.ON)
		{
			UpdateLED(SELECTION_NAME_LED_GREEN, COLOR_LED_GREEN);
		}
		else if (m_LedStateGreen == ELEDState.OFF)
		{
			UpdateLED(SELECTION_NAME_LED_GREEN, COLOR_LED_OFF);
		}
		else if (m_LedStateGreen == ELEDState.BLINKING)
		{
			if(m_BlinkingFlipFlop)
			{
				UpdateLED(SELECTION_NAME_LED_GREEN, COLOR_LED_GREEN);
			}
			else
			{
				UpdateLED(SELECTION_NAME_LED_GREEN, COLOR_LED_OFF);
			}
			
		}
	}
	
	void ResetPanelUsed()
	{
		m_PanelWasUsed = false;
		SetSynchDirty();
	}
	
	/*void OnDoorStateChangedClient(EUndegroundEntranceState newState, EUndegroundEntranceState prevState)
	{
		switch (GetLinkedDoorType())
		{
			case EUndegroundDoorType.MAIN:
			{
				if (newState >= EUndegroundEntranceState.CLOSED && newState < EUndegroundEntranceState.OPENING_G)
				{
					SetLEDState(ELEDColors.RED, ELEDState.ON);
				}
				else if (newState == EUndegroundEntranceState.OPENING_G)
				{
					SetLEDState(ELEDColors.RED, ELEDState.OFF);
				}
				else if (newState >= EUndegroundEntranceState.CLOSING_A)
				{
					SetLEDState(ELEDColors.RED, ELEDState.BLINKING);
				}
				
				if (newState == EUndegroundEntranceState.CLOSED)
				{
					SetLEDState(ELEDColors.GREEN, ELEDState.OFF);
				}
				else if (newState >= EUndegroundEntranceState.OPENING_A && newState < EUndegroundEntranceState.OPENING_G)
				{
					SetLEDState(ELEDColors.GREEN, ELEDState.BLINKING);
				}
				else if (newState >= EUndegroundEntranceState.OPENING_G)
				{
					SetLEDState(ELEDColors.GREEN, ELEDState.ON);
				}
			}
			break;
			case EUndegroundDoorType.SMALL:
			{
				if (newState == EUndegroundEntranceState.OPENING_A)
				{
					SetLEDState(ELEDColors.RED, ELEDState.ON);
					SetLEDState(ELEDColors.GREEN, ELEDState.BLINKING);
				}
				else if (newState == EUndegroundEntranceState.OPENING_B)
				{
					SetLEDState(ELEDColors.RED, ELEDState.OFF);
					SetLEDState(ELEDColors.GREEN, ELEDState.ON);
				}
				else if (newState == EUndegroundEntranceState.CLOSING_A)
				{
					SetLEDState(ELEDColors.RED, ELEDState.BLINKING);
					SetLEDState(ELEDColors.GREEN, ELEDState.ON);
				}
				if (newState == EUndegroundEntranceState.CLOSING_B || newState == EUndegroundEntranceState.CLOSED)
				{
					SetLEDState(ELEDColors.RED, ELEDState.ON);
					SetLEDState(ELEDColors.GREEN, ELEDState.OFF);
				}
			}
			break;
		}
	}*/
	
	protected void UpdateLED(string selection, string color)
	{
		int selectionIdx = GetHiddenSelectionIndex(selection);
		SetObjectTexture(selectionIdx, color);
	}
	
	void OnPanelDeactivated()
	{
		PlaySoundSet(m_ActivationSound, "UndergroundDoor_PanelActivation_SoundSet", 0, 0);
		SetAnimationPhase("PanelLever", 0);
		SetLEDState(ELEDColors.GREEN, ELEDState.OFF);
		SetLEDState(ELEDColors.RED, ELEDState.ON);
	}

	void OnPanelActivated()
	{
		PlaySoundSet(m_ActivationSound, "UndergroundDoor_PanelActivation_SoundSet", 0, 0);
		SetAnimationPhase("PanelLever", 1);
		SetLEDState(ELEDColors.GREEN, ELEDState.ON);
		SetLEDState(ELEDColors.RED, ELEDState.OFF);
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ExpansionActionUseSatellitePanel);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_PanelWasUsed != m_PanelWasUsedPrev)
		{
			if (m_PanelWasUsed)
			{
				OnPanelActivated();
			}
			else
			{
				OnPanelDeactivated();
			}

			m_PanelWasUsedPrev = m_PanelWasUsed;
		}
	}
}