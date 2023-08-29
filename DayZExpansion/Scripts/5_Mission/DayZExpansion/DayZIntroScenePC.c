/**
 * DayZIntroScenePC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZIntroScenePC
{
	protected string m_Expansion_CustomScene;
	protected bool m_Expansion_CanRotateCharacter;
	protected bool m_Expansion_CanUseCustomSceneInCharacterCreation;

	void DayZIntroScenePC()
	{
		if (GetExpansionSettings().GetGeneral().UseExpansionMainMenuIntroScene)
			Expansion_LoadRandomScene("cfgExpansionCharacterScenes " + g_Game.GetWorldName());
	}

	//! @brief Load random Expansion intro scene from `root_path`.
	//! If `characterCreation` is set to `true`, limit to scenes that have `CanUseInCharacterCreation` set in config.
	//! @return true on success, false of failure (i.e. no or no suitable scenes found in root path)
	bool Expansion_LoadRandomScene(string root_path, bool characterCreation = false)
	{
		int count = g_Game.ConfigGetChildrenCount(root_path);
		if (count == 0)
			return false;

		string childName;

		if (characterCreation)
		{
			TStringArray ccScenes = {};

			for (int i = 0; i < count; i++)
			{
				GetGame().ConfigGetChildName(root_path, i, childName);

				if (GetGame().ConfigGetInt(root_path + " " + childName + " CanUseInCharacterCreation"))
					ccScenes.Insert(childName);
			}

			if (ccScenes.Count() == 0)
				return false;

			childName = ccScenes.GetRandomElement();
		}
		else
		{
			int index = Math.RandomInt(0, count);
			g_Game.ConfigGetChildName(root_path, index, childName);
		}

		Expansion_LoadScene(root_path, childName, characterCreation);

		return true;
	}

	void Expansion_LoadScene(string root_path, string childName, bool characterCreation = false)
	{
		string scene_path = root_path + " " + childName;
		m_Expansion_CustomScene = scene_path;

		World world = g_Game.GetWorld();

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
			m_Camera.SetFOV(g_Game.ConfigGetFloat(m_Expansion_CustomScene + " fov"));
			m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
			m_Camera.SetActive(true);
			m_Camera.SetOrientation(g_Game.ConfigGetVector(m_Expansion_CustomScene + " CameraOrientation"));

			//! Equip Player with scene specific stuff here
			Expansion_SetupCharacter(characterCreation);

			TStringArray mapping = new TStringArray;
			g_Game.ConfigGetTextArray(scene_path + " MappingFiles", mapping);
			ExpansionWorldObjectsModule.LoadObjects(mapping, true);
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

		m_DeltaX = Math.AbsFloat(m_CharacterPos[0] - m_Camera.GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_CharacterPos[2] - m_Camera.GetPosition()[2]);

		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}

		if (g_Game.ConfigGetInt(scene_path + " CanRotate") == 1)
			m_Expansion_CanRotateCharacter = true;
		else
			m_Expansion_CanRotateCharacter = false;

		if (g_Game.ConfigGetInt(scene_path + " CanUseInCharacterCreation") == 1)
			m_Expansion_CanUseCustomSceneInCharacterCreation = true;
		else
			m_Expansion_CanUseCustomSceneInCharacterCreation = false;
		
		//! Handle special scenes based on scene config class
		Expansion_HandleCustomScene(childName);
	}

	void Expansion_SetupCharacter(bool characterCreation = false)
	{
		auto trace = EXTrace.StartStack(EXTrace.MISC, this, "characterCreation " + characterCreation);

		m_Character.Expansion_SetPosition(g_Game.ConfigGetVector(m_Expansion_CustomScene + " PlayerPosition"));
		m_Character.Expansion_SetOrientation(g_Game.ConfigGetVector(m_Expansion_CustomScene + " PlayerOrientation"));

		if (characterCreation)
		{
			//! Fugly: I couldn't find a way to reset pose when entering CC menu, so the whole character is re-created - lava76
			m_Character.CreateNewCharacterById(m_Character.GetCharacterID());
		}
		else
		{
			string item;
			GetGame().ConfigGetText(m_Expansion_CustomScene + " HandItem", item);

			if (item != "")
			{
				if (m_Character.GetCharacterObj().GetItemInHands())
					GetGame().ObjectDelete(m_Character.GetCharacterObj().GetItemInHands());
				m_Character.GetCharacterObj().GetHumanInventory().CreateInHands(item);
			}

			int customPose = g_Game.ConfigGetInt(m_Expansion_CustomScene + " CustomPose");
			if (customPose != -1)
				m_Character.GetCharacterObj().StartCommand_Action(customPose, ActionBaseCB, DayZPlayerConstants.STANCEMASK_ALL);
		}
	}
	
	void Expansion_HandleCustomScene(string childName)
	{
		switch (childName)
		{
			case "enoch_bunker_entrance":
			{
				Land_Underground_Entrance.m_Expansion_IsIntroScene = true;

				//! Get and trigger the bunker entrance door
				array<Object> objects = new array<Object>;
				GetGame().GetObjectsAtPosition("742.4 532.942 1225.78", 5.0, objects, null);
				
				foreach (Object obj: objects)
				{
					Land_Underground_Panel undergroundPanel;
					if (Class.CastTo(undergroundPanel, obj))
					{
						undergroundPanel.Interact();
						break;
					}
				}
			}
			break;
		}
	}

	void Expansion_LoadRandomSceneVanilla()
	{
		string root_path = "cfgCharacterScenes " + g_Game.GetWorldName();

		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count - 1);
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);

		Expansion_LoadSceneVanilla(root_path, childName);
	}

	void Expansion_LoadSceneVanilla(string root_path, string childName)
	{
		string scene_path = root_path + " " + childName;
		m_Expansion_CustomScene = string.Empty;

		m_Target = SwapYZ(g_Game.ConfigGetVector(scene_path + " target"));
		vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + " position"));
		TIntArray date = new TIntArray;
		TFloatArray storm = new TFloatArray;
		g_Game.ConfigGetIntArray(scene_path + " date", date);
		float fov = g_Game.ConfigGetFloat(scene_path + " fov");
		float overcast = g_Game.ConfigGetFloat(scene_path + " overcast");
		float rain = g_Game.ConfigGetFloat(scene_path + " rain");
		float fog = g_Game.ConfigGetFloat(scene_path + " fog");
		float windspeed = -1;
		if ( g_Game.ConfigIsExisting(scene_path + " windspeed") ) 	windspeed = g_Game.ConfigGetFloat(scene_path + " windspeed");
		g_Game.ConfigGetFloatArray(scene_path + " storm", storm);
		
		World world = g_Game.GetWorld();
	
		if (world && date.Count() >= 5)
		{
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
		}
	
		GetGame().ObjectDelete( m_Camera );
		Class.CastTo(m_Camera, g_Game.CreateObject("staticcamera", SnapToGround(position), true)); //Vector(position[0] , position[1] + 1, position[2])
		
		Math3D.MatrixIdentity4(m_CameraTrans);
		
		if (m_Camera)
		{
			//m_Camera.SetPosition(Vector(m_Camera.GetPosition()[0],m_Camera.GetPosition()[1]+0,m_Camera.GetPosition()[2]));
			m_Camera.LookAt(m_Target);
			m_Camera.SetFOV(fov);
			m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
			
			m_Camera.SetActive(true);
			
			Math3D.DirectionAndUpMatrix(m_Target - SnapToGround(position), "0 1 0", m_CameraTrans);
			m_CameraTrans[3] = m_Camera.GetPosition();
			m_CharacterPos = Vector(0.685547, -0.988281, 3.68823).Multiply4(m_CameraTrans);

			float pos_x = m_CharacterPos[0];
			float pos_z = m_CharacterPos[2];
			float pos_y = GetGame().SurfaceY(pos_x, pos_z);
			vector ground_demo_pos = Vector(pos_x, pos_y, pos_z);
			m_CharacterPos = ground_demo_pos;

			vector to_cam_dir = m_Camera.GetPosition() - m_CharacterPos;
			m_CharacterRot[0] = Math.Atan2(to_cam_dir[0], to_cam_dir[2]) * Math.RAD2DEG;
		}
		
		m_Weather = g_Game.GetWeather();
		m_Weather.GetOvercast().SetLimits( overcast, overcast );
		m_Weather.GetRain().SetLimits( rain, rain );
		m_Weather.GetFog().SetLimits( fog, fog );
		
		m_Weather.GetOvercast().Set( overcast, 0, 0);
		m_Weather.GetRain().Set( rain, 0, 0);
		m_Weather.GetFog().Set( fog, 0, 0);
		
		if ( storm.Count() == 3 )
		{
			m_Weather.SetStorm(storm.Get(0),storm.Get(1),storm.Get(2));
		}
		
		if ( windspeed != -1 )
		{
			m_Weather.SetWindSpeed(windspeed);
			m_Weather.SetWindMaximumSpeed(windspeed);
			m_Weather.SetWindFunctionParams(1,1,1);
		}
		
		m_Character.LoadCharacterData(m_CharacterPos, m_CharacterRot);
		
		PPEffects.Init(); //Deprecated, left in for legacy purposes only
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(SetInitPostprocesses);
	}

	bool Expansion_IsCustomScene()
	{
		return m_Expansion_CustomScene != string.Empty;
	}

	bool Expansion_CanUseCustomSceneInCharacterCreation()
	{
		return m_Expansion_CanUseCustomSceneInCharacterCreation;
	}

	override void ResetIntroCamera()
	{
		if (!m_Expansion_CustomScene)
			super.ResetIntroCamera();
	}

	override void CharacterRotationStart()
	{
		if (!m_Expansion_CustomScene || m_Expansion_CanRotateCharacter || g_Game.GetUIManager().GetMenu().IsInherited(CharacterCreationMenu))
			super.CharacterRotationStart();
	}
};
