using System;

namespace WebAPI.Models
{
    public class DisplayItem
    {
        public enum DisplayModeEnum
        {
            Normal,
            FadeInOut,
            Scroll,
            KnightRider,
            VerticalScroll,
            HorizontalScroll
        }

        public DateTime Date { get; set; }

        public string Line1 { get; set; }

        public string Line2 { get; set; }

        public DisplayModeEnum DisplayMode { get; set; }

        public int Delay { get; set; }
    }
}
