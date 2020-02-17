using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using WebAPI.Controllers;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class Flitsers : JSonService
    {
        public Flitsers(ILogger<DisplayController> logger) : base(logger)
        {
        }

        public List<DisplayItem> Refresh()
        {
            List<DisplayItem> displayItems = new List<DisplayItem>();
            dynamic stuff = GetJson("https://tesla.flitsmeister.nl/teslaFeed.json");

            foreach (var item in stuff.features)
            {
                // groter dan 6.460000, 53.000000
                if (item.properties.road == "A28" && (float)item.geometry.coordinates[0] > 6.460000 && (float)item.geometry.coordinates[1] > 53.000000)
                {
                    displayItems.Add(new DisplayItem
                    {
                        Date = DateTime.Now,
                        Line1 = String.Format("{0} op {1}", Translate(item.properties.type_description), item.properties.road),
                        Line2 = String.Format("{0} {1}", item.properties.location, item.properties.direction),
                        DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                        Delay = 1000
                    });
                }
            }

            return displayItems;
        }

        private string Translate(dynamic input)
        {
            if(input == null)
            {
                return string.Empty;
            }
            string output = input;
            switch (input.Value)
            {
                case "speedtrap":
                    output = "Controle";
                    break;
                case "stationaryvehicle":
                    output = "Voertuig";
                    break;
                case "bridge_closed":
                    output = "Brug dicht";
                    break;
                case "accident":
                    output = "Ongeluk";
                    break;
                case "trafficjam":
                    output = "File";
                    break;
                case "obstruction":
                    output = "Obstructie";
                    break;
                case "slippery":
                    output = "Glad";
                    break;
                default:
                    break;
            }
            return output;
        }
    }
}
