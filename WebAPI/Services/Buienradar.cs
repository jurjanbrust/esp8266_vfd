using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
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
            Rootobject json = GetJson<Rootobject>("https://data.buienradar.nl/2.0/feed/json");
            var station = json.actual.stationmeasurements.First(s => s.stationid == 6279);
            var shortterm = json.forecast.shortterm;
            var forecast = json.forecast.weatherreport.summary;

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = station.temperature + "'C " + station.weatherdescription,
                Line2 = shortterm.forecast.Replace("&nbsp;", "").Replace("\n\n", " "),
                DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                Delay = 6000
            });

            return displayItems;
        }
    }
}
