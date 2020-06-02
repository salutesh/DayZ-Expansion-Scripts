/**
 * config.cpp
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Config
 * @brief		This file handle expansion core configs
 **/
class CfgPatches
{
    class DayZExpansion_Worlds_ChernarusPlus
    {
        units[]={};
        weapons[]={};
        requiredVersion=0.1;
        requiredAddons[]= 
        { 
            "DZ_Data",
            "DZ_Worlds_Chernarusplus_World",
			"DZ_Gear_Navigation"
        };
    };
};

class CfgWorlds
{
    class Weather;
    class Overcast;
	class Weather1;
	class Weather2;
	class Weather3;
	class Weather4;
	class Weather5;
    class LightingNew;
	
    class C_Night;
    class C_Astro1;
    class C_Astro2;
    class C_Astro3;
    class C_Nautical1;
    class C_Nautical2;
    class C_Nautical3;
    class C_Civil1;
    class C_Civil2;
    class C_Civil3;
    class C_Day0;
    class C_Day4;
    class C_Day8;
    class C_Day12;
    class C_Day16;
    class C_Day35;
    class C_Day60;
    class O_Night;
    class O_Astro1;
    class O_Nautical1;
    class O_Civil1;
    class O_Civil2;
    class O_Day0;
    class O_Day4;
    class O_Day8;
    class O_Day12;
    class O_Day35;
    class O_Day60;
	
	class Settlement_Chernogorsk;
	class Settlement_Novodmitrovsk;
	class Settlement_Novoselki;
	class Settlement_Dubovo;
	class Settlement_Vysotovo;
	class Settlement_Zelenogorsk;
	class Settlement_Berezino;
	class Settlement_Elektrozavodsk;
	class Settlement_NovayaPetrovka;
	class Settlement_Gorka;
	class Settlement_Solnechny;
	class Settlement_StarySobor;
	class Settlement_Vybor;
	class Settlement_Severograd;
	class Settlement_Bor;
	class Settlement_Svetloyarsk;
	class Settlement_Krasnostav;
	class Settlement_ChernayaPolyana;
	class Settlement_Polyana;
	class Settlement_Tulga;
	class Settlement_Msta;
	class Settlement_Staroye;
	class Settlement_Shakhovka;
	class Settlement_Dolina;
	class Settlement_Orlovets;
	class Settlement_NovySobor;
	class Settlement_Kabanino;
	class Settlement_Mogilevka;
	class Settlement_Nadezhdino;
	class Settlement_Guglovo;
	class Settlement_Kamyshovo;
	class Settlement_Pusta;
	class Settlement_Dubrovka;
	class Settlement_VyshnayaDubrovka;
	class Settlement_Khelm;
	class Settlement_Olsha;
	class Settlement_Gvozdno;
	class Settlement_Grishino;
	class Settlement_Rogovo;
	class Settlement_Pogorevka;
	class Settlement_Pustoshka;
	class Settlement_Kozlovka;
	class Settlement_Karmanovka;
	class Settlement_Balota;
	class Settlement_Komarovo;
	class Settlement_Kamenka;
	class Settlement_Myshkino;
	class Settlement_Pavlovo;
	class Settlement_Lopatino;
	class Settlement_Vyshnoye;
	class Settlement_Prigorodki;
	class Settlement_Drozhino;
	class Settlement_Sosnovka;
	class Settlement_Nizhneye;
	class Settlement_Pulkovo;
	class Settlement_Berezhki;
	class Settlement_Turovo;
	class Settlement_BelayaPolyana;
	class Settlement_Dobroye;
	class Settlement_Nagornoye;
	class Settlement_Svergino;
	class Settlement_Ratnoye;
	class Settlement_Kamensk;
	class Settlement_Krasnoye;
	class Settlement_StaryYar;
	class Settlement_Polesovo;
	class Settlement_Tisy;
	class Settlement_Topolniki;
	class Settlement_Zaprudnoye;
	class Settlement_Sinystok;
	class Settlement_Vavilovo;
	class Settlement_Kumyrna;
	class Settlement_Kalinovka;
	class Settlement_Bogatyrka;
	class Settlement_SvyatoyRomanSkiResort;
	class Settlement_SKVSChBiathlonArena;
	class Settlement_Zvir;
	class Settlement_Zabolotye;
	class Settlement_Galkino;
	class Camp_Arsenovo;
	class Camp_Stroytel;
	class Camp_Romashka;
	class Camp_Kometa;
	class Camp_Druzhba;
	class Camp_Nadezhda;
	class Camp_YouthPioneer;
	class Camp_ProudChernarus;
	class Camp_Shkolnik;
	class Camp_Pobeda;
	class Camp_Metalurg;
	class Hill_Zelenayagora;
	class Local_Dichina;
	class Local_Novylug;
	class Local_Staryeluga;
	class Hill_Vysota;
	class Hill_Kopyto;
	class Local_Grubyeskaly;
	class Local_Chernyles;
	class Hill_Altar;
	class Local_RadioZenit;
	class Hill_PikKozlova;
	class Local_Pustoykhrebet;
	class Hill_Bashnya;
	class Hill_Veresnik;
	class Hill_Kurgan;
	class Hill_Kustryk;
	class Hill_Vetryanayagora;
	class Hill_Kalmyk;
	class Hill_PopIvan;
	class Hill_Erbenka;
	class Local_Lesnoykhrebet;
	class Hill_Vysokiykamen;
	class Hill_Dobry;
	class Hill_Baranchik;
	class Hill_Malinovka;
	class Hill_Dubina;
	class Hill_Klen;
	class Hill_Chernayagora;
	class Ruin_Zolotar;
	class Hill_Ostry;
	class Hill_Olsha;
	class Marine_Tikhiyzaliv;
	class Marine_Mutnyizaliv;
	class Marine_Chernyzaliv;
	class Marine_Zelenyzaliv;
	class Marine_Skalistyproliv;
	class Marine_Nizhniyzaliv;
	class Marine_ZalivGuba;
	class Marine_Rify;
	class Marine_Ivovoyeozero;
	class Marine_Chernoyeozero;
	class Marine_PlotinaTopolka;
	class Marine_PlotinaPobeda;
	class Marine_PlotinaTishina;
	class Marine_Ozerko;
	class Marine_Prud;
	class Ruin_Chortovzamok;
	class Ruin_Zub;
	class Ruin_Rog;
	class Local_Grozovypereval;
	class Local_Sosnovypereval;
	class Local_PerevalOreshka;
	class Local_Turovskiypereval;
	class Local_Tridoliny;
	class Local_Grozovyedoly;
	class Ruin_Klyuch;
	class Hill_Lysayagora;
	class Marine_Glubokoyeozero;
	class Local_Skalka;
	class Local_Vidy;
	class Hill_Tumannyverkh;
	class Local_Adamovka;
	class Hill_Shishok;
	class Settlement_Skalisty;
	class Ruin_Storozh;
	class Local_MysGolova;
	class Local_Drakon;
	class Local_Otmel;
	class Local_MysKrutoy;
	class Hill_Tokarnya;
	class Hill_Ostrog;
	class Local_Maryanka;
	class Local_Polonina;
	class Local_Kalinka;
	class Hill_Kikimora;
	class Hill_BolshoyKotel;
	class Hill_Simurg;
	class Hill_Volchiypik;
	class Ruin_Krona;
	class Local_TriKresta;
	class Local_Rostoki;
	class Marine_OrlyeOzero;
	class Local_Makosh;
	class Local_Klenovyipereval;
	class Local_Zmeinykhrebet;
	class Hill_Sokol;
	class Local_Krutyeskaly;
	class Local_Bogat;
	class Local_Dubnik;
	class Hill_Viselitsa;
	class Local_Dazhbog;
	class Marine_Verbnik;
	class Local_Medvezhilugi;
	class Ruin_Voron;
	class Ruin_Gnomovzamok;
	class Marine_Glaza;
	class Local_KarerKrasnayaZarya;
	class Local_Matveyevo;
	class Local_Kotka;
	class Local_Chernyeskaly;
	class Hill_SvyatoyRoman;
	class Hill_Koman;
	class Hill_Mayak;
	class Local_MB_VMC;
	class Local_MB_Tisy;
	class Local_MB_Zeleno;
	class Local_MB_Kamensk;
	class Local_MB_Pavlovo;
	class Local_AF_Balota;
	class Local_AF_Krasno;
	class Local_AF_Vybor;

    class VolFog;
	class Names;

    class SoundMapValues;
    class Subdivision;
    class Fractal;
    class WhiteNoise; 

    class DefaultLighting;

	class CAWorld;
	class ChernarusPlus : CAWorld
	{
		class Names: Names
		{
			class Settlement_Chernogorsk: Settlement_Chernogorsk
			{
				name = "Chernogorsk";
			};
			class Settlement_Novodmitrovsk: Settlement_Novodmitrovsk
			{
				name = "Novodmitrovsk";
			};
			class Settlement_Novoselki: Settlement_Novoselki
			{
				name = "Novoselki";
			};
			class Settlement_Dubovo: Settlement_Dubovo
			{
				name = "Dubovo";
			};
			class Settlement_Vysotovo: Settlement_Vysotovo
			{
				name = "Vysotovo";
			};
			class Settlement_Zelenogorsk: Settlement_Zelenogorsk
			{
				name = "Zelenogorsk";
			};
			class Settlement_Berezino: Settlement_Berezino
			{
				name = "Berezino";
			};
			class Settlement_Elektrozavodsk: Settlement_Elektrozavodsk
			{
				name = "Elektrozavodsk";
			};
			class Settlement_NovayaPetrovka: Settlement_NovayaPetrovka
			{
				name = "Novaya Petrovka";
			};
			class Settlement_Gorka: Settlement_Gorka
			{
				name = "Gorka";
			};
			class Settlement_Solnechny: Settlement_Solnechny
			{
				name = "Solnechny";
			};
			class Settlement_StarySobor: Settlement_StarySobor
			{
				name = "Stary Sobor";
			};
			class Settlement_Vybor: Settlement_Vybor
			{
				name = "Vybor";
			};
			class Settlement_Severograd: Settlement_Severograd
			{
				name = "Severograd";
			};
			class Settlement_Bor: Settlement_Bor
			{
				name = "Bor";
			};
			class Settlement_Svetloyarsk: Settlement_Svetloyarsk
			{
				name = "Svetloyarsk";
			};
			class Settlement_Krasnostav: Settlement_Krasnostav
			{
				name = "Krasnostav";
			};
			class Settlement_ChernayaPolyana: Settlement_ChernayaPolyana
			{
				name = "Chernaya Polyana";
			};
			class Settlement_Polyana: Settlement_Polyana
			{
				name = "Polyana";
			};
			class Settlement_Tulga: Settlement_Tulga
			{
				name = "Tulga";
			};
			class Settlement_Msta: Settlement_Msta
			{
				name = "Msta";
			};
			class Settlement_Staroye: Settlement_Staroye
			{
				name = "Staroye";
			};
			class Settlement_Shakhovka: Settlement_Shakhovka
			{
				name = "Shakhovka";
			};
			class Settlement_Dolina: Settlement_Dolina
			{
				name = "Dolina";
			};
			class Settlement_Orlovets: Settlement_Orlovets
			{
				name = "Orlovets";
			};
			class Settlement_NovySobor: Settlement_NovySobor
			{
				name = "Novy Sobor";
			};
			class Settlement_Kabanino: Settlement_Kabanino
			{
				name = "Kabanino";
			};
			class Settlement_Mogilevka: Settlement_Mogilevka
			{
				name = "Mogilevka";
			};
			class Settlement_Nadezhdino: Settlement_Nadezhdino
			{
				name = "Nadezhdino";
			};
			class Settlement_Guglovo: Settlement_Guglovo
			{
				name = "Guglovo";
			};
			class Settlement_Kamyshovo: Settlement_Kamyshovo
			{
				name = "Kamyshovo";
			};
			class Settlement_Pusta: Settlement_Pusta
			{
				name = "Pusta";
			};
			class Settlement_Dubrovka: Settlement_Dubrovka
			{
				name = "Dubrovka";
			};
			class Settlement_VyshnayaDubrovka: Settlement_VyshnayaDubrovka
			{
				name = "Vyshnaya Dubrovka";
			};
			class Settlement_Khelm: Settlement_Khelm
			{
				name = "Khelm";
			};
			class Settlement_Olsha: Settlement_Olsha
			{
				name = "Olsha";
			};
			class Settlement_Gvozdno: Settlement_Gvozdno
			{
				name = "Gvozdno";
			};
			class Settlement_Grishino: Settlement_Grishino
			{
				name = "Grishino";
			};
			class Settlement_Rogovo: Settlement_Rogovo
			{
				name = "Rogovo";
			};
			class Settlement_Pogorevka: Settlement_Pogorevka
			{
				name = "Pogorevka";
			};
			class Settlement_Pustoshka: Settlement_Pustoshka
			{
				name = "Pustoshka";
			};
			class Settlement_Kozlovka: Settlement_Kozlovka
			{
				name = "Kozlovka";
			};
			class Settlement_Karmanovka: Settlement_Karmanovka
			{
				name = "Karmanovka";
			};
			class Settlement_Balota: Settlement_Balota
			{
				name = "Balota";
			};
			class Settlement_Komarovo: Settlement_Komarovo
			{
				name = "Komarovo";
			};
			class Settlement_Kamenka: Settlement_Kamenka
			{
				name = "Kamenka";
			};
			class Settlement_Myshkino: Settlement_Myshkino
			{
				name = "Myshkino";
			};
			class Settlement_Pavlovo: Settlement_Pavlovo
			{
				name = "Pavlovo";
			};
			class Settlement_Lopatino: Settlement_Lopatino
			{
				name = "Lopatino";
			};
			class Settlement_Vyshnoye: Settlement_Vyshnoye
			{
				name = "Vyshnoye";
			};
			class Settlement_Prigorodki: Settlement_Prigorodki
			{
				name = "Prigorodki";
			};
			class Settlement_Drozhino: Settlement_Drozhino
			{
				name = "Drozhino";
			};
			class Settlement_Sosnovka: Settlement_Sosnovka
			{
				name = "Sosnovka";
			};
			class Settlement_Nizhneye: Settlement_Nizhneye
			{
				name = "Nizhneye";
			};
			class Settlement_Pulkovo: Settlement_Pulkovo
			{
				name = "Pulkovo";
			};
			class Settlement_Berezhki: Settlement_Berezhki
			{
				name = "Berezhki";
			};
			class Settlement_Turovo: Settlement_Turovo
			{
				name = "Turovo";
			};
			class Settlement_BelayaPolyana: Settlement_BelayaPolyana
			{
				name = "Belaya Polyana";
			};
			class Settlement_Dobroye: Settlement_Dobroye
			{
				name = "Dobroye";
			};
			class Settlement_Nagornoye: Settlement_Nagornoye
			{
				name = "Nagornoye";
			};
			class Settlement_Svergino: Settlement_Svergino
			{
				name = "Svergino";
			};
			class Settlement_Ratnoye: Settlement_Ratnoye
			{
				name = "Ratnoye";
			};
			class Settlement_Kamensk: Settlement_Kamensk
			{
				name = "Kamensk";
			};
			class Settlement_Krasnoye: Settlement_Krasnoye
			{
				name = "Krasnoye";
			};
			class Settlement_StaryYar: Settlement_StaryYar
			{
				name = "Stary Yar";
			};
			class Settlement_Polesovo: Settlement_Polesovo
			{
				name = "Polesovo";
			};
			class Settlement_Tisy: Settlement_Tisy
			{
				name = "Tisy";
			};
			class Settlement_Topolniki: Settlement_Topolniki
			{
				name = "Topolniki";
			};
			class Settlement_Zaprudnoye: Settlement_Zaprudnoye
			{
				name = "Zaprudnoye";
			};
			class Settlement_Sinystok: Settlement_Sinystok
			{
				name = "Sinystok";
			};
			class Settlement_Vavilovo: Settlement_Vavilovo
			{
				name = "Vavilovo";
			};
			class Settlement_Kumyrna: Settlement_Kumyrna
			{
				name = "Kumyrna";
			};
			class Settlement_Kalinovka: Settlement_Kalinovka
			{
				name = "Kalinovka";
			};
			class Settlement_Bogatyrka: Settlement_Bogatyrka
			{
				name = "Bogatyrka";
			};
			class Settlement_SvyatoyRomanSkiResort: Settlement_SvyatoyRomanSkiResort
			{
				name = "Svyatoy-Roman Ski Resort";
			};
			class Settlement_SKVSChBiathlonArena: Settlement_SKVSChBiathlonArena
			{
				name = "SKVSCh Biathlon Arena";
			};
			class Settlement_Zvir: Settlement_Zvir
			{
				name = "Zvir";
			};
			class Settlement_Zabolotye: Settlement_Zabolotye
			{
				name = "Zabolotye";
			};
			class Settlement_Galkino: Settlement_Galkino
			{
				name = "Galkino";
			};
			class Camp_Arsenovo: Camp_Arsenovo
			{
				name = "Arsenovo";
			};
			class Camp_Stroytel: Camp_Stroytel
			{
				name = "Stroytel";
			};
			class Camp_Romashka: Camp_Romashka
			{
				name = "Romashka";
			};
			class Camp_Kometa: Camp_Kometa
			{
				name = "Kometa";
			};
			class Camp_Druzhba: Camp_Druzhba
			{
				name = "Druzhba";
			};
			class Camp_Nadezhda: Camp_Nadezhda
			{
				name = "Nadezhda";
			};
			class Camp_YouthPioneer: Camp_YouthPioneer
			{
				name = "Youth Pioneer";
			};
			class Camp_ProudChernarus: Camp_ProudChernarus
			{
				name = "Proud Chernarus";
			};
			class Camp_Shkolnik: Camp_Shkolnik
			{
				name = "Shkolnik";
			};
			class Camp_Pobeda: Camp_Pobeda
			{
				name = "Pobeda";
			};
			class Camp_Metalurg: Camp_Metalurg
			{
				name = "Metalurg";
			};
			class Hill_Zelenayagora: Hill_Zelenayagora
			{
				name = "Zelenaya gora";
			};
			class Local_Dichina: Local_Dichina
			{
				name = "Dichina";
			};
			class Local_Novylug: Local_Novylug
			{
				name = "Novy lug";
			};
			class Local_Staryeluga: Local_Staryeluga
			{
				name = "Starye luga";
			};
			class Hill_Vysota: Hill_Vysota
			{
				name = "Vysota";
			};
			class Hill_Kopyto: Hill_Kopyto
			{
				name = "Kopyto";
			};
			class Local_Grubyeskaly: Local_Grubyeskaly
			{
				name = "Grubye skaly";
			};
			class Local_Chernyles: Local_Chernyles
			{
				name = "Cherny les";
			};
			class Hill_Altar: Hill_Altar
			{
				name = "Altar";
			};
			class Local_RadioZenit: Local_RadioZenit
			{
				name = "Radio Zenit";
			};
			class Hill_PikKozlova: Hill_PikKozlova
			{
				name = "Pik Kozlova";
			};
			class Local_Pustoykhrebet: Local_Pustoykhrebet
			{
				name = "Pustoy khrebet";
			};
			class Hill_Bashnya: Hill_Bashnya
			{
				name = "Bashnya";
			};
			class Hill_Veresnik: Hill_Veresnik
			{
				name = "Veresnik";
			};
			class Hill_Kurgan: Hill_Kurgan
			{
				name = "Kurgan";
			};
			class Hill_Kustryk: Hill_Kustryk
			{
				name = "Kustryk";
			};
			class Hill_Vetryanayagora: Hill_Vetryanayagora
			{
				name = "Vetryanaya gora";
			};
			class Hill_Kalmyk: Hill_Kalmyk
			{
				name = "Kalmyk";
			};
			class Hill_PopIvan: Hill_PopIvan
			{
				name = "Pop Ivan";
			};
			class Hill_Erbenka: Hill_Erbenka
			{
				name = "Erbenka";
			};
			class Local_Lesnoykhrebet: Local_Lesnoykhrebet
			{
				name = "Lesnoy khrebet";
			};
			class Hill_Vysokiykamen: Hill_Vysokiykamen
			{
				name = "Vysokiy kamen";
			};
			class Hill_Dobry: Hill_Dobry
			{
				name = "Dobry";
			};
			class Hill_Baranchik: Hill_Baranchik
			{
				name = "Baranchik";
			};
			class Hill_Malinovka: Hill_Malinovka
			{
				name = "Malinovka";
			};
			class Hill_Dubina: Hill_Dubina
			{
				name = "Dubina";
			};
			class Hill_Klen: Hill_Klen
			{
				name = "Klen";
			};
			class Hill_Chernayagora: Hill_Chernayagora
			{
				name = "Chernaya gora";
			};
			class Ruin_Zolotar: Ruin_Zolotar
			{
				name = "Zolotar";
			};
			class Hill_Ostry: Hill_Ostry
			{
				name = "Ostry";
			};
			class Hill_Olsha: Hill_Olsha
			{
				name = "Olsha";
			};
			class Marine_Tikhiyzaliv: Marine_Tikhiyzaliv
			{
				name = "Tikhiy zaliv";
			};
			class Marine_Mutnyizaliv: Marine_Mutnyizaliv
			{
				name = "Tikhiy zaliv";
			};
			class Marine_Chernyzaliv: Marine_Chernyzaliv
			{
				name = "Cherny zaliv";
			};
			class Marine_Zelenyzaliv: Marine_Zelenyzaliv
			{
				name = "Zeleny zaliv";
			};
			class Marine_Skalistyproliv: Marine_Skalistyproliv
			{
				name = "Skalisty proliv";
			};
			class Marine_Nizhniyzaliv: Marine_Nizhniyzaliv
			{
				name = "Nizhniy zaliv";
			};
			class Marine_ZalivGuba: Marine_ZalivGuba
			{
				name = "Zaliv Guba";
			};
			class Marine_Rify: Marine_Rify
			{
				name = "Rify";
			};
			class Marine_Ivovoyeozero: Marine_Ivovoyeozero
			{
				name = "Ivovoye ozero";
			};
			class Marine_Chernoyeozero: Marine_Chernoyeozero
			{
				name = "Chernoye ozero";
			};
			class Marine_PlotinaTopolka: Marine_PlotinaTopolka
			{
				name = "Plotina Topolka";
			};
			class Marine_PlotinaPobeda: Marine_PlotinaPobeda
			{
				name = "Plotina Pobeda";
			};
			class Marine_PlotinaTishina: Marine_PlotinaTishina
			{
				name = "Plotina Tishina";
			};
			class Marine_Ozerko: Marine_Ozerko
			{
				name = "Ozerko";
			};
			class Marine_Prud: Marine_Prud
			{
				name = "Prud";
			};
			class Ruin_Chortovzamok: Ruin_Chortovzamok
			{
				name = "Chortov zamok";
			};
			class Ruin_Zub: Ruin_Zub
			{
				name = "Zub";
			};
			class Ruin_Rog: Ruin_Rog
			{
				name = "Rog";
			};
			class Local_Grozovypereval: Local_Grozovypereval
			{
				name = "Grozovy pereval";
			};
			class Local_Sosnovypereval: Local_Sosnovypereval
			{
				name = "Sosnovy pereval";
			};
			class Local_PerevalOreshka: Local_PerevalOreshka
			{
				name = "Oreshka pereval";
			};
			class Local_Turovskiypereval: Local_Turovskiypereval
			{
				name = "Turovskiy pereval";
			};
			class Local_Tridoliny: Local_Tridoliny
			{
				name = "Tri doliny";
			};
			class Local_Grozovyedoly: Local_Grozovyedoly
			{
				name = "Grozovye doly";
			};
			class Ruin_Klyuch: Ruin_Klyuch
			{
				name = "Klyuch";
			};
			class Hill_Lysayagora: Hill_Lysayagora
			{
				name = "Lysaya gora";
			};
			class Marine_Glubokoyeozero: Marine_Glubokoyeozero
			{
				name = "Glubokoye ozero";
			};
			class Local_Skalka: Local_Skalka
			{
				name = "Skalka";
			};
			class Local_Vidy: Local_Vidy
			{
				name = "Vidy";
			};
			class Hill_Tumannyverkh: Hill_Tumannyverkh
			{
				name = "Tumanny verkh";
			};
			class Local_Adamovka: Local_Adamovka
			{
				name = "Adamovka";
			};
			class Hill_Shishok: Hill_Shishok
			{
				name = "Shishok";
			};
			class Settlement_Skalisty: Settlement_Skalisty
			{
				name = "Skalisty";
			};
			class Ruin_Storozh: Ruin_Storozh
			{
				name = "Storozh";
			};
			class Local_Drakon: Local_Drakon
			{
				name = "Drakon";
			};
			class Local_Otmel: Local_Otmel
			{
				name = "Otmel";
			};
			class Local_MysGolova: Local_MysGolova
			{
				name = "Mys Golova";
			};
			class Local_MysKrutoy: Local_MysKrutoy
			{
				name = "Mys Krutoy";
			};
			class Hill_Tokarnya: Hill_Tokarnya
			{
				name = "Tokarnya";
			};
			class Hill_Ostrog: Hill_Ostrog
			{
				name = "Ostrog";
			};
			class Local_Maryanka: Local_Maryanka
			{
				name = "Maryanka";
			};
			class Local_Polonina: Local_Polonina
			{
				name = "Polonina";
			};
			class Local_Kalinka: Local_Kalinka
			{
				name = "Kalinka";
			};
			class Hill_Kikimora: Hill_Kikimora
			{
				name = "Kikimora";
			};
			class Hill_BolshoyKotel: Hill_BolshoyKotel
			{
				name = "Bolshoy Kotel";
			};
			class Hill_Simurg: Hill_Simurg
			{
				name = "Simurg";
			};
			class Hill_Volchiypik: Hill_Volchiypik
			{
				name = "Volchiy pik";
			};
			class Ruin_Krona: Ruin_Krona
			{
				name = "Krona";
			};
			class Local_TriKresta: Local_TriKresta
			{
				name = "Tri Kresta";
			};
			class Local_Rostoki: Local_Rostoki
			{
				name = "Rostoki";
			};
			class Marine_OrlyeOzero: Marine_OrlyeOzero
			{
				name = "Orlye Ozero";
			};
			class Local_Makosh: Local_Makosh
			{
				name = "Makosh";
			};
			class Local_Klenovyipereval: Local_Klenovyipereval
			{
				name = "Klenovyi pereval";
			};
			class Local_Zmeinykhrebet: Local_Zmeinykhrebet
			{
				name = "Zmeiny khrebet";
			};
			class Hill_Sokol: Hill_Sokol
			{
				name = "Sokol";
			};
			class Local_Krutyeskaly: Local_Krutyeskaly
			{
				name = "Krutye skaly";
			};
			class Local_Bogat: Local_Bogat
			{
				name = "Bogat";
			};
			class Local_Dubnik: Local_Dubnik
			{
				name = "Dubnik";
			};
			class Hill_Viselitsa: Hill_Viselitsa
			{
				name = "Viselitsa";
			};
			class Local_Dazhbog: Local_Dazhbog
			{
				name = "Dazhbog";
			};
			class Marine_Verbnik: Marine_Verbnik
			{
				name = "Verbnik";
			};
			class Local_Medvezhilugi: Local_Medvezhilugi
			{
				name = "Medvezhi lugi";
			};
			class Ruin_Voron: Ruin_Voron
			{
				name = "Voron";
			};
			class Ruin_Gnomovzamok: Ruin_Gnomovzamok
			{
				name = "Gnomov zamok";
			};
			class Marine_Glaza: Marine_Glaza
			{
				name = "Glaza";
			};
			class Local_KarerKrasnayaZarya: Local_KarerKrasnayaZarya
			{
				name = "Karer Krasnaya Zarya";
			};
			class Local_Matveyevo: Local_Matveyevo
			{
				name = "Matveyevo";
			};
			class Local_Kotka: Local_Kotka
			{
				name = "Kotka";
			};
			class Local_Chernyeskaly: Local_Chernyeskaly
			{
				name = "Chernye skaly";
			};
			class Hill_SvyatoyRoman: Hill_SvyatoyRoman
			{
				name = "Svyatoy roman";
			};
			class Hill_Koman: Hill_Koman
			{
				name = "Koman";
			};
			class Hill_Mayak: Hill_Mayak
			{
				name = "Mayak";
			};
			class Local_MB_VMC: Local_MB_VMC
			{
				name = "Military base VMC";
			};
			class Local_MB_Tisy: Local_MB_Tisy
			{
				name = "Military base Tisy";
			};
			class Local_MB_Kamensk: Local_MB_Kamensk
			{
				name = "Military base Kamensk";
			};
			class Local_MB_Zeleno: Local_MB_Zeleno
			{
				name = "Military base Zeleno";
			};
			class Local_MB_Pavlovo: Local_MB_Pavlovo
			{
				name = "Military base Pavlovo";
			};
			class Local_AF_Balota: Local_AF_Balota
			{
				name = "Airfield balota";
			};
			class Local_AF_Krasno: Local_AF_Krasno
			{
				name = "Airfield Krasno";
			};
			class Local_AF_Vybor: Local_AF_Vybor
			{
				name = "Airfield Vybor";
			};
		};
	};
};

class RscMapControl
{	
	x = 0.10;
	y = 0.10;
	w = 0.80;
	h = 0.60;
	
	colorBackground[] = { 1.00, 1.00, 1.00, 1.00 };
	colorText[] = { 1.00, 1.00, 1.00, 1.00 };
	colorSea[] = { 0.56, 0.80, 0.98, 0.50 };
	colorForest[] = { 0.60, 0.80, 0.20, 0.50 };
	colorRocks[] = { 0.50, 0.50, 0.50, 0.50 };
	colorTracks[]={0.77,0.66,0.34,1.00};
	colorRoads[]={0.69,0.43,0.23,1.00};
	colorMainRoads[]={0.52,0.34,0.00,1.00};
	colorTracksFill[]={0.95,0.91,0.60,1.00};
	colorRoadsFill[]={0.92,0.73,0.41,1.00};
	colorMainRoadsFill[]={0.83,0.61,0.20,1.00};
	colorCountlines[] = { 0.65, 0.45, 0.27, 0.50 };
	colorMainCountlines[] = { 0.65, 0.45, 0.27, 1.00 };
	colorCountlinesWater[] = { 0.00, 0.53, 1.00, 0.50 };
	colorMainCountlinesWater[] = { 0.00, 0.53, 1.00, 1.00 };
	colorForestBorder[] = { 0.40, 0.80, 0.00, 1.00 };
	colorRocksBorder[] = { 0.50, 0.50, 0.50, 1.00 };
	colorPowerLines[] = { 0.00, 0.00, 0.00, 1.00 };
	colorRailWay[]={0.80,0.20,0.00,1.00};
	colorNames[] = { 1.00, 1.00, 1.00, 1.00 };
	colorInactive[] = { 1.00, 1.00, 1.00, 0.50 };
	colorLevels[] = { 0.00, 0.00, 0.00, 1.00 };
	colorGrid[]={0.90,0.90,0.90,0.80};
	colorGridMap[]={0.90,0.90,0.90,0.20};
	colorOutside[]={1.00,1.00,1.00,1.00};
	
	font = "gui/fonts/etelkatextpro";
	sizeEx = 0.040000;
	fontLabel = "gui/fonts/etelkatextpro";
	sizeExLabel = 0.02;
	fontGrid = "gui/fonts/etelkatextpro";
	sizeExGrid = 0.02;
	fontUnits = "gui/fonts/etelkatextpro";
	sizeExUnits = 0.02;
	fontNames = "gui/fonts/etelkatextpro";
	sizeExNames = 0.02;
	fontInfo = "gui/fonts/etelkatextpro";
	sizeExInfo = 0.02;
	fontLevel = "gui/fonts/etelkatextpro";
	sizeExLevel = 0.02;
	
	ptsPerSquareSea = 6;
	ptsPerSquareTxt = 8;
	ptsPerSquareCLn = 8;
	ptsPerSquareExp = 8;
	ptsPerSquareCost = 8;
	ptsPerSquareFor = "4.0f";
	ptsPerSquareForEdge = "10.0f";
	ptsPerSquareRoad = 2;
	ptsPerSquareObj = 10;
	
	maxSatelliteAlpha=1;
	alphaFadeStartScale=1;
	alphaFadeEndScale=1;
	userMapPath="dz\gear\navigation\data\usermap";
	maxUserMapAlpha=0.2;
	alphaUserMapFadeStartScale=0.5;
	alphaUserMapFadeEndScale=0.80000000;
	
	showCountourInterval=2;
	scaleMin=0.050000000;
	scaleMax=0.80000000;
	scaleDefault=0.80000000;
	
	class Legend
	{
		x=0.050000001;
		y=0.85000002;
		w=0.40000001;
		h=0.1;
		font="gui/fonts/etelkatextpro22";
		sizeEx=0.02;
		colorBackground[]={1,1,1,0};
		color[]={0,0,0,0};
	};
	class BusStop
	{
		icon="\DZ\gear\navigation\data\map_busstop_ca.paa";
		color[]={1,1,1,0};
		size=16;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Bush
	{
		icon="\DZ\gear\navigation\data\map_bush_ca.paa";
		color[]={0.40000001,0.80000001,0,0};
		size=14;
		importance="0.2 * 14 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Bunker
	{
		icon="\DZ\gear\navigation\data\map_bunker_ca.paa";
		color[]={0.60000002,0.60000002,0.60000002,1};
		size=20;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Chapel
	{
		icon="\dz\gear\navigation\data\map_chapel_ca.paa";
		color[]={0,0,0,1};
		size=16;
		importance="1 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Church
	{
		icon="\dz\gear\navigation\data\map_church_ca.paa";
		color[]={0,0,0,1};
		size=10;
		importance="2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Cross
	{
		icon="\DZ\gear\navigation\data\map_cross_ca.paa";
		size=20;
		color[]={1,1,0,0};
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Fortress
	{
		icon="\DZ\gear\navigation\data\map_fortress_ca.paa";
		size=20;
		color[]={0.60000002,0.60000002,0.60000002,1};
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4.00;
	};
	class Fountain
	{
		icon="\DZ\gear\navigation\data\map_fountain_ca.paa";
		color[]={0.00, 0.35, 0.70, 1.00};
		size=20;
		importance="1 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4.00;
	};
		class Fuelstation
	{
		icon="\dz\gear\navigation\data\map_fuelstation_ca.paa";
		size=16;
		color[]={0,0,0,1};
		importance="2 * 16 * 0.05";
		coefMin=0.75;
		coefMax=4;
	};
	class FireDep
	{
		icon="\dz\gear\navigation\data\map_firedep_ca.paa";
		color[]={0.764,0,0,1};
		size=20;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Monument
	{
		icon="\DZ\gear\navigation\data\map_monument_ca.paa";
		color[]={1,1,1,0};
		size=20;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Quay
	{
		icon="\DZ\gear\navigation\data\map_quay_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="0.9 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Rocks
	{
		icon="\DZ\gear\navigation\data\map_rock_ca.paa";
		color[]={0.60000002,0.60000002,0.60000002,1};
		size=12;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Ruin
	{
		icon="\DZ\gear\navigation\data\map_ruin_ca.paa";
		size=16;
		color[]={0.60000002,0.60000002,0.60000002,1};
		importance="1.2 * 16 * 0.05";
		coefMin=1;
		coefMax=4;
	};
	class Rock
	{
		icon="\DZ\gear\navigation\data\map_rock_ca.paa";
		color[]={0.1,0.1,0.1,0.80000001};
		size=12;
		importance="0.5 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class SmallTree
	{
		
		icon="DayZExpansion\GUI\icons\map\icon_tree_ca.paa";
		color[]={0,0.58039215,0.19607843,0.80000001};
		size=12;
		importance="0.6 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Stack
	{
		icon="\DZ\gear\navigation\data\map_stack_ca.paa";
		size=16;
		color[]={1,1,1,0};
		importance="0.9 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Shipwreck
	{
		icon="\DZ\gear\navigation\data\map_shipwreck_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="0.9 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Store
	{
		icon="\dz\gear\navigation\data\map_store_ca.paa";
		color[]={0,0,0,1};
		size=20;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class TouristSign
	{
		icon="\DZ\gear\navigation\data\map_tsign_ca.paa";
		color[]={1,1,1,0};
		size=15;
		importance="0.9 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class TouristShelter
	{
		icon="\DZ\gear\navigation\data\map_tshelter_ca.paa";
		color[]={1,1,1,1};
		size=18;
		importance="0.9 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Tree
	{
		icon="DayZExpansion\GUI\icons\map\icon_tree_ca.paa";
		color[]={0,0.58039215,0.19607843,0.80000001};
		size=12;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Transmitter
	{
		icon="\dz\gear\navigation\data\map_transmitter_ca.paa";
		color[]={0,0,0,1};
		size=16;
		importance="2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Tourism
	{
		icon="\dz\gear\navigation\data\map_tourism_ca.paa";
		color[]={0,0,0,1};
		size=14;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Lighthouse
	{
		icon="DayZExpansion\GUI\icons\map\icon_lighthouse_ca.paa";
		color[]={0.91764705,0.12549019,0.15294117,0.80000001};
		size=20;
		importance="0.9 * 16 * 0.05";
		coefMin=1;
		coefMax=4;
	};
	class ViewTower
	{
		icon="\dz\gear\navigation\data\map_viewtower_ca.paa";
		color[]={0,0,0,1};
		size=16;
		importance="2.5 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Waterpump
	{
		icon="\DZ\gear\navigation\data\map_waterpump_ca.paa";
		color[]={0.00, 0.35, 0.70, 1.00};
		size=20;
		importance="1 * 12 * 0.05";
		coefMin=0.50;
		coefMax=4.00;
	};
	class Watertower
	{
		icon="DayZExpansion\GUI\icons\map\icon_watertower_ca.paa";
		color[]={0.00, 0.35, 0.70, 1.00};
		size=20;
		importance="1 * 12 * 0.05";
		coefMin=0.50;
		coefMax=4.00;
	};
};

class CfgLocationTypes
{
	class Name
	{
		name="keypoint";
		drawStyle="name";
		texture="#(argb,1,1,1)color(1,1,1,1)";
		color[]={1,1,1,1};
		size=0;
		font="gui/fonts/etelkatextpro22";
		textSize=0.030000000;
		shadow=0.1;
		importance=1;
	};
	class NameIcon
	{
		name="keypoint";
		drawStyle="icon";
		texture="#(argb,1,1,1)color(1,1,1,1)";
		color[]={1,1,1,1};
		size=0;
		font="gui/fonts/etelkatextpro22";
		textSize=0.030000000;
		shadow=0;
		importance=1;
	};
	class Capital: Name
	{
		color[]={1,1,1,1};
		textSize=0.060000000;
		importance=7;
	};
	class BigMilitary: Name
	{
		color[]={1,0.1,0.1,1};
		textSize=0.050000000;
		importance=6;
	};
	class City: Name
	{
		textSize=0.050000001;
		importance=6;
	};
	class MediumMilitary: Name
	{
		color[]={1,0.1,0.1,1};
		textSize=0.040000000;
		importance=5;
	};
	class SmallMilitary: Name
	{
		color[]={1,0.1,0.1,1};
		textSize=0.035;
		importance=5;
	};
	class Village: Name
	{
		textSize=0.040000000;
		importance=3;
	};
	class Local: Name
	{
		color[]={0.44,0.38,0.30000001,1};
		textSize=0.030000000;
	};
	class Marine: Name
	{
		color[]={0.050000001,0.40000001,0.80000001,0.80000001};
		textSize=0.030000000;
	};
	/*class FeedShack: NameIcon
	{
		texture="";
		importance=1;
		size=10;
	};
	class Deerstand: NameIcon
	{
		texture="";
		importance=1;
		size=10;
	};
	class Ruin: NameIcon
	{
		texture="\DZ\gear\navigation\data\map_ruin_ca.paa";
		textSize=0.029999999;
		importance=2;
		size=7;
	};
	class Camp: NameIcon
	{
		color[]={0.44,0.38,0.30000001,1};
		texture="\DZ\gear\navigation\data\map_camp_ca.paa";
		textSize=0.02;
		size=5;
	};
	class Hill: NameIcon
	{
		color[]={0.69999999,0.69999999,0.69999999,1};
		texture="\DZ\gear\navigation\data\map_hill_ca.paa";
		textSize=0.02;
		size=3;
	};
	class ViewPoint: NameIcon
	{
		color[]={0.77999997,0,0.050000001,0};
		texture="\DZ\gear\navigation\data\map_viewpoint_ca.paa";
		size=9;
	};
	class RockArea: NameIcon
	{
		color[]={0,0,0,1};
		texture="\DZ\gear\navigation\data\map_rock_ca.paa";
		size=9;
	};
	class RailroadStation: NameIcon
	{
		color[]={0,0,0,0};
		texture="\DZ\gear\navigation\data\map_station_ca.paa";
		size=9;
	};
	class IndustrialSite: NameIcon
	{
		color[]={0,0,0,1};
		texture="\DZ\gear\navigation\data\map_factory_ca.paa";
		size=9;
	};
	class LocalOffice: NameIcon
	{
		color[]={0.22,0.20999999,0.50999999,0};
		texture="\DZ\gear\navigation\data\map_govoffice_ca.paa";
		size=10;
	};
	class BorderCrossing: NameIcon
	{
		color[]={0.77999997,0,0.050000001,1};
		texture="\DZ\gear\navigation\data\map_border_cross_ca.paa";
		size=9;
	};
	class VegetationBroadleaf: NameIcon
	{
		color[]={0.25,0.40000001,0.2,1};
		texture="\DZ\gear\navigation\data\map_broadleaf_ca.paa";
		size=9;
	};
	class VegetationFir: NameIcon
	{
		color[]={0.25,0.40000001,0.2,1};
		texture="\DZ\gear\navigation\data\map_fir_ca.paa";
		size=9;
	};
	class VegetationPalm: NameIcon
	{
		color[]={0.25,0.40000001,0.2,1};
		texture="\DZ\gear\navigation\data\map_palm_ca.paa";
		size=9;
	};
	class VegetationVineyard: NameIcon
	{
		color[]={0.25,0.40000001,0.2,1};
		texture="\DZ\gear\navigation\data\map_vineyard_ca.paa";
		size=9;
	};*/
};