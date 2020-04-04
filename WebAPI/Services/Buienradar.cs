using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using WebAPI.Controllers;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class Buienradar : JSonService
    {
        public Buienradar(ILogger<DisplayController> logger, IConfiguration configuration) : base(logger, configuration)
        {
        }

        public List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing buienradar");

            List<DisplayItem> displayItems = new List<DisplayItem>();
            dynamic stuff = GetJson("https://data.buienradar.nl/2.0/feed/json");

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = (int)stuff.actual.stationmeasurements[12].temperature + "'C " + stuff.actual.stationmeasurements[12].weatherdescription,
                Line2 = ((String)stuff.forecast.weatherreport.title).Replace("&nbsp;", ""),
                DisplayMode = DisplayItem.DisplayModeEnum.FadeInOut,
                Delay = 2000
            });

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = "Weersverwachting",
                Line2 = ((String)stuff.forecast.weatherreport.summary).Replace("&nbsp;", ""),
                DisplayMode = DisplayItem.DisplayModeEnum.VerticalScroll,
                Delay = 2000
            });

            return displayItems;
        }
    }
}
