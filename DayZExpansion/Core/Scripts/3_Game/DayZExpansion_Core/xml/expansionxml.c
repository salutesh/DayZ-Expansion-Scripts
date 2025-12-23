class ExpansionXML
{
	//! TODO: Move below static functions to CF_XML_Tag as non-static methods

	/**
	 * @brief Get attribute value as bool. If attribute does not exist, return defaultValue.
	 */
	static bool GetAttributeBool(CF_XML_Tag tag, string name, bool defaultValue = false)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
		{
			//! TODO: Change CF_XML_Attribute::ValueAsBool to detect "1" value as true
			switch (attr.GetValue())
			{
				case "true":
				case "1":
					return true;

				default:
					return false;
			}
		}

		return defaultValue;
	}

	/**
	 * @brief Get attribute value as float. If attribute does not exist, return defaultValue.
	 */
	static float GetAttributeFloat(CF_XML_Tag tag, string name, float defaultValue = 0)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
			return attr.ValueAsFloat();

		return defaultValue;
	}

	/**
	 * @brief Get attribute value as int. If attribute does not exist, return defaultValue.
	 */
	static int GetAttributeInt(CF_XML_Tag tag, string name, int defaultValue = 0)
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
			return attr.ValueAsInt();

		return defaultValue;
	}

	/**
	 * @brief Get attribute value as string. If attribute does not exist, return defaultValue.
	 */
	static string GetAttributeString(CF_XML_Tag tag, string name, string defaultValue = "")
	{
		auto attr = tag.GetAttribute(name);
		if (attr)
			return attr.ValueAsString();

		return defaultValue;
	}

	/**
	 * @brief Get tag content as int. If tag does not exist, return defaultValue.
	 */
	static int GetTagContentInt(CF_XML_Tag tag, string type, int defaultValue = 0)
	{
		auto children = tag.GetTag(type);
		if (children.Count() > 0)
			return children[0].GetContent().GetContent().ToInt();

		return defaultValue;
	}

	/**
	 * @brief Get tag content as string. If tag does not exist, return defaultValue.
	 */
	static string GetTagContentString(CF_XML_Tag tag, string type, string defaultValue = "")
	{
		auto children = tag.GetTag(type);
		if (children.Count() > 0)
			return children[0].GetContent().GetContent();

		return defaultValue;
	}


}
