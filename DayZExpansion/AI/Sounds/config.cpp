class CfgPatches
{
	class DayZExpansion_Sounds_AI
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSoundShaders
{
	class baseCharacter_SoundShader;
	class Expansion_AI_The_Sound_Of_Love_01_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\The_Sound_Of_Love_01",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_The_Sound_Of_Love_02_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\The_Sound_Of_Love_02",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_0_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\o",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_1_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\1",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_2_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\2",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_3_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\3",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_4_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\4",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_5_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\5",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_6_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\6",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_7_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\7",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_8_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\8",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_9_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\9",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_i_can_see_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\i_can_see",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_hundred_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\hundred",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_jack_shit_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\jack_shit",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_meters_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\meters",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_Female_thousand_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\thousand",
				1
			}
		};
		volume=1;
	};
	class Expansion_AI_ShoryukenF_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\ShoryukenF",
				1
			}
		};
		volume=0.75;
	};
	class Expansion_AI_ShoryukenM_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\ShoryukenM",
				1
			}
		};
		volume=0.75;
	};
	class Expansion_AI_Heavy_Punch_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\Heavy_Punch",
				1
			}
		};
		volume=1.25;
	};
	class Expansion_AI_UahUahUahM_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			
			{
				"\DayZExpansion\AI\Sounds\UahUahUahM",
				1
			}
		};
		volume=1.25;
	};
};
class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class Expansion_AI_The_Sound_Of_Love_01_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_The_Sound_Of_Love_01_SoundShader"
		};
	};
	class Expansion_AI_The_Sound_Of_Love_02_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_The_Sound_Of_Love_02_SoundShader"
		};
	};
	class Expansion_AI_Female_0_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_0_SoundShader"
		};
	};
	class Expansion_AI_Female_1_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_1_SoundShader"
		};
	};
	class Expansion_AI_Female_2_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_2_SoundShader"
		};
	};
	class Expansion_AI_Female_3_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_3_SoundShader"
		};
	};
	class Expansion_AI_Female_4_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_4_SoundShader"
		};
	};
	class Expansion_AI_Female_5_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_5_SoundShader"
		};
	};
	class Expansion_AI_Female_6_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_6_SoundShader"
		};
	};
	class Expansion_AI_Female_7_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_7_SoundShader"
		};
	};
	class Expansion_AI_Female_8_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_8_SoundShader"
		};
	};
	class Expansion_AI_Female_9_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_9_SoundShader"
		};
	};
	class Expansion_AI_Female_hundred_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_hundred_SoundShader"
		};
	};
	class Expansion_AI_Female_i_can_see_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_i_can_see_SoundShader"
		};
	};
	class Expansion_AI_Female_jack_shit_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_jack_shit_SoundShader"
		};
	};
	class Expansion_AI_Female_meters_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_meters_SoundShader"
		};
	};
	class Expansion_AI_Female_thousand_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Female_thousand_SoundShader"
		};
	};
	class Expansion_AI_ShoryukenF_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_ShoryukenF_SoundShader"
		};
	};
	class Expansion_AI_ShoryukenM_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_ShoryukenM_SoundShader"
		};
	};
	class Expansion_AI_Heavy_Punch_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_Heavy_Punch_SoundShader"
		};
	};
	class Expansion_AI_UahUahUahM_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"Expansion_AI_UahUahUahM_SoundShader"
		};
	};
};
