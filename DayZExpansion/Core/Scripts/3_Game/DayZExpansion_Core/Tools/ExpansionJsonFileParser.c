/**
 * ExpansionJsonFileParser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! TODO: Use JsonApiStruct for a much more flexible JSON parser
class ExpansionJsonFileParser<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;

	static bool Load(string filename, out T data)
	{
		if (!FileExist(filename))
			return false;

		FileHandle handle = OpenFile(filename, FileMode.READ);
		if (!handle)
			return false;

		string file_content;
		string line_content;

		while (FGets(handle, line_content) >= 0)
		{
			file_content += line_content;
		}

		CloseFile(handle);

		return ReadFromString(file_content, data);
	}

	static void Save(string filename, T data)
	{
		FileHandle handle = OpenFile(filename, FileMode.WRITE);
		if (!handle)
			return;

		FPrint(handle, AsString(data));

		CloseFile(handle);
	}

	static bool ReadFromString(string string_data, out T data)
	{
		string error;
		if (m_Serializer.ReadFromString(data, string_data, error))
			return true;

		Error(error);

		return false;
	}

	static string AsString(T data)
	{
		string string_data;
		m_Serializer.WriteToString(data, true, string_data);
		return string_data;
	}
}
