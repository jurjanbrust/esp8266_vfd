using System;
using System.Collections.Generic;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class TestData
    {
        public List<DisplayItem> Refresh()
        {
            List<DisplayItem> displayItems = new()
            {
                new DisplayItem
                {
                    Date = DateTime.Now,
                    Line1 = "Horizontal scroll",
                    Line2 = "Hallo wereld 1234 met een zeer lange tekst",
                    DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                    Delay = 1000
                },

                new DisplayItem
                {
                    Date = DateTime.Now,
                    Line1 = "Vertical scroll",
                    Line2 = "Hallo wereld 1234 met een zeer lange tekst",
                    DisplayMode = DisplayItem.DisplayModeEnum.VerticalScroll,
                    Delay = 1000
                }
            };

            return displayItems;
        }
    }
}
