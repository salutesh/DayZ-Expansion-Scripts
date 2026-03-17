/**
 * ExpansionCESpawnableTypes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionCESpawnableAttachment
{
	string ItemName;
	float Chance;
	
	void ExpansionCESpawnableAttachment(string itemName, float chance)
	{
		ItemName = itemName;
		Chance = chance;
	}
}

class ExpansionCESpawnableAttachmentGroup
{
	float GroupChance;
	ref array<ref ExpansionCESpawnableAttachment> Items = new array<ref ExpansionCESpawnableAttachment>();

	void ExpansionCESpawnableAttachmentGroup(float groupChance)
	{
		GroupChance = groupChance;
	}

	void AddItem(string itemName, float itemChance)
	{
		Items.Insert(new ExpansionCESpawnableAttachment(itemName, itemChance));
	}
}

class ExpansionCESpawnableType
{
	string Name;
	ref array<ref ExpansionCESpawnableAttachmentGroup> AttachmentGroups = new array<ref ExpansionCESpawnableAttachmentGroup>();

	void ExpansionCESpawnableType(string name)
	{
		Name = name;
	}

	void AddAttachmentGroup(ExpansionCESpawnableAttachmentGroup group)
	{
		AttachmentGroups.Insert(group);
	}

	//! See https://community.bistudio.com/wiki/DayZ:Central_Economy_mission_files_modding for file specifics
	void Merge(ExpansionCESpawnableType other)
	{
		ExpansionArray<ExpansionCESpawnableAttachmentGroup>.RefCopy(other.AttachmentGroups, AttachmentGroups);
	}
}

class ExpansionCESpawnableTypes: map<string, ref ExpansionCESpawnableType>
{
	static ExpansionCESpawnableTypes LoadTypes(string fileName)
	{
		ExpansionCESpawnableTypes types = new ExpansionCESpawnableTypes();
		types.ReadTypes(fileName);
		return types;
	}

	void ReadTypes(string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("spawnabletypes")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'spawnabletypes' in '%1'", fileName);
				return;
			}

			auto types = root.GetContent().GetTags();
			foreach (CF_XML_Tag type: types)
			{
				string tagName = type.GetName();
				if (tagName != "type")
					continue;

				string name = ExpansionXML.GetAttributeString(type, "name");

				string key = name;
				key.ToLower();

				auto spawnableType = new ExpansionCESpawnableType(name);

				auto attachmentGroups = type.GetTag("attachments");
				foreach (CF_XML_Tag attachmentGroup: attachmentGroups)
				{
					float groupChance = ExpansionXML.GetAttributeFloat(attachmentGroup, "chance", 0.0);
					auto group = new ExpansionCESpawnableAttachmentGroup(groupChance);

					auto items = attachmentGroup.GetTag("item");
					foreach (CF_XML_Tag item: items)
					{
						string itemName = ExpansionXML.GetAttributeString(item, "name");
						float itemChance = ExpansionXML.GetAttributeFloat(item, "chance", 0.0);

						group.AddItem(itemName, itemChance);
					}

					spawnableType.AddAttachmentGroup(group);
				}

				Merge(key, spawnableType);
			}
		}
	}

	void Merge(string key, ExpansionCESpawnableType spawnableType)
	{
		ExpansionCESpawnableType existing;
		if (Find(key, existing))
			existing.Merge(spawnableType);
		else
			Insert(key, spawnableType);
	}
}
