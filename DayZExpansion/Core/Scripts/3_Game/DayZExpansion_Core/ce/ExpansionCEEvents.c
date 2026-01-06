/**
 * ExpansionCEEvents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionCEEventChild
{
	string Type;
	int LootMin;
	int LootMax;
	int Min;
	int Max;

	void ExpansionCEEventChild(string type)
	{
		Type = type;
	}

	void Merge(ExpansionCEEventChild other)
	{
		if (other.LootMin != ExpansionCE.NOT_SET)
			LootMin = other.LootMin;

		if (other.LootMax != ExpansionCE.NOT_SET)
			LootMax = other.LootMax;

		if (other.Min != ExpansionCE.NOT_SET)
			Min = other.Min;

		if (other.Max != ExpansionCE.NOT_SET)
			Max = other.Max;
	}
}

class ExpansionCEEventFlags
{
	int Deletable;
	int InitRandom;
	int RemoveDamaged;

	void Merge(ExpansionCEEventFlags other)
	{
		if (other.Deletable != ExpansionCE.NOT_SET)
			Deletable = other.Deletable;

		if (other.InitRandom != ExpansionCE.NOT_SET)
			InitRandom = other.InitRandom;

		if (other.RemoveDamaged != ExpansionCE.NOT_SET)
			RemoveDamaged = other.RemoveDamaged;
	}
}

class ExpansionCEEvent
{
	string Name;
	int Nominal;
	int Min;
	int Max;
	int Lifetime;
	int Restock;
	int SafeRadius;
	int DistanceRadius;
	int CleanupRadius;
	string Position;
	string Limit;
	int Active;
	ref ExpansionCEEventFlags Flags;
	ref map<string, ref ExpansionCEEventChild> Children = new map<string, ref ExpansionCEEventChild>();

	void ExpansionCEEvent(string name)
	{
		Name = name;
	}

	void AddChild(ExpansionCEEventChild child)
	{
		string type = child.Type;
		type.ToLower();

		Children[type] = child;
	}

	//! See https://community.bistudio.com/wiki/DayZ:Central_Economy_mission_files_modding for file specifics
	void Merge(ExpansionCEEvent other)
	{
		if (other.Nominal != ExpansionCE.NOT_SET)
			Nominal = other.Nominal;

		if (other.Min != ExpansionCE.NOT_SET)
			Min = other.Min;

		if (other.Max != ExpansionCE.NOT_SET)
			Max = other.Max;

		if (other.Lifetime != ExpansionCE.NOT_SET)
			Lifetime = other.Lifetime;

		if (other.Restock != ExpansionCE.NOT_SET)
			Restock = other.Restock;

		if (other.SafeRadius != ExpansionCE.NOT_SET)
			SafeRadius = other.SafeRadius;

		if (other.DistanceRadius != ExpansionCE.NOT_SET)
			DistanceRadius = other.DistanceRadius;

		if (other.CleanupRadius != ExpansionCE.NOT_SET)
			CleanupRadius = other.CleanupRadius;

		if (other.Position)
			Position = other.Position;

		if (other.Limit)
			Limit = other.Limit;

		if (other.Active != ExpansionCE.NOT_SET)
			Active = other.Active;

		if (other.Flags)
		{
			if (!Flags)
				Flags = new ExpansionCEEventFlags();

			Flags.Merge(other.Flags);
		}

		foreach (string type, ExpansionCEEventChild child: other.Children)
		{
			ExpansionCEEventChild existingChild = Children[type];
			if (existingChild)
				existingChild.Merge(child);
			else
				AddChild(child);
		}
	}
}

class ExpansionCEEvents: map<string, ref ExpansionCEEvent>
{
	static ExpansionCEEvents LoadEvents(string fileName)
	{
		ExpansionCEEvents events = new ExpansionCEEvents();
		events.ReadEvents(fileName);
		return events;
	}
	
	void ReadEvents(string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("events")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'events' in '%1'", fileName);
				return;
			}

			auto events = root.GetContent().GetTags();
			foreach (CF_XML_Tag eventTag: events)
			{
				string tagName = eventTag.GetName();
				if (tagName != "event")
					continue;

				string name = ExpansionXML.GetAttributeString(eventTag, "name");

				string key = name;
				key.ToLower();

				int defaultValue = 0;

				if (Contains(key))
					defaultValue = ExpansionCE.NOT_SET;

				auto ceEvent = new ExpansionCEEvent(name);

				ceEvent.Nominal = ExpansionXML.GetTagContentInt(eventTag, "nominal", defaultValue);
				ceEvent.Min = ExpansionXML.GetTagContentInt(eventTag, "min", defaultValue);
				ceEvent.Max = ExpansionXML.GetTagContentInt(eventTag, "max", defaultValue);
				ceEvent.Lifetime = ExpansionXML.GetTagContentInt(eventTag, "lifetime", defaultValue);
				ceEvent.Restock = ExpansionXML.GetTagContentInt(eventTag, "restock", defaultValue);
				ceEvent.SafeRadius = ExpansionXML.GetTagContentInt(eventTag, "saferadius", defaultValue);
				ceEvent.DistanceRadius = ExpansionXML.GetTagContentInt(eventTag, "distanceradius", defaultValue);
				ceEvent.CleanupRadius = ExpansionXML.GetTagContentInt(eventTag, "cleanupradius", defaultValue);
				ceEvent.Position = ExpansionXML.GetTagContentString(eventTag, "position", "");
				ceEvent.Limit = ExpansionXML.GetTagContentString(eventTag, "limit", "");
				ceEvent.Active = ExpansionXML.GetTagContentInt(eventTag, "active", defaultValue);

				auto flags = eventTag.GetTag("flags")[0];
				if (flags)
				{
					ceEvent.Flags = new ExpansionCEEventFlags();
					ceEvent.Flags.Deletable = ExpansionXML.GetAttributeInt(flags, "deletable", defaultValue);
					ceEvent.Flags.InitRandom = ExpansionXML.GetAttributeInt(flags, "init_random", defaultValue);
					ceEvent.Flags.RemoveDamaged = ExpansionXML.GetAttributeInt(flags, "remove_damaged", defaultValue);
				}

				auto childrenTag = eventTag.GetTag("children")[0];
				if (childrenTag)
				{
					auto children = childrenTag.GetTag("child");
					foreach (CF_XML_Tag child: children)
					{
						string type = ExpansionXML.GetAttributeString(child, "type");

						auto ceEventChild = new ExpansionCEEventChild(type);

						ceEventChild.LootMin = ExpansionXML.GetAttributeInt(child, "lootmin", defaultValue);
						ceEventChild.LootMax = ExpansionXML.GetAttributeInt(child, "lootmax", defaultValue);
						ceEventChild.Min = ExpansionXML.GetAttributeInt(child, "min", defaultValue);
						ceEventChild.Max = ExpansionXML.GetAttributeInt(child, "max", defaultValue);

						ceEvent.AddChild(ceEventChild);
					}
				}

				Merge(key, ceEvent);
			}
		}
	}

	void Merge(string key, ExpansionCEEvent ceEvent)
	{
		ExpansionCEEvent existing;
		if (Find(key, existing))
			existing.Merge(ceEvent);
		else
			Insert(key, ceEvent);
	}
}
