using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using WebAPI.Controllers;
using WebAPI.Models;
using WebAPI.Helpers;

namespace WebAPI.Services
{
    public class Flitsers : JSonService
    {
        public Flitsers(ILogger<DisplayController> logger, IConfiguration configuration) : base(logger, configuration)
        {
        }

        public List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing flitsers");

            List<DisplayItem> displayItems = new();
            dynamic stuff = GetJson("https://tesla.flitsmeister.nl/teslaFeed.json");
            int i = 0;
            foreach (var item in stuff.features)
            {
                i++;
                try
                {
                    float longitude = (item.geometry.coordinates[0].Type == Newtonsoft.Json.Linq.JTokenType.Float) ? item.geometry.coordinates[0] : item.geometry.coordinates[0][0];
                    float latitude = (item.geometry.coordinates[0].Type == Newtonsoft.Json.Linq.JTokenType.Float) ? item.geometry.coordinates[1] : item.geometry.coordinates[0][1];
                    if (item.properties.road == "A28" && longitude > 6.460000 && latitude > 53.000000)
                    {
                        displayItems.Add(new DisplayItem
                        {
                            Date = DateTime.Now,
                            Line1 = Textual.RemoveDiacritics(String.Format("{0} op {1}", Translate(item.properties.type_description), item.properties.road)),
                            Line2 = Textual.RemoveDiacritics(String.Format("{0} {1}", item.properties.location, item.properties.direction)),
                            DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                            Delay = 1000
                        });
                    }
                }
                catch (Exception e)
                {
                    logger.LogError($"Flitsers: {e.Message} {e.StackTrace}");
                }

            }
            return displayItems;
        }

        private static string Translate(dynamic input)
        {
            if (input == null)
            {
                return "Onbekend";
            }
            string output;
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
                    output = input.Value ;
                    break;
            }
            return output;
        }
    }
}
