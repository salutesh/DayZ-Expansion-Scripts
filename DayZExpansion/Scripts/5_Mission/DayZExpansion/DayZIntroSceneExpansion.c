/**
 * DayZIntroSceneExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
class DayZIntroSceneExpansion: Managed
{
	protected Camera m_Camera;
	protected vector m_CameraTrans[4];
	protected vector m_CharacterPos;
	protected vector m_CharacterRot;
	protected Weather m_Weather;
	protected vector m_Target;
	protected ref IntroSceneCharacter	m_Character;
	protected bool m_EnableClick;
	protected bool m_RotatingCharacter;
	protected int m_RotatingCharacterMouseX;
	protected int m_RotatingCharacterMouseY;
	protected float m_RotatingCharacterRot;
	protected float m_Radius;
	protected float m_Radius_original;
	protected float m_DiffX;
	protected float m_DeltaX;
	protected float m_DeltaZ;
	protected float m_Angle;
	protected float m_Angle_offset = 0;
	protected float m_NewX = 0;
	protected float m_NewZ = 0;
	protected float m_BlurValue;
	protected bool m_CanSpin = false;
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

		GetGame().ObjectDelete(m_Camera);
		Class.CastTo(m_Camera, g_Game.CreateObject("staticcamera", g_Game.ConfigGetVector(scene_path + " CameraPosition"), true));

		if (m_Camera)
		{
			m_Camera.SetFOV(g_Game.ConfigGetFloat(scene_path + " fov"));
			m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
			m_Camera.SetActive(true);
			m_Camera.SetOrientation(g_Game.ConfigGetVector(scene_path + " CameraOrientation"));
			m_Character = new IntroSceneCharacter();
			m_Character.LoadCharacterData(g_Game.ConfigGetVector(scene_path + " PlayerPosition"), g_Game.ConfigGetVector(scene_path + " PlayerOrientation"));

			//Equip Player with scene specific stuff here
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
		m_Weather.GetOvercast().SetLimits(overcast, overcast);
		m_Weather.GetRain().SetLimits(rain, rain);
		m_Weather.GetFog().SetLimits(fog, fog );
		m_Weather.GetOvercast().Set(overcast, overcast, 0);
		m_Weather.GetRain().Set(rain, rain, 0);
		m_Weather.GetFog().Set(fog, fog, 0);

		if ( windspeed != -1 )
		{
			m_Weather.SetWindSpeed(windspeed);
			m_Weather.SetWindMaximumSpeed(windspeed);
			m_Weather.SetWindFunctionParams(windspeed,windspeed,0);
		}

		SetClickEnable(true);
		m_DeltaX = Math.AbsFloat(m_CharacterPos[0] - m_Camera.GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_CharacterPos[2] - m_Camera.GetPosition()[2]);

		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}

		if (g_Game.ConfigGetInt(scene_path + " CanRotate") == 1)
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
		}
	}

	void CustomHandItem()
	{
		string item;
		GetGame().ConfigGetText(GetScenePath() + " HandItem", item);

		if (item != "")
		{
			if (m_Character.GetCharacterObj().GetItemInHands())
				GetGame().ObjectDelete(m_Character.GetCharacterObj().GetItemInHands());
			m_Character.GetCharacterObj().GetHumanInventory().CreateInHands(item);
		}
	}

	bool IsClickEnabled()
	{
		return m_EnableClick;
	}

	void CharacterRotationStart()
	{
		m_RotatingCharacter = true;
		g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);

		if (m_Character && m_CanSpin )
		{
			m_RotatingCharacterRot = m_CharacterRot[0];
		}
	}

	void CharacterRotationStop()
	{
		if (m_RotatingCharacter)
		{
			m_RotatingCharacter = false;
		}
	}

	bool IsRotatingCharacter()
	{
		return m_RotatingCharacter;
	}

	void CharacterRotate()
	{
		if (m_Character && m_Character.GetCharacterObj() && m_CanSpin)
		{
			int actual_mouse_x;
			int actual_mouse_y;
			float coef;

			g_Game.GetMousePos(actual_mouse_x, actual_mouse_y);
			m_DiffX = m_RotatingCharacterMouseX - actual_mouse_x;
			coef = (m_RotatingCharacterRot + (m_DiffX * 0.5)) / 360;
			coef = coef - Math.Floor(coef);
			m_CharacterRot[0] = coef * 360;
			m_Character.GetCharacterObj().SetOrientation(m_CharacterRot);
		}
	}

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
		if (user_manager)
		{
			BiosUser user = user_manager.GetSelectedUser();
			if (user)
			{
				g_Game.SetPlayerGameName(user.GetName());
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
				obj = GetGame().CreateObject(className, position);
				if (!obj)
					continue;

				if (position)
					obj.SetPosition(position);

				if (rotation)
					obj.SetOrientation(rotation);

				if (attachments)
				{
					EntityAI itemWithAttachment = EntityAI.Cast(obj);
					if (itemWithAttachment)
					{
						for (int j = 0; j < attachments.Count(); j++)
						{
							itemWithAttachment.GetInventory().CreateAttachment(attachments[j]);
						}
					}
				}

				obj.SetPosition(position);
				obj.SetOrientation(rotation);

			#ifdef NAMALSK_SURVIVAL
				vfx_the_thing theThing;
				if (Class.CastTo(theThing, obj))
				{
					theThing.SetStable(true);
				}
			#endif
			}
		}
	};

	private bool GetObjectFromFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false", out TStringArray itemAttachments = NULL )
	{
		string line;
		int lineSize = FGets(file, line);

		if (lineSize < 1)
			return false;

		TStringArray tokens = new TStringArray;
		line.Split("|", tokens);
		name = tokens.Get(0);
		position = tokens.Get(1).ToVector();
		rotation = tokens.Get(2).ToVector();
		special = tokens.Get(3);
		string gear_array = tokens.Get(4);
		TStringArray geartokens = new TStringArray;
		gear_array.Split(",", geartokens);
		itemAttachments = geartokens;

		return true;
	}
};
#endif
