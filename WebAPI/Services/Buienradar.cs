using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using WebAPI.Controllers;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class Buienradar : JSonService
    {
        public Buienradar(ILogger<DisplayController> logger) : base(logger)
        {
        }

        public List<DisplayItem> Refresh()
        {
            List<DisplayItem> displayItems = new List<DisplayItem>();
            dynamic stuff = GetJson("https://data.buienradar.nl/2.0/feed/json");

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = "Temp: " + stuff.forecast.fivedayforecast[0].maxtemperature + "c wind: " + stuff.forecast.fivedayforecast[0].wind,
                Line2 = stuff.forecast.fivedayforecast[0].weatherdescription,
                DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                Delay = 2000
            });

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = "Weersverwachting",
                Line2 = stuff.forecast.shortterm.forecast,
                DisplayMode = DisplayItem.DisplayModeEnum.VerticalScroll,
                Delay = 2000
            });

            return displayItems;
        }
    }
}
