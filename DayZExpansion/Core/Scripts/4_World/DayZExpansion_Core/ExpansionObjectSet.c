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
			GetGame().ObjectDelete(obj);
		}

		m_Objects.Clear();
	}
	
	void SpawnObjects()
	{
		string filePath = m_FilePath + m_FileName + ".map";
		if (!FileExist(filePath))
			return;
		
		ExpansionObjectSpawnTools.LoadObjectsFile(filePath, m_Objects);
	}
};
