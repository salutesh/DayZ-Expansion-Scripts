
readme.txt

DayZ Expansion Mod
www.dayzexpansion.com
© 2020 DayZ Expansion Mod Team

This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.

=====================================================================================================================

How to add expansion config to your mission or on a new map

STEP 1.
Your mission must be named "Expansion" before the dot
Exemple : Expansion.ChernarusPlus

STEP 2.
Inside the provided init.c rename "MyWorld" from "Expansion.MyWorld" to the name of your mission (map)
Exemple : Expansion.ChernarusPlus

STEP 3.
Make sure you have inside your mission the "expansion" and "expansion_ce" folder that we have provided

STEP 4.
Inside the file "cfgeconomycore.xml" copy from <ce folder="expansion_ce"> to </ce>
and paste this lines inside your own "cfgeconomycore.xml" of your mission.