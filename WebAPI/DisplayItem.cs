using System;

namespace WebAPI.Models
{
    public class DisplayItem
    {
        public enum DisplayModeEnum
        {
            Normal,               // 0
            FadeOut,              // 1
            FadeIn,               // 2
            Scroll,               // 3
            KnightRider,          // 4
            KnightRider2,         // 5
            KnightRider3,         // 6
            VerticalScroll,       // 7
            HorizontalScroll,     // 8
            ClearScreen,          // 9
            SetBrightness0,       // 10
            SetBrightness1,       // 11
            SetBrightness2,       // 12
            SetBrightness3,       // 13
            SetBrightness4,       // 14
            SetBrightness5,       // 15
            SetBrightness6,       // 16
            SetBrightness7,       // 17
            FadeLeftToRight,      // 18
            FadeRightToLeft,      // 19
        }

        public DateTime Date { get; set; }

        public string Line1 { get; set; }

        public string Line2 { get; set; }

        public DisplayModeEnum DisplayMode { get; set; }

        public int Delay { get; set; }
    }
}
