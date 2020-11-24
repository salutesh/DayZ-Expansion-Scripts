#define _ARMA_

class CfgPatches
{
	class BuilderItems_StaticItems_Books
	{
		units[] = {"bldr_BookBible","bldr_BookTheWarOfTheWorlds","bldr_BookAroundTheWorldIn80Days","bldr_BookCrimeAndPunishment","bldr_BookTheMetamorphosis","bldr_BookTheRaven","bldr_BookTheArtOfWar","bldr_BookRobinsonCrusoe","bldr_BookRussian","bldr_BookRussianCheatSheet","bldr_BookThePictureOfDorianGray","bldr_BookThePitAndThePendulum","bldr_BookTheTimeMachine","bldr_BookTheCountryOfTheBlind","bldr_BookBilaNemoc","bldr_BookYouth","bldr_BookVerwandlung","bldr_BookTheJungleBook","bldr_BookTheBrothersKaramazov","bldr_BookTheCallOfCthulhu","bldr_BookTheShunnedHouse","bldr_BookTheHoundOfTheBaskervilles","bldr_BookAndersen","bldr_BookTonyAndTheBeetles","bldr_BookTheCosmicComputer","bldr_BookATravelerInTime","bldr_BookTheVariableMan","bldr_BookTheOutlet","bldr_BookTheBarrier","bldr_BookTheDesertOfWheat","bldr_BookFlyingURanch","bldr_BookCassidysRustlerRoundUp","bldr_BookLonesomeLand","bldr_BookTrailin","bldr_BookRonickyDoone","bldr_BookBettyZane","bldr_BookATexasMatchmaker","bldr_BookTheThunderBird","bldr_BookBlackJack","bldr_BookTheLastTrail","bldr_BookSelectedStories","bldr_BookTheHeritageOfTheSioux","bldr_BookDesertGold","bldr_BookTheHiddenChildren","bldr_BookTheLastOfThePlainsmen","bldr_BookGunmansReckoning","bldr_BookSkyrider","bldr_BookSpinifexAndSand","bldr_BookSunsetPass","bldr_BookTheCaveOfGold","bldr_BookTheDayOfTheBeast","bldr_BookTheQuirt","bldr_BookAVoyageToArcturus","bldr_BookWealthOfNations","bldr_BookAndersensFairyTales","bldr_BookBlackBeauty","bldr_BookBuddenbrooks","bldr_BookDeadSouls","bldr_BookErewhon","bldr_BookFatherGoriot","bldr_BookGreatExpectations","bldr_BookGulliversTravels","bldr_BookHeartOfDarkness","bldr_BookHerland","bldr_BookIvanhoe","bldr_BookKidnapped","bldr_BookLaChartreuseDeParme","bldr_BookLordJim","bldr_BookMadameBovary","bldr_BookNostromo","bldr_BookNaturalSelection","bldr_BookTheMetamorphosesOfOvid","bldr_BookMobyDick","bldr_BookKingLear","bldr_BookLaDbcle","bldr_BookTheCanterburyTales","bldr_BookTheRightsOfWoman","bldr_BookToTheInteriorOfTheEarth","bldr_BookNjalsSaga","bldr_BookTheTrial","bldr_BookDonQuixote","bldr_BookPhilosophiaeNaturalis","bldr_BookPrideAndPrejudice","bldr_BookRasselas","bldr_BookScaramouche","bldr_BookShe","bldr_BookSonsAndLovers","bldr_BookTheCallOfTheWild","bldr_BookTheComingRace","bldr_BookRobinsonCrusoeAdventures","bldr_BookTheIslandOfDoctorMoreau","bldr_BookTheMahabharataOfKrishna","bldr_BookTheManWhoWasThursday","bldr_BookThePossessed","bldr_BookThePrisonerOfZenda","bldr_BookAPrincessOfMars","bldr_BookKingSolomonsMines","bldr_BookMiddlemarch","bldr_BookTheHouseOfTheSevenGables","bldr_BookTheMonkARomance","bldr_BookTheJustifiedSinner","bldr_BookTheRedBadgeOfCourage","bldr_BookADollsHouse","bldr_BookAfterLondon","bldr_BookGilgameshEpic","bldr_BookAnnaKarenina","bldr_BookCandide","bldr_BookFaust","bldr_BookHinduLiterature","bldr_BookHunger","bldr_BookJapaneseLiterature","bldr_BookLeRougeEtLeNoir","bldr_BookTheDecameron","bldr_BookTheGiacomoLeopardi","bldr_BookAliceInWonderland","bldr_BookTheScarletPimpernel","bldr_BookTheThreeMusketeers","bldr_BookTheTurnOfTheScrew","bldr_BookTranslationsOfShakuntala","bldr_BookTreasureIsland","bldr_BookUlysses","bldr_BookUncleSilas","bldr_BookTheAeneid","bldr_BookWarAndPeace","bldr_BookWielandOrTheTransformation","bldr_BookWutheringHeights","bldr_BookCinqSemainesEnBallon","bldr_BookDracula","bldr_BookMrchenFrKinder","bldr_BookLesTroisMousquetaires","bldr_BookDeLorigineDesEspces","bldr_BookDasNibelungenlied","bldr_BookTheLastMan","bldr_BookFaustErsterTeil","bldr_BookLenfer1Of2","bldr_BookVoyageAuCentreDeLaTerre","bldr_BookRomeoUndJulia","bldr_BookFrankenstein","bldr_BookDeLaTerreLaLune","bldr_BookDonQuichotte","bldr_BookLleMystrieuse","bldr_BookLaComdieHumaine","bldr_BookLesCorneilles","bldr_Book20000LieuesSousLesMers","bldr_BookThreeMenInABoat"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_BookBible: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_bible_CO.paa"};
	};
	class bldr_BookTheWarOfTheWorlds: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheWarOfTheWorlds_co.paa"};
	};
	class bldr_BookAroundTheWorldIn80Days: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AroundTheWorldIn80Days_co.paa"};
	};
	class bldr_BookCrimeAndPunishment: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_CrimeAndPunishment_co.paa"};
	};
	class bldr_BookTheMetamorphosis: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheMetamorphosis_co.paa"};
	};
	class bldr_BookTheRaven: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheRaven_co.paa"};
	};
	class bldr_BookTheArtOfWar: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheArtOfWar_co.paa"};
	};
	class bldr_BookRobinsonCrusoe: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_RobinsonCrusoe_co.paa"};
	};
	class bldr_BookRussian: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Russian_co.paa"};
	};
	class bldr_BookRussianCheatSheet: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_RussianCheatSheet_co.paa"};
	};
	class bldr_BookThePictureOfDorianGray: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ThePictureOfDorianGray_co.paa"};
	};
	class bldr_BookThePitAndThePendulum: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ThePitAndThePendulum_co.paa"};
	};
	class bldr_BookTheTimeMachine: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheTimeMachine_co.paa"};
	};
	class bldr_BookTheCountryOfTheBlind: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCountryOfTheBlind_co.paa"};
	};
	class bldr_BookBilaNemoc: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_BilaNemoc_co.paa"};
	};
	class bldr_BookYouth: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Youth_co.paa"};
	};
	class bldr_BookVerwandlung: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Verwandlung_co.paa"};
	};
	class bldr_BookTheJungleBook: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheJungleBook_co.paa"};
	};
	class bldr_BookTheBrothersKaramazov: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheBrothersKaramazov_co.paa"};
	};
	class bldr_BookTheCallOfCthulhu: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCallOfCthulhu_co.paa"};
	};
	class bldr_BookTheShunnedHouse: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheShunnedHouse_co.paa"};
	};
	class bldr_BookTheHoundOfTheBaskervilles: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheHoundOfTheBaskervilles_co.paa"};
	};
	class bldr_BookAndersen: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Andersen_co.paa"};
	};
	class bldr_BookTonyAndTheBeetles: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TonyAndTheBeetles_co.paa"};
	};
	class bldr_BookTheCosmicComputer: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCosmicComputer_co.paa"};
	};
	class bldr_BookATravelerInTime: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ATravelerInTime_co.paa"};
	};
	class bldr_BookTheVariableMan: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheVariableMan_co.paa"};
	};
	class bldr_BookTheOutlet: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheOutlet_co.paa"};
	};
	class bldr_BookTheBarrier: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheBarrier_co.paa"};
	};
	class bldr_BookTheDesertOfWheat: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheDesertOfWheat_co.paa"};
	};
	class bldr_BookFlyingURanch: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_FlyingURanch_co.paa"};
	};
	class bldr_BookCassidysRustlerRoundUp: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_CassidysRustlerRoundUp_co.paa"};
	};
	class bldr_BookLonesomeLand: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LonesomeLand_co.paa"};
	};
	class bldr_BookTrailin: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Trailin_co.paa"};
	};
	class bldr_BookRonickyDoone: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_RonickyDoone_co.paa"};
	};
	class bldr_BookBettyZane: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_BettyZane_co.paa"};
	};
	class bldr_BookATexasMatchmaker: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ATexasMatchmaker_co.paa"};
	};
	class bldr_BookTheThunderBird: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheThunderBird_co.paa"};
	};
	class bldr_BookBlackJack: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_BlackJack_co.paa"};
	};
	class bldr_BookTheLastTrail: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheLastTrail_co.paa"};
	};
	class bldr_BookSelectedStories: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_SelectedStories_co.paa"};
	};
	class bldr_BookTheHeritageOfTheSioux: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheHeritageOfTheSioux_co.paa"};
	};
	class bldr_BookDesertGold: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DesertGold_co.paa"};
	};
	class bldr_BookTheHiddenChildren: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheHiddenChildren_co.paa"};
	};
	class bldr_BookTheLastOfThePlainsmen: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheLastOfThePlainsmen_co.paa"};
	};
	class bldr_BookGunmansReckoning: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_GunmansReckoning_co.paa"};
	};
	class bldr_BookSkyrider: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Skyrider_co.paa"};
	};
	class bldr_BookSpinifexAndSand: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_SpinifexAndSand_co.paa"};
	};
	class bldr_BookSunsetPass: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_SunsetPass_co.paa"};
	};
	class bldr_BookTheCaveOfGold: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCaveOfGold_co.paa"};
	};
	class bldr_BookTheDayOfTheBeast: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheDayOfTheBeast_co.paa"};
	};
	class bldr_BookTheQuirt: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheQuirt_co.paa"};
	};
	class bldr_BookAVoyageToArcturus: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AVoyageToArcturus_co.paa"};
	};
	class bldr_BookWealthOfNations: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_WealthOfNations_co.paa"};
	};
	class bldr_BookAndersensFairyTales: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AndersensFairyTales_co.paa"};
	};
	class bldr_BookBlackBeauty: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_BlackBeauty_co.paa"};
	};
	class bldr_BookBuddenbrooks: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Buddenbrooks_co.paa"};
	};
	class bldr_BookDeadSouls: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DeadSouls_co.paa"};
	};
	class bldr_BookErewhon: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Erewhon_co.paa"};
	};
	class bldr_BookFatherGoriot: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_FatherGoriot_co.paa"};
	};
	class bldr_BookGreatExpectations: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_GreatExpectations_co.paa"};
	};
	class bldr_BookGulliversTravels: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_GulliversTravels_co.paa"};
	};
	class bldr_BookHeartOfDarkness: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_HeartOfDarkness_co.paa"};
	};
	class bldr_BookHerland: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Herland_co.paa"};
	};
	class bldr_BookIvanhoe: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Ivanhoe_co.paa"};
	};
	class bldr_BookKidnapped: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Kidnapped_co.paa"};
	};
	class bldr_BookLaChartreuseDeParme: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LaChartreuseDeParme_co.paa"};
	};
	class bldr_BookLordJim: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LordJim_co.paa"};
	};
	class bldr_BookMadameBovary: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_MadameBovary_co.paa"};
	};
	class bldr_BookNostromo: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Nostromo_co.paa"};
	};
	class bldr_BookNaturalSelection: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_NaturalSelection_co.paa"};
	};
	class bldr_BookTheMetamorphosesOfOvid: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheMetamorphosesOfOvid_co.paa"};
	};
	class bldr_BookMobyDick: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_MobyDick_co.paa"};
	};
	class bldr_BookKingLear: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_KingLear_co.paa"};
	};
	class bldr_BookLaDbcle: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LaDbcle_co.paa"};
	};
	class bldr_BookTheCanterburyTales: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCanterburyTales_co.paa"};
	};
	class bldr_BookTheRightsOfWoman: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheRightsOfWoman_co.paa"};
	};
	class bldr_BookToTheInteriorOfTheEarth: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ToTheInteriorOfTheEarth_co.paa"};
	};
	class bldr_BookNjalsSaga: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_NjalsSaga_co.paa"};
	};
	class bldr_BookTheTrial: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheTrial_co.paa"};
	};
	class bldr_BookDonQuixote: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DonQuixote_co.paa"};
	};
	class bldr_BookPhilosophiaeNaturalis: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_PhilosophiaeNaturalis_co.paa"};
	};
	class bldr_BookPrideAndPrejudice: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_PrideAndPrejudice_co.paa"};
	};
	class bldr_BookRasselas: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Rasselas_co.paa"};
	};
	class bldr_BookScaramouche: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Scaramouche_co.paa"};
	};
	class bldr_BookShe: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_She_co.paa"};
	};
	class bldr_BookSonsAndLovers: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_SonsAndLovers_co.paa"};
	};
	class bldr_BookTheCallOfTheWild: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheCallOfTheWild_co.paa"};
	};
	class bldr_BookTheComingRace: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheComingRace_co.paa"};
	};
	class bldr_BookRobinsonCrusoeAdventures: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_RobinsonCrusoeAdventures_co.paa"};
	};
	class bldr_BookTheIslandOfDoctorMoreau: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheIslandOfDoctorMoreau_co.paa"};
	};
	class bldr_BookTheMahabharataOfKrishna: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheMahabharataOfKrishna_co.paa"};
	};
	class bldr_BookTheManWhoWasThursday: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheManWhoWasThursday_co.paa"};
	};
	class bldr_BookThePossessed: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ThePossessed_co.paa"};
	};
	class bldr_BookThePrisonerOfZenda: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ThePrisonerOfZenda_co.paa"};
	};
	class bldr_BookAPrincessOfMars: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_APrincessOfMars_co.paa"};
	};
	class bldr_BookKingSolomonsMines: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_KingSolomonsMines_co.paa"};
	};
	class bldr_BookMiddlemarch: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Middlemarch_co.paa"};
	};
	class bldr_BookTheHouseOfTheSevenGables: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheHouseOfTheSevenGables_co.paa"};
	};
	class bldr_BookTheMonkARomance: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheMonkARomance_co.paa"};
	};
	class bldr_BookTheJustifiedSinner: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheJustifiedSinner_co.paa"};
	};
	class bldr_BookTheRedBadgeOfCourage: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheRedBadgeOfCourage_co.paa"};
	};
	class bldr_BookADollsHouse: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ADollsHouse_co.paa"};
	};
	class bldr_BookAfterLondon: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AfterLondon_co.paa"};
	};
	class bldr_BookGilgameshEpic: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_GilgameshEpic_co.paa"};
	};
	class bldr_BookAnnaKarenina: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AnnaKarenina_co.paa"};
	};
	class bldr_BookCandide: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Candide_co.paa"};
	};
	class bldr_BookFaust: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Faust_co.paa"};
	};
	class bldr_BookHinduLiterature: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_HinduLiterature_co.paa"};
	};
	class bldr_BookHunger: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Hunger_co.paa"};
	};
	class bldr_BookJapaneseLiterature: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_JapaneseLiterature_co.paa"};
	};
	class bldr_BookLeRougeEtLeNoir: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LeRougeEtLeNoir_co.paa"};
	};
	class bldr_BookTheDecameron: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheDecameron_co.paa"};
	};
	class bldr_BookTheGiacomoLeopardi: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheGiacomoLeopardi_co.paa"};
	};
	class bldr_BookAliceInWonderland: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_AliceInWonderland_co.paa"};
	};
	class bldr_BookTheScarletPimpernel: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheScarletPimpernel_co.paa"};
	};
	class bldr_BookTheThreeMusketeers: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheThreeMusketeers_co.paa"};
	};
	class bldr_BookTheTurnOfTheScrew: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheTurnOfTheScrew_co.paa"};
	};
	class bldr_BookTranslationsOfShakuntala: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TranslationsOfShakuntala_co.paa"};
	};
	class bldr_BookTreasureIsland: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TreasureIsland_co.paa"};
	};
	class bldr_BookUlysses: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Ulysses_co.paa"};
	};
	class bldr_BookUncleSilas: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_UncleSilas_co.paa"};
	};
	class bldr_BookTheAeneid: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheAeneid_co.paa"};
	};
	class bldr_BookWarAndPeace: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_WarAndPeace_co.paa"};
	};
	class bldr_BookWielandOrTheTransformation: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_WielandOrTheTransformation_co.paa"};
	};
	class bldr_BookWutheringHeights: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_WutheringHeights_co.paa"};
	};
	class bldr_BookCinqSemainesEnBallon: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_CinqSemainesEnBallon_co.paa"};
	};
	class bldr_BookDracula: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Dracula_co.paa"};
	};
	class bldr_BookMrchenFrKinder: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_MrchenFrKinder_co.paa"};
	};
	class bldr_BookLesTroisMousquetaires: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LesTroisMousquetaires_co.paa"};
	};
	class bldr_BookDeLorigineDesEspces: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DeLorigineDesEspces_co.paa"};
	};
	class bldr_BookDasNibelungenlied: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DasNibelungenlied_co.paa"};
	};
	class bldr_BookTheLastMan: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_TheLastMan_co.paa"};
	};
	class bldr_BookFaustErsterTeil: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_FaustErsterTeil_co.paa"};
	};
	class bldr_BookLenfer1Of2: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Lenfer1Of2_co.paa"};
	};
	class bldr_BookVoyageAuCentreDeLaTerre: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_VoyageAuCentreDeLaTerre_co.paa"};
	};
	class bldr_BookRomeoUndJulia: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_RomeoUndJulia_co.paa"};
	};
	class bldr_BookFrankenstein: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_Frankenstein_co.paa"};
	};
	class bldr_BookDeLaTerreLaLune: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DeLaTerreLaLune_co.paa"};
	};
	class bldr_BookDonQuichotte: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_DonQuichotte_co.paa"};
	};
	class bldr_BookLleMystrieuse: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LleMystrieuse_co.paa"};
	};
	class bldr_BookLaComdieHumaine: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LaComdieHumaine_co.paa"};
	};
	class bldr_BookLesCorneilles: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_LesCorneilles_co.paa"};
	};
	class bldr_Book20000LieuesSousLesMers: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_20000LieuesSousLesMers_co.paa"};
	};
	class bldr_BookThreeMenInABoat: HouseNoDestruct
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"dz\gear\books\data\book_ThreeMenInABoat_co.paa"};
	};
};
