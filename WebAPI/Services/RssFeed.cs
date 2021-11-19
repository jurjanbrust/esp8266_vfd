using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Xml;
using WebAPI.Controllers;
using WebAPI.Models;
using System.ServiceModel.Syndication;
using System.Linq;
using System.Text.RegularExpressions;

namespace WebAPI.Services
{
    public class RssFeed {

        protected ILogger<DisplayController> logger;
        private readonly string url;
        private Display option;

        public enum Display
        {
            Title,
            Summary
        }

        public RssFeed(ILogger<DisplayController> logger, string url, Display option)
        {
            this.logger = logger;
            this.url = url;
            this.option = option;
        }

        public List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing tech");

            List<DisplayItem> displayItems = new List<DisplayItem>();

            using var reader = XmlReader.Create(url);
            var feed = SyndicationFeed.Load(reader);
            var post = feed.Items.FirstOrDefault();
            reader.Close();
            string text = string.Empty;

            switch (option)
            {
                case Display.Title:
                    text = post.Title.Text;
                    break;
                case Display.Summary:
                    text = post.Summary.Text;
                    break;
            }
            text = Regex.Replace(text, @"<[^>]*>", String.Empty);
            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = text,
                Line2 = "",
                DisplayMode = DisplayItem.DisplayModeEnum.VerticalScroll,
                Delay = 2000
            });

            return displayItems;
        }
    }
}
