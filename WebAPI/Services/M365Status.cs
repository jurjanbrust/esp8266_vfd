using System;
using System.Xml;
using System.Linq;
using WebAPI.Models;
using WebAPI.Helpers;
using WebAPI.Controllers;
using System.ServiceModel.Syndication;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;
using System.Xml.Linq;

namespace WebAPI.Services
{
    public class M365Status : RssFeed {


        public M365Status(ILogger<DisplayController> logger, string url, Display option) : base(logger, url, option) { }

        public override List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing m365");

            List<DisplayItem> displayItems = new List<DisplayItem>();

            using var reader = XmlReader.Create(url);
            var feed = SyndicationFeed.Load(reader);

            foreach (var item in feed.Items)
            {
                foreach (SyndicationElementExtension extension in item.ElementExtensions)
                {
                    XElement ele = extension.GetObject<XElement>();
                    Console.WriteLine(ele.Value);
                    if (ele.Name.LocalName == "status" && ele.Value != "Available")
                    {
                        AddToDisplay(displayItems, item);
                    }
                }
            }

            reader.Close();
            return displayItems;
        } 
    }
}
