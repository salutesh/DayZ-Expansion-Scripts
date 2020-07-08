class EditorMenu extends UIScriptedMenu 
{
	protected ButtonWidget m_MatButton;
	protected ButtonWidget m_ObjectButton;
	protected ButtonWidget m_PositionButton;
	protected ButtonWidget m_WeatherButton;
	protected ButtonWidget m_GameButton;
	protected ButtonWidget m_ObjectEditorButton;

	protected Widget m_aiMenu;
	protected Widget m_objectMenu;
	protected Widget m_weatherMenu;
	protected Widget m_positionMenu;
	protected Widget m_gameMenu;
	protected Widget m_matMenu;
	protected Widget m_objectInfoMenu;
	protected bool m_toogleDOJCode;

	void EditorMenu()
	{
		SetID( 133742 );
		
		m_toogleDOJCode = false;
	}
	
	void ~EditorMenu()
	{
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\EditorMenu.layout" );

		m_ObjectButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("objects_button") );
		m_PositionButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("position_button") );
		m_WeatherButton  = ButtonWidget.Cast( layoutRoot.FindAnyWidget("weather_button") );
		m_GameButton	 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("game_button") );
		m_MatButton	 = ButtonWidget.Cast( layoutRoot.FindAnyWidget("mat_button") );
		m_ObjectEditorButton = layoutRoot.FindAnyWidget( "objectEditor_button" );

		m_aiMenu   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\AIMenu.layout", layoutRoot );
		m_objectMenu   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectMenu.layout", layoutRoot );
		m_weatherMenu  = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\WeatherMenu.layout", layoutRoot );
		m_positionMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\PositionMenu.layout", layoutRoot );
		m_gameMenu 	   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\GameMenu.layout", layoutRoot );
		m_matMenu 	   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\CameraMenu.layout", layoutRoot );
		m_objectInfoMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\ExpansionCOM.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectEditorInfo.layout", layoutRoot );

		return layoutRoot;
	}

	override bool UseMouse() 
	{
		return false;
	}

	override bool UseKeyboard() 
	{
		return false;
	}

	override void OnShow()
	{
		super.OnShow();

		GetGame().GetInput().ChangeGameFocus( 1 );
		GetGame().GetUIManager().ShowUICursor( true );
	}

	override void OnHide()
	{
		super.OnHide();

		ObjectEditor.Cast( GetCOMModuleManager().GetModule( ObjectEditor )).EditorState( false );

		GetGame().GetInput().ResetGameFocus();
	}

	override bool OnDoubleClick( Widget w, int x, int y, int button ) 
	{
		if ( w == layoutRoot ) 
		{
			COM_ObjectMenu objectMenu;
			m_objectMenu.GetScript( objectMenu );
			string strSelection = objectMenu.GetCurrentSelection();

			if ( strSelection != "" ) 
			{
				bool ai = false;

				if ( GetGame().IsKindOf( strSelection, "DZ_LightAI" ) ) 
				{
					ai = false;
				}

				Object obj = GetGame().CreateObject( strSelection, GetPointerPos(), true, ai );
				obj.PlaceOnSurface(); 
				ForceTargetCollisionUpdate( obj );
				ObjectEditor.Cast(GetCOMModuleManager().GetModule( ObjectEditor )).SelectObject( obj );
				ObjectEditor.Cast(GetCOMModuleManager().GetModule( ObjectEditor )).addObject( obj );
				
				ProcessObject( obj );
				
				obj.PlaceOnSurface();

				if ( m_toogleDOJCode )
				{
					float random_yaw = Math.RandomFloat(1, 359);
					float random_pitch = Math.RandomFloat(0, 0.7);
					float random_roll = Math.RandomFloat(0, 0.7);
					obj.SetOrientation( Vector(random_yaw, random_pitch, random_roll) );
					
					// GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RandomOri, 100, false, obj);
				}

				ObjectInfoMenu.listBox.AddItem(obj.GetType(), obj, 0);
			}
		}

		return false;
	}

	void ProcessObject(Object obj)
	{
		CarScript car;
		HelicopterScript heli;

		if ( obj.IsKindOf("ExpansionMerlin") )
		{
			heli = HelicopterScript.Cast( obj );
			if ( heli )
			{
				heli.GetInventory().CreateInInventory("ExpansionMerlinFrontWheel");
				heli.GetInventory().CreateInInventory("ExpansionMerlinFrontWheel");
				heli.GetInventory().CreateInInventory("ExpansionMerlinBackWheel");
				heli.GetInventory().CreateInInventory("ExpansionMerlinBackWheel");
			}
		}
		else if ( obj.IsKindOf("ExpansionVodnik") )
		{
			car = CarScript.Cast( obj );
			if ( car )
			{
				car.GetInventory().CreateInInventory("ExpansionVodnikWheel");
				car.GetInventory().CreateInInventory("ExpansionVodnikWheel");
				car.GetInventory().CreateInInventory("ExpansionVodnikWheel");
				car.GetInventory().CreateInInventory("ExpansionVodnikWheel");
			}
		}
		else if ( obj.IsKindOf("ExpansionQuadAtv") )
		{
			car = CarScript.Cast( obj );
			if ( car )
			{
				car.GetInventory().CreateInInventory("ExpansionQuadATVWheelBack");
				car.GetInventory().CreateInInventory("ExpansionQuadATVWheelBack");
				car.GetInventory().CreateInInventory("ExpansionQuadATVWheelFront");
				car.GetInventory().CreateInInventory("ExpansionQuadATVWheelFront");
			}
		}
		else if ( obj.IsKindOf("ExpansionBus") )
		{
			car = CarScript.Cast( obj );
			if ( car )
			{
				car.GetInventory().CreateInInventory("ExpansionBusWheel");
				car.GetInventory().CreateInInventory("ExpansionBusWheel");
				car.GetInventory().CreateInInventory("ExpansionBusWheelDouble");
				car.GetInventory().CreateInInventory("ExpansionBusWheelDouble");
			}
		}
		else if ( obj.IsKindOf("ExpansionTractor") )
		{
			car = CarScript.Cast( obj );
			if ( car )
			{
				car.GetInventory().CreateInInventory("ExpansionTractorFrontWheel");
				car.GetInventory().CreateInInventory("ExpansionTractorFrontWheel");
				car.GetInventory().CreateInInventory("ExpansionTractorBackWheel");
				car.GetInventory().CreateInInventory("ExpansionTractorBackWheel");
			}
		}
		else if ( obj.IsKindOf("ExpansionUAZ") || obj.IsKindOf("ExpansionUAZRoofless") || obj.IsKindOf("ExpansionUAZCargoRoofless") || obj.IsKindOf("ExpansionUAZCargo") )
		{
			car = CarScript.Cast( obj );
			if ( car )
			{
				car.GetInventory().CreateInInventory("ExpansionUAZWheel");
				car.GetInventory().CreateInInventory("ExpansionUAZWheel");
				car.GetInventory().CreateInInventory("ExpansionUAZWheel");
				car.GetInventory().CreateInInventory("ExpansionUAZWheel");
			}
		}
	}

	vector GetSize( Object selected_obj )
	{
		vector size = Vector(1,1,1);
		vector min_max[2];
		
		if ( selected_obj )
		{	
			selected_obj.GetCollisionBox(min_max);
				
			size[0] = min_max[1][0] - min_max[0][0];
			size[2] = min_max[1][2] - min_max[0][2];
			size[1] = min_max[1][1] - min_max[0][1];
			
			return size;
		}

		return size;
	}

	void RandomOri( Object selected_obj )
	{
		vector obj_pos = selected_obj.GetPosition();
		vector obj_dir = selected_obj.GetDirection();
		vector obj_size = GetSize( selected_obj );

		vector view_target = obj_pos;
			
		Message( GetPlayer(), "Size " + obj_size );
		Message( GetPlayer(), "Direction " + obj_dir );

		float random_yaw = Math.RandomFloat(-1, 1);

		selected_obj.SetDirection(Vector(random_yaw, obj_dir[1], obj_dir[2]));

		// vector pos = objtoedit.GetPosition();
		// objtoedit.SetOrientation( GetGame().GetSurfaceOrientation(pos[0], pos[2]) );

		// float random_yaw = Math.RandomFloat(-180, 180);
		// objtoedit.SetOrientation( Vector(objtoedit.GetOrientation()[0], objtoedit.GetOrientation()[1], objtoedit.GetOrientation()[2]) );
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		PopupMenu popMenu;

		if ( w == m_ObjectButton ) 
		{
			m_objectMenu.GetScript( popMenu );
		}
		if ( w == m_PositionButton ) 
		{
			m_positionMenu.GetScript( popMenu );
		}

		if ( w == m_WeatherButton ) 
		{
			m_weatherMenu.GetScript( popMenu );
		}

		if ( w == m_GameButton ) 
		{
			m_gameMenu.GetScript( popMenu );
		}
		if ( w == m_MatButton ) 
		{
			m_matMenu.GetScript( popMenu );
		}
		if ( w == m_ObjectEditorButton ) 
		{
			m_objectInfoMenu.GetScript( popMenu );

			ObjectEditor.Cast( GetCOMModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}

		if ( popMenu ) 
		{

			if ( popMenu.GetLayoutRoot().IsVisible() ) 
			{
				popMenu.GetLayoutRoot().Show( false );
				popMenu.OnHide();
			}
			else 
			{
				popMenu.GetLayoutRoot().Show( true );
				popMenu.OnShow();
			}

			SetButtonFocus( w );
			HideMenus( popMenu.GetLayoutRoot() );
		}

		return false;
	}

	void SetButtonFocus( Widget focus ) 
	{
		// todo change this so that its based on visible layout and not click event

		Widget m_ObjectButtonBkg   = layoutRoot.FindAnyWidget("objects_button_bkg");
		Widget m_PositionButtonBkg = layoutRoot.FindAnyWidget("position_button_bkg");
		Widget m_WeatherButtonBkg  = layoutRoot.FindAnyWidget("weather_button_bkg");
		Widget m_GameButtonBkg	 = layoutRoot.FindAnyWidget("game_button_bkg");
		Widget m_MatButtonBkg	 = layoutRoot.FindAnyWidget("mat_button_bkg");
		Widget m_ObjectEditorBkg   = layoutRoot.FindAnyWidget("objectEditor_button_bkg");

		m_ObjectButtonBkg.SetColor(ARGB(0, 255, 255, 255)); // reset colors
		m_PositionButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_WeatherButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_GameButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_MatButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_ObjectEditorBkg.SetColor(ARGB(0, 255, 255, 255));
		
		if ( m_ObjectButton == focus && m_objectMenu.IsVisible() ) 
		{
			m_ObjectButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_WeatherButton == focus && m_weatherMenu.IsVisible() )
		{
			m_WeatherButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_PositionButton == focus && m_positionMenu.IsVisible() )
		{
			m_PositionButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_GameButton == focus && m_gameMenu.IsVisible() )
		{
			m_GameButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_MatButton == focus && m_matMenu.IsVisible() )
		{
			m_MatButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_ObjectEditorButton == focus && m_objectInfoMenu.IsVisible() )
		{
			m_ObjectEditorBkg.SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void HideMenus( Widget focus ) 
	{
		if ( m_objectInfoMenu != focus && m_objectMenu != focus && m_objectMenu.IsVisible() ) 
		{
			m_objectMenu.Show(false);
		}
		if ( m_objectInfoMenu != focus && m_aiMenu != focus && m_aiMenu.IsVisible() ) 
		{
			m_aiMenu.Show(false);
		}
		if ( m_weatherMenu != focus && m_weatherMenu.IsVisible() ) 
		{
			m_weatherMenu.Show(false);
		}
		if ( m_positionMenu != focus && m_positionMenu.IsVisible() ) 
		{
			m_positionMenu.Show(false);
		}
		if ( m_gameMenu != focus && m_gameMenu.IsVisible() ) 
		{
			m_gameMenu.Show(false);
		}
		if ( m_matMenu != focus && m_matMenu.IsVisible() ) 
		{
			m_matMenu.Show(false);
		}
		if ( m_objectMenu != focus && m_objectInfoMenu != focus && m_objectInfoMenu.IsVisible() ) 
		{
			m_objectInfoMenu.Show(false);
			ObjectEditor.Cast( GetCOMModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}
	}

	override void Update( float timeslice ) 
	{
		//GetPlayer().MessageStatus( (GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK).ToString() );

		if ( GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( false );
				GetGame().GetInput().ResetGameFocus( );
			}
		}
		else
		{
			if ( !GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( true );
				GetGame().GetInput().ChangeGameFocus( 1 );
			}
		}
	}
	
	void SetDOJBool(bool state)
	{
		m_toogleDOJCode = state;
	}
	
	bool GetDOJBoolState()
	{
		return m_toogleDOJCode;
	}
	
	void ToogleDOJCode()
	{
		if (m_toogleDOJCode)
		{
			m_toogleDOJCode = false;
		}
		else
		{
			m_toogleDOJCode = true;
		}
	}
}

