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
		TStringArray parts = {};
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

	//! sRGB to L*a*b* conversion, D65 reference
	static void sRGBToLab(float r, float g, float b, out float lab_l, out float lab_a, out float lab_b)
	{
		float rgb[3];
		float xyz[3];

		rgb[0] = r / 255.0;
		rgb[1] = g / 255.0;
		rgb[2] = b / 255.0;

		int i;

		for (i = 0; i < 3; i++)
		{
			if (rgb[i] > 0.04045)
				rgb[i] = Math.Pow((rgb[i] + 0.055) / 1.055, 2.4);
			else
				rgb[i] = rgb[i] / 12.92;
		}

		xyz[0] = ((rgb[0] * 0.412391) + (rgb[1] * 0.357584) + (rgb[2] * 0.180481));  
		xyz[1] = ((rgb[0] * 0.212639) + (rgb[1] * 0.715169) + (rgb[2] * 0.072192));
		xyz[2] = ((rgb[0] * 0.019331) + (rgb[1] * 0.119195) + (rgb[2] * 0.950532));

		xyz[0] = xyz[0] / 0.950456;
		xyz[1] = xyz[1] / 1.0;
		xyz[2] = xyz[2] / 1.089058;

		for (i = 0; i < 3; i++)
		{
			if (xyz[i] > 0.008856)
				xyz[i] = Math.Pow(xyz[i], (1.0 / 3.0));
			else
				xyz[i] = (903.296296 * xyz[i] + 16.0) / 116.0;
		}

		lab_l = 116.0 * xyz[1] - 16.0;
		lab_a = 500.0 * (xyz[0] - xyz[1]);
		lab_b = 200.0 * (xyz[1] - xyz[2]);
	}
}
