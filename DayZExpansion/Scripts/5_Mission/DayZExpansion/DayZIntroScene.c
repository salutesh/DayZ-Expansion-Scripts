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
	
	void DayZIntroSceneExpansion()
	{


		World world = g_Game.GetWorld();
		string root_path = "cfgExpansionCharacterScenes " + g_Game.GetWorldName();

		int count = g_Game.ConfigGetChildrenCount(root_path);
		//int index = Math.RandomInt(0, count - 1);
		int index = 4;
		
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
		string scene_path = root_path + " " + childName;	
		
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
			PlayerBase dummy = m_Character.GetCharacterObj();
			
			if ( g_Game.ConfigGetInt(scene_path + " CustomPose") != -1)
			{
				EntityAI guitar = dummy.GetInventory().CreateInInventory("Expansion_Guitar");
				dummy.LocalTakeEntityToHands(guitar);
				dummy.StartCommand_Action(g_Game.ConfigGetInt(scene_path + " CustomPose"), ActionBaseCB, DayZPlayerConstants.STANCEMASK_ALL);
			};
			TStringArray mapping = new TStringArray;
			g_Game.ConfigGetTextArray(scene_path + " MappingFiles", mapping);
			NobodyIsOnlineRightNowToHelpMeDoThisProperlySoImDoingThisRIP(mapping);
			//EntityAI logo = g_Game.CreateObject("ExpansionPropLogoPlane", "14162.269531 3.923709 13014.549805")
			//logo.SetOrientation("60.089050 0.000000 0.000000");
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
	
	void SetClickEnable( bool enable )
	{
		m_EnableClick = enable;
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


	void NobodyIsOnlineRightNowToHelpMeDoThisProperlySoImDoingThisRIP(TStringArray filestoload)
	{
		string className;
		vector position;
		vector rotation;
		string special;
		for (int i = 0; i < filestoload.Count(); i++)
		{
			string name = filestoload[i];

			string filePath = EXPANSION_MAPPING_FOLDER + "ChernarusPlus" + "/" + name + EXPANSION_MAPPING_EXT;
			FileHandle file = OpenFile( filePath, FileMode.READ );
			if ( !file )
				return;
			array< Object > objects = new array< Object >;
			while ( GetObjectFromFile( file, className, position, rotation, special ) )
			{
							
				Object obj;
				Print("Spawning object with chached collition: " + className + " on pos: " + position.ToString());
				obj = GetGame().CreateObject(className, position);
					
								
				if( position )
					obj.SetPosition( position );
					
				if( rotation )
					obj.SetOrientation( rotation );


					
				Print("Succesfully spawned object with chached collition: " + className + " on pos: " + position.ToString());
				if ( !obj )
				{
					continue;
				}
					
								
				obj.SetPosition( position );
				obj.SetOrientation( rotation );
				if (className == "Fireplace")
				{
					FireplaceBase fireplace = FireplaceBase.Cast( obj );
					fireplace.GetInventory().CreateAttachment("Bark_Oak");
					fireplace.GetInventory().CreateAttachment("Firewood");
					fireplace.GetInventory().CreateAttachment("WoodenStick");
					fireplace.StartFire(true);

				}				
				if (className == "LongWoodenStick")
				{
					LongWoodenStick stick = LongWoodenStick.Cast( obj );
					stick.GetInventory().CreateAttachment("CowSteakMeat");
				}
			}
			
		}
		
	};
	
	private bool GetObjectFromFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false" )
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
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		#ifndef PLATFORM_CONSOLE
		if ( w == m_CharacterRotationFrame )
		{
			if (m_CustomScene)
				DayZIntroSceneExpansion.Cast( m_CustomScene ).CharacterRotationStart();
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
		#endif
		return false;
	}
};
#endif
