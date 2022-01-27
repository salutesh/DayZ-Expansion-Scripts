/**
 * ExpansionColor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionColor
{
	static const ref TStringArray HEX_BYTES = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
	};

	//! DayZ ARGB int to RRGGBBAA hex color string
	static string ARGBToHex(int color)
	{
		int a, r, g, b;

		a = color >> 24;
		if (a < 0)
			a += 256;
		r = (color & 0x00ff0000) >> 16;
		g = (color & 0x0000ff00) >> 8;
		b = (color & 0x000000ff);

		return ARGBToHex(a, r, g, b);
	}

	//! ARGB to RRGGBBAA hex color string
	static string ARGBToHex(int a, int r, int g, int b)
	{
		string hexcolor;

		hexcolor += IntToHex(r);
		hexcolor += IntToHex(g);
		hexcolor += IntToHex(b);
		hexcolor += IntToHex(a);

		return hexcolor;
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
			color[i] = HexToInt(hex.Substring(i * 2, 2));
		}

		return ARGB(color[3], color[0], color[1], color[2]);
	}

	static int HexToInt(string hex)
	{
		hex.ToUpper();

		int n;

		for (int i = 0; i < hex.Length(); i++)
		{
			//! Snatched from Dabs Framework
			n += HEX_BYTES.Find(hex[i]) * Math.Pow(16, Math.AbsInt(i - hex.Length() + 1));
		}

		return n;
	}

	static string IntToHex(int n, int mod = 2)
	{
		string hex;

		while (true)
		{
			hex = HEX_BYTES[n % 16] + hex;
			n /= 16;
			if (n <= 0)
				break;
		}

		while (hex.Length() % mod)
		{
			hex = "0" + hex;
		}

		return hex;
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
