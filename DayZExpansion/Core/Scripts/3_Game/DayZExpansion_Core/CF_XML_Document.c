#ifndef CF_BUGFIX_XML
modded class CF_XML_Document
{
	override private bool ReadTag()
	{
		string c = _reader.SkipWhitespace();
		string tagName = "";

		if (c == "<")
		{
			c = _reader.GetCharacter();
			if (c == "/")
			{
				tagName = _reader.GetWord();

				if (_currentTag == NULL)
				{
					_reader.Err("Unexpected closing tag: " + tagName);
				}

				c = _reader.GetCharacter();
				if (c != ">")
				{
					_reader.Err("Expected '>' for closing tag, got: " + c);
				}

				PopTag();

				return true;
			}

			if (c != "?")
				_reader.BackChar();

			tagName = _reader.GetWord();

			CreateTag(tagName);

			c = _reader.SkipWhitespace();
			while (c != "/" && c != "?" && c != ">")
			{
				c = _reader.BackChar();

				if (!ReadAttribute())
					return false;

				c = _reader.SkipWhitespace();
			}

			if (c == "/" || c == "?")
			{
				c = _reader.GetCharacter();
				if (c != ">")
				{
					_reader.Err("Expected '>' for inline closing tag, got: " + c);
				}

				PopTag();
			}
			else if (c == ">")
			{
				c = _reader.SkipWhitespace();
				if (c == "<")
				{
					c = _reader.BackChar();
					return true;
				}

				string content = c;

				while (true)
				{
					bool wasNewLine = _reader.WasNewLine();
					if (wasNewLine) c = _reader.SkipWhitespace();
					else c = _reader.GetCharacter();

					if (c != "<")
					{
						if (wasNewLine) content += "\n";
						content += c;
						continue;
					}
					
					c = _reader.GetCharacter();
					if (c != "/")
					{
						_reader.Err("Expected '/' for inline closing tag, got: " + c);
					}

					tagName = _reader.GetWord();
					
					if (tagName != _currentTag.GetName())
					{
						_reader.Err("Unexpected closing tag: " + tagName);
					}
				
					break;
				}

				foreach (string ent_key, string ent_val : _entities)
					ExpansionString.Replace(content, ent_key, ent_val);

				_currentTag.GetContent().SetContent(content);

				c = _reader.GetCharacter();
				if (c != ">")
				{
					_reader.Err("Expected '>' for closing tag, got: " + c);
				}

				PopTag();

				return true;
			}
			else
			{
				_reader.Err("Expected '>' for opening element within tag, got: " + c);
			}

			return true;
		}

		// could just be non-tagged content.

		return true;
	}
}
#endif
