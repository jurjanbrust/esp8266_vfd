using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Xml;
using WebAPI.Controllers;
using WebAPI.Models;
using System.ServiceModel.Syndication;
using System.Linq;

namespace WebAPI.Services
{
    public class RssFeed {

        protected ILogger<DisplayController> logger;
        private readonly string url;

        public RssFeed(ILogger<DisplayController> logger, string url)
        {
            this.logger = logger;
            this.url = url;
        }

        public List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing tech");

            List<DisplayItem> displayItems = new List<DisplayItem>();

            using var reader = XmlReader.Create(url);
            var feed = SyndicationFeed.Load(reader);
            var post = feed.Items.FirstOrDefault();
            reader.Close();

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = post.Title.Text,
                Line2 = post.Summary.Text,
                DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                Delay = 2000
            });

            return displayItems;
        }
    }
}
