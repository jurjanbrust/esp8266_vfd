using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using System.Linq;
using WebAPI.Controllers;
using WebAPI.Models;
using WebAPI.Helpers;

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

            List<DisplayItem> displayItems = new();
            Rootobject json = GetJson<Rootobject>("https://data.buienradar.nl/2.0/feed/json");
            var station = json.actual.stationmeasurements.First(s => s.stationid == 6279);
            var shortterm = json.forecast.shortterm;
            var forecast = json.forecast.weatherreport.summary;

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = station.temperature + "'C " + station.weatherdescription,
                Line2 = Textual.CleanupText(shortterm.forecast),
                DisplayMode = DisplayItem.DisplayModeEnum.Normal,
                Delay = 6000
            });

            return displayItems;
        }
    }
}
