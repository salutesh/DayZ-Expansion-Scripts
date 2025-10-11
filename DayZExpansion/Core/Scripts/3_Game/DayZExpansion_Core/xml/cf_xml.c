modded class CF_XML
{
#ifndef CF_XML_READ_DOCUMENT_EX
	//! TODO: Move this to CF
	static CF_XML_Document ReadDocumentEx(string file)
	{
		if (!FileExist(file))
		{
			CF.FormatError("File does not exist: '%1'", file);
			return null;
		}

		CF_XML_Document document;
		ReadDocument(file, document);
		return document;
	}
#endif
}
