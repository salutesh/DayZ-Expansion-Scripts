/*
	Manages all keybinds for COM
*/
class COMKeyBinds extends COM_Module 
{
	bool m_IsHudVisible = true;
	protected int m_nAutoWalkMode = 0;

	void COMKeyBinds()
	{
	}

	void ~COMKeyBinds()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
		UpdateAutoWalk();

		UpdateGodMode();
	}
	
	override void RegisterCOM_KeyMouseBindings() 
	{
		COM_KeyMouseBinding toggleCursor	= new COM_KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "Toggles the cursor."   , true	 );
		COM_KeyMouseBinding toggleCOMEditor = new COM_KeyMouseBinding( GetModuleType() , "ShowCOMEditor" ,  "Opens the COM Editor."		   );
		COM_KeyMouseBinding reload		  = new COM_KeyMouseBinding( GetModuleType() , "Reload"		,  "Instantly reloads mag."		  );
		COM_KeyMouseBinding spawnZ		  = new COM_KeyMouseBinding( GetModuleType() , "SpawnZ"		,  "Spawns infected."				);
		COM_KeyMouseBinding hideHud		 = new COM_KeyMouseBinding( GetModuleType() , "HideHud"	   ,  "Hides ui completely."			);
		COM_KeyMouseBinding printPlayer	 = new COM_KeyMouseBinding( GetModuleType() , "PrintPlayer"   ,  "Print current player position."  );
		COM_KeyMouseBinding autoRun		 = new COM_KeyMouseBinding( GetModuleType() , "AutoRun"	   ,  "Toggle autorun."				 );
		//COM_KeyMouseBinding keyFrame		= new COM_KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );
		COM_KeyMouseBinding closeMenu	   = new COM_KeyMouseBinding( GetModuleType() , "CloseOpenMenu" ,  "Close the menu on esc.", true	);
		COM_KeyMouseBinding decayGetPos	   = new COM_KeyMouseBinding( GetModuleType() , "DecayGetPos" ,  "Close the menu on esc.", true	);

		//toggleCursor   .AddBinding( "kU" );
		toggleCOMEditor.AddBinding( "kU" );
		reload		 .AddBinding( "kR" );
		spawnZ		 .AddBinding( "kO" );
		hideHud		.AddBinding( "kHome" );
		printPlayer	.AddBinding( "kP" );
		autoRun		.AddBinding( "kComma" );
		//keyFrame	   .AddBinding( "kPrior" );
		closeMenu	  .AddBinding( "kEscape" );
		decayGetPos	  .AddBinding( "kL" );

		RegisterCOM_KeyMouseBinding( toggleCursor );
		RegisterCOM_KeyMouseBinding( toggleCOMEditor );
		RegisterCOM_KeyMouseBinding( reload );
		//RegisterCOM_KeyMouseBinding( spawnZ );
		RegisterCOM_KeyMouseBinding( hideHud );
		RegisterCOM_KeyMouseBinding( printPlayer );
		RegisterCOM_KeyMouseBinding( autoRun );
		//RegisterCOM_KeyMouseBinding( keyFrame );
		RegisterCOM_KeyMouseBinding( closeMenu );
		RegisterCOM_KeyMouseBinding( decayGetPos );
	}

	void OpenKeyframe() 
	{
		GetGame().GetUIManager().ShowScriptedMenu( GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL );
	}

	void DecayGetPos()
	{
	}

	void ToggleCursor()
	{
		if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
		{
			GetGame().GetInput().ChangeGameFocus( 1 );
			GetGame().GetUIManager().ShowUICursor( true );
			FreeDebugCamera.GetInstance().SetFreezed(true);
		}
		else
		{
			GetGame().GetUIManager().ShowUICursor( false );
			GetGame().GetInput().ResetGameFocus();
			FreeDebugCamera.GetInstance().SetFreezed(false);
		}
		 // Message( GetDayZGame().GetMissionFolderPath());
	}

	void CloseOpenMenu()
	{
		if( GetGame().GetUIManager().GetMenu() && ( GetGame().GetUIManager().GetMenu().GetID() == 133742 ) )
		{
			GetGame().GetUIManager().Back();
		}
	}

	void ShowCOMEditor()
	{
		GetGame().GetUIManager().ShowScriptedMenu( new EditorMenu() , NULL );
	}

	void TeleportCursor()
	{
		Print( "COMKeyBinds::TeleportCursor()" );
		
		vector hitPos = GetCursorPos();

		float distance = vector.Distance( GetPlayer().GetPosition(), hitPos );

		if ( distance < 5000 )
		{
			EntityAI oVehicle = GetPlayer().GetDrivingVehicle();

			if( oVehicle )
			{
				GetPlayer().MessageStatus( "Get out of the vehicle first!" );
			}
			else
			{
				GetPlayer().SetPosition( hitPos );
				GetPlayer().MessageStatus( "Teleported!" );
			}
		}
		else
		{
			GetPlayer().MessageStatus( "Distance for teleportation is too far!" );
		}
	}

	void Reload()
	{
		EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();

		if( oWeapon )
		{
			Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );

			if( oMag && oMag.IsMagazine() )
			{
				oMag.LocalSetAmmoMax();
			}

			Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );

			if( oSupressor )
			{
				oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
			}
		}
	}

	void SpawnZ() 
	{
		if( CTRL() )
		{
			GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos(), false, true );
		}
		else if( SHIFT() )
		{
			GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), GetCursorPos(), false, true );
		}
		else
		{
			GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), GetCursorPos(), false, true );
		}
	}

	void HideHud() 
	{
		Widget hudWidget = IngameHud.Cast(GetClientMission().GetHud()).GetHudPanelWidget();
		hudWidget.Show(!hudWidget.IsVisible());
	}

	void PrintPlayer()
	{
		Print( "Position:" + GetPlayer().GetPosition().ToString() );
		Print( "Orientation" + GetPlayer().GetOrientation().ToString() );
		// // Message(  "POS X:" + GetPlayer().GetPosition()[0] + " Y:" + GetPlayer().GetPosition()[2] + " Z:" + GetPlayer().GetPosition()[1] );
		// // Message(  "Player position and orientation vector were written to the game logs too." );
	}

	void AutoRun()
	{
		if( m_nAutoWalkMode && !SHIFT() )
		{
			m_nAutoWalkMode = 0;
			GetPlayer().GetInputController().OverrideMovementSpeed( false, 0 );
			GetPlayer().GetInputController().OverrideMovementAngle( false, 0 );
		}
		else
		{
			if( SHIFT() )
			{
				m_nAutoWalkMode = 2;
			}
			else
			{
				m_nAutoWalkMode = 1;
			}
		}
	}

	void UpdateAutoWalk()
	{
		if( m_nAutoWalkMode )
		{
			if( ( GetPlayer().GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 1 ) )
			{
				GetPlayer().GetInputController().OverrideMovementSpeed( true, 2 );
			}
			else
			{
				GetPlayer().GetInputController().OverrideMovementSpeed( true, 3 );
			}

			GetPlayer().GetInputController().OverrideMovementAngle( true, 1 );
		}
	}

	void UpdateGodMode()
	{
		// just putting this here for now
		if ( m_comGodMode ) // located in staticfunctions
		{
			GetPlayer().SetAllowDamage( false );

			GetPlayer().SetHealth( GetPlayer().GetMaxHealth( "", "" ) );
			GetPlayer().SetHealth( "","Blood", GetPlayer().GetMaxHealth( "", "Blood" ) );
			GetPlayer().SetHealth( "","Shock", GetPlayer().GetMaxHealth( "", "Shock" ) );

			// GetPlayer().GetStaminaHandler().SyncStamina(1000, 1000);
			GetPlayer().GetStatStamina().Set(GetPlayer().GetStaminaHandler().GetStaminaCap());
			GetPlayer().GetStatEnergy().Set(1000);
			GetPlayer().GetStatWater().Set(1000);
//			GetPlayer().GetStatStomachVolume().Set(300);
//			GetPlayer().GetStatStomachWater().Set(300);
//			GetPlayer().GetStatStomachEnergy().Set(300);
			GetPlayer().GetStatHeatComfort().Set(0);

			EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();

			if( oWeapon )
			{
				Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );

				if( oMag && oMag.IsMagazine() )
				{
					oMag.LocalSetAmmoMax();
				}

				Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );

				if( oSupressor )
				{
					oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
				}
			}
		}
	}
}