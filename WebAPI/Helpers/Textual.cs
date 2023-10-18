using System;
using System.Globalization;
using System.Text;
using System.Text.RegularExpressions;

namespace WebAPI.Helpers
{
    public static class Textual
    {
        private static string RemoveDiacritics(string text)
        {
            var normalizedString = text.Normalize(NormalizationForm.FormD);
            var stringBuilder = new StringBuilder(capacity: normalizedString.Length);

            for (int i = 0; i < normalizedString.Length; i++)
            {
                char c = normalizedString[i];
                var unicodeCategory = CharUnicodeInfo.GetUnicodeCategory(c);
                if (unicodeCategory != UnicodeCategory.NonSpacingMark)
                {
                    stringBuilder.Append(c);
                }
            }

            return stringBuilder
                .ToString()
                .Normalize(NormalizationForm.FormC);
        }

        public static string CleanupText(string text)
        {
            text = Regex.Replace(text, @"<[^>]*>", String.Empty);
            text = RemoveDiacritics(text);
            return text.Replace("&amp;", "&").Replace("\n", "").Replace("’", "'").Replace("°", "'").Replace("&nbsp;", "").Replace("\n\n", " ").Trim();
        }
    }
}
