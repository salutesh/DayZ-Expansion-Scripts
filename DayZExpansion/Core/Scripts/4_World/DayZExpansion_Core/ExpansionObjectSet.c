/**
 * ExpansionObjectSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionObjectSet
{
	private string m_FileName;
	private string m_FilePath;
	//! All objects from the loaded file that got created
	private ref array<Object> m_Objects = new array<Object>;
	
	void ExpansionObjectSet(string filePath, string fileName)
	{
		m_FilePath = filePath;
		m_FileName = fileName;
	}
	
	void ~ExpansionObjectSet()
	{
		Delete();
	}
	
	void Delete()
	{
		foreach (Object obj: m_Objects)
		{
			g_Game.ObjectDelete(obj);
		}

		m_Objects.Clear();
	}
	
	void SpawnObjects()
	{
		string filePath = GetFilePath();

		if (!FileExist(filePath))
			return;
		
		ExpansionObjectSpawnTools.LoadObjectsFile(filePath, m_Objects);
	}

	string GetFilePath()
	{
		int trimSize = m_FileName.Length() - EXPANSION_MAPPING_EXT.Length();
		string fileformat = EXPANSION_MAPPING_EXT;

		if( m_FileName.IndexOf(EXPANSION_MAPPING_EXT) == trimSize )
			fileformat = "";
		
		return m_FilePath + m_FileName + fileformat;
	}
};
