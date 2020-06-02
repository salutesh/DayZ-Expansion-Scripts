/**
 * ExpansionServerInfos.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionServerInfos
 * @brief
 **/
class ExpansionServerInfos
{
    string serverName;
	string infoTitle1;
	string infoContent1;
	string infoTitle2;
	string infoContent2;
	string infoTitle3;
	string infoContent3;

	// ------------------------------------------------------------
	// Expansion ExpansionServerDescription
	// ------------------------------------------------------------
    void ExpansionServerInfos( string ServerName = "", string Title1 = "", string Content1 = "", string Title2 = "", string Content2 = "", string Title3 = "", string Content3 = "")
	{
        this.serverName = ServerName;
        this.infoTitle1 = Title1;
		this.infoContent1 = Content1;
		this.infoTitle2 = Title2;
		this.infoContent2 = Content2;
		this.infoTitle3 = Title3;
		this.infoContent3 = Content3;
    }
}