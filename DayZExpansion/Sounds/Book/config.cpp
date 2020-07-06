#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Book
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgSoundShaders
{
	class Expansion_Book_SoundShader
	{
		volume = 5;
		frequency = 1;
		range = 10;
		rangeCurve[] = {{0,1},{5,0.7},{10,0}};
		limitation = 0;
	};
	class Expansion_OpenBook_SoundShader: Expansion_Book_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Book\sound_book_page1",1}};
	};
	class Expansion_SwitchPage_SoundShader: Expansion_Book_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Book\sound_book_page2",1},{"DayZExpansion\Sounds\Book\sound_book_page3",1},{"DayZExpansion\Sounds\Book\sound_book_page4",1}};
	};
	class Expansion_CloseBook_SoundShader: Expansion_Book_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Book\sound_book_close",1}};
	};
};
class CfgSoundSets
{
	class Expansion_Book_SoundSet
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Expansion_OpenBook_SoundSet: Expansion_Book_SoundSet
	{
		soundShaders[] = {"Expansion_OpenBook_SoundShader"};
	};
	class Expansion_SwitchPage_SoundSet: Expansion_Book_SoundSet
	{
		soundShaders[] = {"Expansion_SwitchPage_SoundShader"};
	};
	class Expansion_CloseBook_SoundSet: Expansion_Book_SoundSet
	{
		soundShaders[] = {"Expansion_CloseBook_SoundShader"};
	};
};
