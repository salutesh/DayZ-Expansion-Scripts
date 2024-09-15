class CfgPatches
{
	class DayZExpansion_Sounds_UI
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgSoundShaders
{
	class Expansion_UI_SoundShader
	{
		range=100;
	};
	class Expansion_Shocks_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_0",
				1
			},
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_1",
				1
			},
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_2",
				1
			},
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_3",
				1
			}
		};
		volume=0.5;
	};
	class Expansion_Prints_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Print1",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Print2",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Backs_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Back1",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Back2",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_ClickBeeps_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_01",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_02",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_03",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_04",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Draws_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_01",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_02",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_03",
				1
			},
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_04",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Shock1_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_0",
				1
			}
		};
		volume=0.5;
	};
	class Expansion_Shock2_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_1",
				1
			}
		};
		volume=0.5;
	};
	class Expansion_Shock3_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_2",
				1
			}
		};
		volume=0.5;
	};
	class Expansion_Shock4_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\Characters\misc\wire\shock\shock_3",
				1
			}
		};
		volume=0.5;
	};
	class Expansion_Hint_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Hint",
				0.5
			}
		};
		volume=0.1;
	};
	class Expansion_Print1_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Print1",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Print2_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Print2",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Back1_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Back1",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Back2_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Back2",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Click_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_ClickBeep1_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_01",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_ClickBeep2_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_02",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_ClickBeep3_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_03",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_ClickBeep4_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Click_Beep_04",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Denied_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Denied",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Draw1_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_01",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Draw2_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_02",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Draw3_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_03",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Draw4_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Draw_04",
				1
			}
		};
		volume=0.25;
	};
	class Expansion_Succes_SoundShader: Expansion_UI_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\UI\Expansion_UI_Succes",
				1
			}
		};
		volume=0.25;
	};
};
class CfgSoundSets
{
	class Expansion_UI_SoundSet
	{
		sound3DProcessingType="character3DProcessingType";
		volumeCurve="characterAttenuationCurve";
		spatial=1;
		doppler=0;
		loop=0;
	};
	class Expansion_Shock1_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Shock1_SoundShader"
		};
	};
	class Expansion_Shock2_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Shock2_SoundShader"
		};
	};
	class Expansion_Shock3_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Shock3_SoundShader"
		};
	};
	class Expansion_Shock4_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Shock4_SoundShader"
		};
	};
	class Expansion_Hint_Sound_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Hint_SoundShader"
		};
	};
	class Expansion_Print1_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Print1_SoundShader"
		};
	};
	class Expansion_Print2_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Print2_SoundShader"
		};
	};
	class Expansion_Back1_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Back1_SoundShader"
		};
	};
	class Expansion_Back2_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Back2_SoundShader"
		};
	};
	class Expansion_Shocks_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Shocks_SoundShader"
		};
	};
	class Expansion_Prints_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Prints_SoundShader"
		};
	};
	class Expansion_Backs_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Backs_SoundShader"
		};
	};
	class Expansion_ClickBeeps_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_ClickBeeps_SoundShader"
		};
	};
	class Expansion_Draws_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Draws_SoundShader"
		};
	};
	class Expansion_Click_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Click_SoundShader"
		};
	};
	class Expansion_ClickBeep1_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_ClickBeep1_SoundShader"
		};
	};
	class Expansion_ClickBeep2_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_ClickBeep2_SoundShader"
		};
	};
	class Expansion_ClickBeep3_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_ClickBeep3_SoundShader"
		};
	};
	class Expansion_ClickBeep4_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_ClickBeep4_SoundShader"
		};
	};
	class Expansion_Denied_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Denied_SoundShader"
		};
	};
	class Expansion_Draw1_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Draw1_SoundShader"
		};
	};
	class Expansion_Draw2_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Draw2_SoundShader"
		};
	};
	class Expansion_Draw3_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Draw3_SoundShader"
		};
	};
	class Expansion_Draw4_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Draw4_SoundShader"
		};
	};
	class Expansion_Succes_SoundSet: Expansion_UI_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_Succes_SoundShader"
		};
	};
};
class CfgSounds
{
	class default
	{
		name="";
		titles[]={};
	};
	class Expansion_Hint_Sound: default
	{
		sound[]=
		{
			"\DayZExpansion\Sounds\UI\Expansion_UI_Hint",
			0.89125091,
			1
		};
	};
};
