#ifndef EXPANSION_MAINMENU_NEW_DISABLE
class DayZIntroSceneExpansion
{
	protected Camera 					m_Camera;
	protected vector 					m_CameraTrans[4];
	protected vector 					m_CharacterPos;
	protected vector 					m_CharacterRot;
	protected Weather					m_Weather;	
	protected vector 					m_Target;
	protected ref IntroSceneCharacter	m_Character;
	protected bool		m_EnableClick;
	protected bool 		m_RotatingCharacter;
	protected int 		m_RotatingCharacterMouseX;
	protected int 		m_RotatingCharacterMouseY;
	protected float 	m_RotatingCharacterRot;
	protected float 	m_Radius;
	protected float 	m_Radius_original;
	protected float 	m_DiffX;
	protected float 	m_DeltaX;
	protected float 	m_DeltaZ;
	protected float 	m_Angle;
	protected float 	m_Angle_offset = 0;
	protected float 	m_NewX = 0;
	protected float 	m_NewZ = 0;
	protected float 	m_BlurValue;
	protected bool 		m_CanSpin = false;
	string currScenePath;
	
	
	void DayZIntroSceneExpansion()
	{


		World world = g_Game.GetWorld();
		string root_path = "cfgExpansionCharacterScenes " + g_Game.GetWorldName();

		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count);
		//index = 4;
		
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
		string scene_path = root_path + " " + childName;	
		SetScenePath(scene_path);
		
		if (world)
		{
			TIntArray date = new TIntArray;
			g_Game.ConfigGetIntArray(scene_path + " date", date);
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
		}
		GetGame().ObjectDelete( m_Camera );
		Class.CastTo(m_Camera, g_Game.CreateObject("staticcamera", g_Game.ConfigGetVector(scene_path + " CameraPosition"), true));
		if (m_Camera)
		{
			m_Camera.SetFOV(g_Game.ConfigGetFloat(scene_path + " fov"));
			m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
			
			m_Camera.SetActive(true);		
			m_Camera.SetOrientation(g_Game.ConfigGetVector(scene_path + " CameraOrientation"));
			m_Character = new IntroSceneCharacter();
			m_Character.LoadCharacterData(g_Game.ConfigGetVector(scene_path + " PlayerPosition"), g_Game.ConfigGetVector(scene_path + " PlayerOrientation"));
			//Equip Player with scene specific shit here
			CustomHandItem();
			SetAnim();
			TStringArray mapping = new TStringArray;
			g_Game.ConfigGetTextArray(scene_path + " MappingFiles", mapping);
			CreateSzeneObjects(mapping);

		}
		float overcast, rain, windspeed, fog;
		overcast = g_Game.ConfigGetFloat(scene_path + " overcast");
		rain = g_Game.ConfigGetFloat(scene_path + " rain");
		fog = g_Game.ConfigGetFloat(scene_path + " fog");
		windspeed = g_Game.ConfigGetFloat(scene_path + " windspeed");
		m_Weather = g_Game.GetWeather();
		m_Weather.GetOvercast().SetLimits( overcast, overcast );
		m_Weather.GetRain().SetLimits( rain, rain );
		m_Weather.GetFog().SetLimits( fog, fog );
		
		m_Weather.GetOvercast().Set( overcast, overcast, 0);
		m_Weather.GetRain().Set( rain, rain, 0);
		m_Weather.GetFog().Set( fog, fog, 0);
		
		
		if ( windspeed != -1 )
		{
			m_Weather.SetWindSpeed(windspeed);
			m_Weather.SetWindMaximumSpeed(windspeed);
			m_Weather.SetWindFunctionParams(windspeed,windspeed,0);
		}	
		
		SetClickEnable( true );		
		m_DeltaX = Math.AbsFloat(m_CharacterPos[0] - m_Camera.GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_CharacterPos[2] - m_Camera.GetPosition()[2]);
		
		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}
		if (  g_Game.ConfigGetInt(scene_path + " CanRotate") == 1)
		{
			m_CanSpin = true;
		}
	}
	void SetScenePath(string path)
	{
		currScenePath = path;
	}
	string GetScenePath()
	{
		return currScenePath;
	}
	void SetClickEnable( bool enable )
	{
		m_EnableClick = enable;
	}
	
	void SetAnim()
	{		
		if ( g_Game.ConfigGetInt(GetScenePath() + " CustomPose") != -1)
		{
			m_Character.GetCharacterObj().StartCommand_Action(g_Game.ConfigGetInt(GetScenePath() + " CustomPose"), ActionBaseCB, DayZPlayerConstants.STANCEMASK_ALL);
		};
	}
	void CustomHandItem()
	{		
		string item;
		GetGame().ConfigGetText(GetScenePath() + " HandItem", item);
		if ( item != "")
		{
			if (m_Character.GetCharacterObj().GetItemInHands())
				GetGame().ObjectDelete(m_Character.GetCharacterObj().GetItemInHands());
			m_Character.GetCharacterObj().GetHumanInventory().CreateInHands(item);
		};
	}
	// ------------------------------------------------------------
	bool IsClickEnabled()
	{
		return m_EnableClick;
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStart()
	{
		m_RotatingCharacter = true;
		g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);
		
		if (m_Character && m_CanSpin ) 
		{
			m_RotatingCharacterRot = m_CharacterRot[0];
		}
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStop()
	{
		if (m_RotatingCharacter)
		{
			m_RotatingCharacter = false;
		}
	}
	
	// ------------------------------------------------------------
	bool IsRotatingCharacter()
	{
		return m_RotatingCharacter;
	}
	
	// ------------------------------------------------------------
	void CharacterRotate()
	{
		if (m_Character && m_Character.GetCharacterObj() && m_CanSpin)
		{
			int actual_mouse_x;
			int actual_mouse_y;
			float coef;
			g_Game.GetMousePos(actual_mouse_x, actual_mouse_y);
		
			m_DiffX = m_RotatingCharacterMouseX - actual_mouse_x;
			
			coef = ( m_RotatingCharacterRot + (m_DiffX * 0.5) ) / 360;
			coef = coef - Math.Floor(coef);
			m_CharacterRot[0] = coef * 360;
			
			m_Character.GetCharacterObj().SetOrientation(m_CharacterRot);
		}
	}
	
	// ------------------------------------------------------------
	void Update()
	{
		if (m_Character && m_RotatingCharacter)
		{
			CharacterRotate();
		}
	}		
	IntroSceneCharacter GetIntroCharacter()
	{
		return m_Character;
	}
	protected void GetSelectedUserName()
	{
		string name;
		BiosUserManager user_manager = GetGame().GetUserManager();
		if( user_manager )
		{
			BiosUser user = user_manager.GetSelectedUser();
			if( user )
			{
				g_Game.SetPlayerGameName( user.GetName() );
				return;
			}
		}
		g_Game.SetPlayerGameName(GameConstants.DEFAULT_CHARACTER_NAME);
	}	


	void CreateSzeneObjects(TStringArray filestoload)
	{
		string className;
		vector position;
		vector rotation;
		string special;
		TStringArray attachments = new TStringArray;
		for (int i = 0; i < filestoload.Count(); i++)
		{
			string name = filestoload[i];

			string filePath = name + EXPANSION_MAPPING_EXT;
			FileHandle file = OpenFile(filePath, FileMode.READ);
			if (!file)
				continue;
			
			array<Object> objects = new array<Object>;
			while (GetObjectFromFile(file, className, position, rotation, special, attachments))
			{	
				Object obj;

				#ifdef EXPANSIONEXPRINT
				EXPrint("Spawning object with chached collition: " + className + " on pos: " + position.ToString());
				#endif

				obj = GetGame().CreateObject(className, position);
				if (!obj)
					continue;
								
				if( position )
					obj.SetPosition( position );
					
				if( rotation )
					obj.SetOrientation( rotation );

				if ( attachments )
				{
					EntityAI itemWithAttachment = EntityAI.Cast(obj);
					if ( itemWithAttachment )
					{
						for (int j = 0; j < attachments.Count(); j++)
						{
							Print( itemWithAttachment.GetType() + attachments[j]);
							itemWithAttachment.GetInventory().CreateAttachment(attachments[j]);
						}					
					}
				}
				#ifdef EXPANSIONEXPRINT
				EXPrint("Succesfully spawned object with chached collition: " + className + " on pos: " + position.ToString());
				#endif

				if ( !obj )
					continue;					
								
				obj.SetPosition( position );
				obj.SetOrientation( rotation );			
				#ifdef NAMALSK_SURVIVAL
				vfx_the_thing theThing = vfx_the_thing.Cast(obj);
				if ( theThing )
				{	
					if ( Math.RandomInt(0, 2))
						theThing.SetStable(true);
				}
				#endif				
			}
			
		}
		
	};
	
	private bool GetObjectFromFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false", out TStringArray itemAttachments = NULL )
	{
		
		string line;
		int lineSize = FGets( file, line );

		
		if ( lineSize < 1 )
			return false;
		
		ref TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get( 0 );		
		position = tokens.Get( 1 ).ToVector();
		rotation = tokens.Get( 2 ).ToVector();	
		special = tokens.Get( 3 );
		string gear_array = tokens.Get( 4 );
		TStringArray geartokens = new TStringArray;
		gear_array.Split(",", geartokens);
		itemAttachments = geartokens;
	

		return true;
	}		
};
modded class CharacterCreationMenu
{
	DayZIntroSceneExpansion										m_CustomScene;
	void CharacterCreationMenu()
	{
		MissionMainMenu mission = MissionMainMenu.Cast( GetGame().GetMission() );
		

		m_CustomScene = mission.GetIntroSceneExpansion();
		
		//m_CustomScene.ResetIntroCamera();
	}
	override PlayerBase GetPlayerObj()
	{
		if (m_Scene)
			return m_Scene.GetIntroCharacter().GetCharacterObj();
		if (m_CustomScene)
			return m_CustomScene.GetIntroCharacter().GetCharacterObj();
		return super.GetPlayerObj();
	}
	override Widget Init()
	{
		#ifdef PLATFORM_CONSOLE
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/character_creation/xbox/character_creation.layout" );
			m_CharacterSaved 				= false;
			m_ConsoleSave 					= layoutRoot.FindAnyWidget( "save_console" );
		#else
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/character_creation/pc/character_creation.layout" );
		#endif
		
		m_CharacterRotationFrame			= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		m_Apply								= layoutRoot.FindAnyWidget( "apply" );
		m_Save 								= layoutRoot.FindAnyWidget( "save" );
		m_RandomizeCharacter				= layoutRoot.FindAnyWidget( "randomize_character" );
		m_BackButton						= layoutRoot.FindAnyWidget( "back" );
		m_Version							= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_DetailsRoot 						= layoutRoot.FindAnyWidget( "menu_details_tooltip" );
		m_DetailsLabel						= TextWidget.Cast( m_DetailsRoot.FindAnyWidget( "menu_details_label" ) );
		m_DetailsText						= RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( "menu_details_tooltip_content" ) );
		m_CharacterHeaderText 				= TextWidget.Cast(layoutRoot.FindAnyWidget( "char_header_text" ));
		m_PlayedCharacterInfo 				= layoutRoot.FindAnyWidget( "played_char_info" );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		#else
			version = "#main_menu_version" + " " + version;
		#endif
		m_Version.SetText( version );
		
		if( m_CustomScene && m_CustomScene.GetIntroCharacter() )
		{
			m_OriginalCharacterID = m_CustomScene.GetIntroCharacter().GetCharacterID();
			/*#ifdef PLATFORM_CONSOLE
				//m_CustomScene.GetIntroCharacter().SetToDefaultCharacter();
				m_CustomScene.GetIntroCharacter().LoadCharacterData( m_CustomScene.GetIntroCharacter().GetCharacterObj().GetPosition(), m_CustomScene.GetIntroCharacter().GetCharacterObj().GetDirection(), true );
			#endif;*/
		}
		if( m_Scene && m_Scene.GetIntroCharacter() )
		{
			m_OriginalCharacterID = m_Scene.GetIntroCharacter().GetCharacterID();
			/*#ifdef PLATFORM_CONSOLE
				//m_Scene.GetIntroCharacter().SetToDefaultCharacter();
				m_Scene.GetIntroCharacter().LoadCharacterData( m_Scene.GetIntroCharacter().GetCharacterObj().GetPosition(), m_Scene.GetIntroCharacter().GetCharacterObj().GetDirection(), true );
			#endif;*/
		}
		if (m_CustomScene)
		{
			m_NameSelector		= new OptionSelectorEditbox( layoutRoot.FindAnyWidget( "character_name_setting_option" ), m_CustomScene.GetIntroCharacter().GetCharacterName(), null, false );
			m_GenderSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharGenderList() );
		};
		if (m_Scene)
		{
			m_NameSelector		= new OptionSelectorEditbox( layoutRoot.FindAnyWidget( "character_name_setting_option" ), m_Scene.GetIntroCharacter().GetCharacterName(), null, false );
			m_GenderSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_gender_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharGenderList() );
		};
		
		if (m_CustomScene )
		{
			if (m_CustomScene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue( "Female" );
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharList( ECharGender.Female ) );
			}
			else
			{
				m_GenderSelector.SetValue( "Male" );
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharList( ECharGender.Male ) );
			}
		};
		if (m_Scene)
		{
			if (m_Scene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue( "Female" );
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharList( ECharGender.Female ) );
			}
			else
			{
				m_GenderSelector.SetValue( "Male" );
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_head_setting_option" ), 0, null, false, m_Scene.GetIntroCharacter().GetCharList( ECharGender.Male ) );
			}	
		};			
		m_TopSelector		= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_top_setting_option" ), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.BODY) );
		m_BottomSelector	= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_bottom_setting_option" ), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.LEGS) );
		m_ShoesSelector		= new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget( "character_shoes_setting_option" ), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.FEET) );
		
		PlayerBase scene_char = GetPlayerObj();
		if( scene_char )
		{
			Object obj = scene_char.GetInventory().FindAttachment(InventorySlots.BODY);
			if( obj )
				m_TopSelector.SetValue( obj.GetType(), false );
			
			obj = scene_char.GetInventory().FindAttachment(InventorySlots.LEGS);
			if( obj )
				m_BottomSelector.SetValue( obj.GetType(), false );
			
			obj = scene_char.GetInventory().FindAttachment(InventorySlots.FEET);
			if( obj )
				m_ShoesSelector.SetValue( obj.GetType(), false );
			
			m_SkinSelector.SetValue( scene_char.GetType() );
		}
		
		m_GenderSelector.m_OptionChanged.Insert( GenderChanged );
		m_SkinSelector.m_OptionChanged.Insert( SkinChanged );
		m_TopSelector.m_OptionChanged.Insert( TopChanged );
		m_BottomSelector.m_OptionChanged.Insert( BottomChanged );
		m_ShoesSelector.m_OptionChanged.Insert( ShoesChanged );
		
		#ifdef PLATFORM_PS4
			string confirm = "cross";
			string back = "circle";
			if( GetGame().GetInput().GetEnterButton() == GamepadButton.A )
			{
				confirm = "cross";
				back = "circle";
			}
			else
			{
				confirm = "circle";
				back = "cross";
			}
		
			ImageWidget toolbar_a = layoutRoot.FindAnyWidget( "SelectIcon" );
			ImageWidget toolbar_b = layoutRoot.FindAnyWidget( "BackIcon" );
			ImageWidget toolbar_b2 = layoutRoot.FindAnyWidget( "BackIcon0" );
			ImageWidget toolbar_x = layoutRoot.FindAnyWidget( "RandomizeIcon" );
			ImageWidget toolbar_x2 = layoutRoot.FindAnyWidget( "RandomizeIcon0" );
			ImageWidget toolbar_y = layoutRoot.FindAnyWidget( "save_consoleIcon" );
			ImageWidget toolbar_y2 = layoutRoot.FindAnyWidget( "save_consoleIcon0" );
			toolbar_a.LoadImageFile( 0, "set:playstation_buttons image:" + confirm );
			toolbar_b.LoadImageFile( 0, "set:playstation_buttons image:" + back );
			toolbar_b2.LoadImageFile( 0, "set:playstation_buttons image:" + back );
			toolbar_x.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_x2.LoadImageFile( 0, "set:playstation_buttons image:square" );
			toolbar_y.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
			toolbar_y2.LoadImageFile( 0, "set:playstation_buttons image:triangle" );
		#endif
		
		Refresh();
		SetCharacter();
		CheckNewOptions();
		return layoutRoot;
	}

	override void Apply()
	{
		string name;
		if (m_CustomScene && !m_CustomScene.GetIntroCharacter().IsDefaultCharacter() )
		{
			name = m_NameSelector.GetValue();
			if( name == "" )
				name = GameConstants.DEFAULT_CHARACTER_NAME;
			
			m_CustomScene.GetIntroCharacter().SaveCharName(name);
		}
		if (m_Scene && !m_Scene.GetIntroCharacter().IsDefaultCharacter() )
		{
			name = m_NameSelector.GetValue();
			if( name == "" )
				name = GameConstants.DEFAULT_CHARACTER_NAME;
			
			m_Scene.GetIntroCharacter().SaveCharName(name);
		}		
		MainMenu menu_main = MainMenu.Cast(GetGame().GetUIManager().FindMenu(MENU_MAIN));
		if (menu_main)
		{
			menu_main.OnChangeCharacter(false);
		}
		GetGame().GetUIManager().Back();
	}
	
	override void Save()
	{
		if (m_CustomScene && m_CustomScene.GetIntroCharacter().IsDefaultCharacter() )
		{
			m_CustomScene.GetIntroCharacter().SaveDefaultCharacter();
			m_CustomScene.GetIntroCharacter().SetToDefaultCharacter();
			SetCharacterSaved(true);
		}
		if (m_Scene && m_Scene.GetIntroCharacter().IsDefaultCharacter() )
		{
			m_Scene.GetIntroCharacter().SaveDefaultCharacter();
			m_Scene.GetIntroCharacter().SetToDefaultCharacter();
			SetCharacterSaved(true);
		}		
		string name = m_NameSelector.GetValue();
		if( name == "" )
			name = GameConstants.DEFAULT_CHARACTER_NAME;
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SaveCharName(name);
		if (m_Scene)
			m_Scene.GetIntroCharacter().SaveCharName(name);
		
		//GetGame().GetUIManager().Back();
	}
	
	override void Back()
	{
		//bring back DefaultCharacter, if it exists (it should), or a previously played one.
		GetGame().GetMenuData().RequestGetDefaultCharacterData();
		#ifdef PLATFORM_CONSOLE
			if (m_OriginalCharacterID != GameConstants.DEFAULT_CHARACTER_MENU_ID && m_CharacterSaved)
			{
				m_OriginalCharacterID = GameConstants.DEFAULT_CHARACTER_MENU_ID;
			}
		#endif
		if (m_CustomScene)
		{
			m_CustomScene.GetIntroCharacter().SetCharacterID(m_OriginalCharacterID);
			m_CustomScene.GetIntroCharacter().CreateNewCharacterById(m_CustomScene.GetIntroCharacter().GetCharacterID());
		};		
		if (m_Scene)
		{
			m_Scene.GetIntroCharacter().SetCharacterID(m_OriginalCharacterID);
			m_Scene.GetIntroCharacter().CreateNewCharacterById(m_Scene.GetIntroCharacter().GetCharacterID());
		};
		GetGame().GetUIManager().Back();
	}

	override void SetCharacter()
	{
		if (m_CustomScene && m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
		{
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());
		}		
		if (m_Scene && m_Scene.GetIntroCharacter().IsDefaultCharacter())
		{
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());
		}
	}
	
	override void RandomizeCharacter()
	{
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SetToDefaultCharacter();
		if (m_Scene)
			m_Scene.GetIntroCharacter().SetToDefaultCharacter();
		
		// make random selection
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SetCharacterGender( Math.RandomInt(0, 2) );
		if (m_Scene)
			m_Scene.GetIntroCharacter().SetCharacterGender( Math.RandomInt(0, 2) );
		
		if (m_CustomScene)
		{
			if (m_CustomScene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue( "Female" );
				m_SkinSelector.LoadNewValues( m_CustomScene.GetIntroCharacter().GetCharList( ECharGender.Female ), 0 );
				m_SkinSelector.SetRandomValue();
			}
			else
			{
				m_GenderSelector.SetValue( "Male" );
				m_SkinSelector.LoadNewValues( m_CustomScene.GetIntroCharacter().GetCharList( ECharGender.Male ), 0 );
				m_SkinSelector.SetRandomValue();
			}
		}
		if (m_Scene)
		{
			if (m_Scene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue( "Female" );
				m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( ECharGender.Female ), 0 );
				m_SkinSelector.SetRandomValue();
			}
			else
			{
				m_GenderSelector.SetValue( "Male" );
				m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( ECharGender.Male ), 0 );
				m_SkinSelector.SetRandomValue();
			}	
		};			
		GetGame().GetMenuDefaultCharacterData().GenerateRandomEquip();
		
		m_TopSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.BODY),false);
		m_BottomSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.LEGS),false);
		m_ShoesSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.FEET),false);
		
		Refresh();
		SetCharacter();
		
		CheckNewOptions();
	}
	
	//Selector Events
	override void GenderChanged()
	{
		ECharGender gender = ECharGender.Male;
		
		if ( m_GenderSelector.GetStringValue() == "Female" )
		{
			gender = ECharGender.Female;
		}
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SetCharacterGender( gender );		
		if (m_Scene)
			m_Scene.GetIntroCharacter().SetCharacterGender( gender );
		
		if (m_CustomScene)
			m_SkinSelector.LoadNewValues( m_CustomScene.GetIntroCharacter().GetCharList( gender ) , 0 );
		if (m_Scene)
			m_SkinSelector.LoadNewValues( m_Scene.GetIntroCharacter().GetCharList( gender ) , 0 );
		
		m_SkinSelector.SetRandomValue();
		SetCharacterSaved(false);
	}
	
	override void SkinChanged()
	{
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().CreateNewCharacterByName( m_SkinSelector.GetStringValue(), false );		
		if (m_Scene)
			m_Scene.GetIntroCharacter().CreateNewCharacterByName( m_SkinSelector.GetStringValue(), false );
		SetCharacterSaved(false);
		
		//layoutRoot.FindAnyWidget( "character_root" ).Show( m_CustomScene.GetIntroCharacter().IsDefaultCharacter() );
	}
	
	override void TopChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.BODY,m_TopSelector.GetStringValue());
		if (m_CustomScene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());		
		if (m_Scene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());
		SetCharacterSaved(false);
		//m_CustomScene.GetIntroCharacter().SetAttachment( m_TopSelector.GetStringValue(), InventorySlots.BODY );
	}
	
	override void BottomChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.LEGS,m_BottomSelector.GetStringValue());
		if (m_CustomScene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());
		if (m_Scene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());
		
		SetCharacterSaved(false);
		//m_CustomScene.GetIntroCharacter().SetAttachment( m_BottomSelector.GetStringValue(), InventorySlots.LEGS );
	}
	
	override void ShoesChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.FEET,m_ShoesSelector.GetStringValue());
		if (m_CustomScene)		
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());
		if (m_Scene)		
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());
		
		SetCharacterSaved(false);
		//m_CustomScene.GetIntroCharacter().SetAttachment( m_ShoesSelector.GetStringValue(), InventorySlots.FEET );
	}	

	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		#ifndef PLATFORM_CONSOLE
		if ( w == m_CharacterRotationFrame )
		{
			if (m_CustomScene)
				DayZIntroSceneExpansion.Cast( m_CustomScene ).CharacterRotationStart();			
			if (m_Scene)
				DayZIntroScenePC.Cast( m_Scene ).CharacterRotationStart();
			return true;
		}
		#endif
		return false;
	}	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		#ifndef PLATFORM_CONSOLE
		if (m_CustomScene)
			DayZIntroSceneExpansion.Cast( m_CustomScene ).CharacterRotationStop();
		if (m_Scene)
			DayZIntroScenePC.Cast( m_Scene ).CharacterRotationStop();
		
		#endif
		return false;
	}
	override void CheckNewOptions()
	{
		bool show_widgets;
		if (m_CustomScene)
			show_widgets = m_CustomScene.GetIntroCharacter().IsDefaultCharacter();
		if (m_Scene)
			show_widgets = m_Scene.GetIntroCharacter().IsDefaultCharacter();
		
		bool was_visible = layoutRoot.FindAnyWidget( "character_gender_button" ).IsVisible();
		layoutRoot.FindAnyWidget( "character_gender_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_head_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_top_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_bottom_button" ).Show( show_widgets );
		layoutRoot.FindAnyWidget( "character_shoes_button" ).Show( show_widgets );
		
		if (!was_visible && show_widgets)
			m_GenderSelector.Focus();
		if (!show_widgets)
			SetFocus(m_RandomizeCharacter);
	}	
	override void Refresh()
	{
		string name;
		#ifdef PLATFORM_CONSOLE
			if( GetGame().GetUserManager() && GetGame().GetUserManager().GetSelectedUser() )
			{
				name = GetGame().GetUserManager().GetSelectedUser().GetName();
				if( name.LengthUtf8() > 16 )
				{
					name = name.SubstringUtf8(0, 16);
					name += "...";
				}
			}
		#else
			if (m_CustomScene)
				name = m_CustomScene.GetIntroCharacter().GetCharacterName();			
			if (m_Scene)
				name = m_Scene.GetIntroCharacter().GetCharacterName();
			if( name == "" )
				name = GameConstants.DEFAULT_CHARACTER_NAME;
		#endif
		
		m_NameSelector.SetValue( name );
		
		string version;
		GetGame().GetVersion( version );
		#ifdef PLATFORM_CONSOLE
			version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
			m_ConsoleSave.Show(!m_CharacterSaved && m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
			m_Apply.Show(m_CharacterSaved || !m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
			m_Save.Show(!m_CharacterSaved && m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
		#else
			version = "#main_menu_version" + " " + version;
			if (m_CustomScene)
			{
				m_Apply.Show(!m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
				m_Save.Show(m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
			};			
			if (m_Scene)
			{
				m_Apply.Show(!m_Scene.GetIntroCharacter().IsDefaultCharacter());
				m_Save.Show(m_Scene.GetIntroCharacter().IsDefaultCharacter());
			};
		#endif
		if (m_CustomScene)
		{
			m_PlayedCharacterInfo.Show(!m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
		
			if (m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
			{
				m_CharacterHeaderText.SetText("#character_menu_header");
			}
			else
			{
				m_CharacterHeaderText.SetText("#server_browser_prev_play_filter");
			}
		};		
		if (m_Scene)
		{
			m_PlayedCharacterInfo.Show(!m_Scene.GetIntroCharacter().IsDefaultCharacter());
		
			if (m_Scene.GetIntroCharacter().IsDefaultCharacter())
			{
				m_CharacterHeaderText.SetText("#character_menu_header");
			}
			else
			{
				m_CharacterHeaderText.SetText("#server_browser_prev_play_filter");
			}
		};
		
		m_Version.SetText( version );
	}
	
	override void Update(float timeslice)
	{
		if ( GetGame().GetInput().LocalPress("UAUIBack",false) )
		{
			Back();
		}
		
		if ( GetGame().GetInput().LocalPress("UAUICtrlX",false) )
		{
			RandomizeCharacter();
		}
		
		#ifdef PLATFORM_CONSOLE
		if ( GetGame().GetInput().LocalPress("UAUICtrlY",false) )
		{
			if ( m_CustomScene && m_CustomScene.GetIntroCharacter().IsDefaultCharacter() && !m_CharacterSaved )
			{
				Save();
			}			
			if ( m_Scene && m_Scene.GetIntroCharacter().IsDefaultCharacter() && !m_CharacterSaved )
			{
				Save();
			}
		}
		#endif
	}	
};
#endif
