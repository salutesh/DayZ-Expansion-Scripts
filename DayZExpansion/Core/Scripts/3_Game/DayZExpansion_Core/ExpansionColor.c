/**
 * ExpansionColor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionColor
{
	//! DayZ ARGB int to RRGGBBAA hex color string
	static string ARGBToHex(int color)
	{
		TIntArray argb = ExpansionStatic.IntToByteArray(color);

		return ARGBToHex(argb[0], argb[1], argb[2], argb[3]);
	}

	//! ARGB to RRGGBBAA hex color string
	static string ARGBToHex(int a, int r, int g, int b)
	{
		return ExpansionStatic.ByteArrayToHex({r, g, b, a});
	}

	//! RGB, RGBA, RRGGBB or RRGGBBAA hex color string to DayZ ARGB color int
	static int HexToARGB(string hexcolor)
	{
		if (hexcolor.Length() < 3 || hexcolor.Length() > 8)
			return 0;

		string hex;

		if (hexcolor.Length() <= 4)
		{
			//! RGBA or RGBA
			hex = hexcolor[0] + hexcolor[0] + hexcolor[1] + hexcolor[1] + hexcolor[2] + hexcolor[2];
			if (hexcolor.Length() == 4)
				hex += hexcolor[3] + hexcolor[3];  //! Alpha
		}
		else
		{
			hex = hexcolor;
		}

		int color[4];
		color[3] = 255;  //! Init alpha to fully opaque

		for (int i = 0; i < hex.Length() / 2; i++)
		{
			color[i] = ExpansionStatic.HexToInt(hex.Substring(i * 2, 2));
		}

		return ARGB(color[3], color[0], color[1], color[2]);
	}

	static int ToARGB(string format)
	{
		TStringArray parts();
		if (format.IndexOf(",") > -1)
			format.Split(",", parts);
		else if (format.IndexOf(" ") > -1)
			format.Split(" ", parts);
		else
			return HexToARGB(format);

		int color[4];
		color[3] = 255;  //! Init alpha to fully opaque

		int i;
		foreach (string part: parts)
		{
			part.TrimInPlace();
			if (!part)
				continue;
			color[i++] = part.ToInt();
		}

		return ARGB(color[3], color[0], color[1], color[2]);
	}
}
