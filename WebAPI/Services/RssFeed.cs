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

namespace WebAPI.Services
{
    public class RssFeed {

        protected ILogger<DisplayController> logger;
        protected readonly string url;
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

        public virtual List<DisplayItem> Refresh()
        {
            logger.LogInformation("Refreshing rss");

            List<DisplayItem> displayItems = new List<DisplayItem>();

            using var reader = XmlReader.Create(url);
            var feed = SyndicationFeed.Load(reader);
            var post = feed.Items.FirstOrDefault();
            AddToDisplay(displayItems, post);
            reader.Close();

            return displayItems;
        }

        public void AddToDisplay(List<DisplayItem> displayItems, SyndicationItem post)
        {
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

            // remove accents from string: é, û.... and so on
            text = Textual.RemoveDiacritics(text);

            displayItems.Add(new DisplayItem
            {
                Date = DateTime.Now,
                Line1 = text,
                Line2 = "",
                DisplayMode = DisplayItem.DisplayModeEnum.VerticalScroll,
                Delay = 2000
            });
        }
    }
}
