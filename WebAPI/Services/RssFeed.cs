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
    public class RssFeed
    {

        protected ILogger<DisplayController> logger;
        protected readonly string url;
        private readonly Display option;

        public string Source { get; }

        public enum Display
        {
            Title,
            Summary
        }

        public RssFeed(ILogger<DisplayController> logger, string url, Display option, string source = null)
        {
            this.logger = logger;
            this.url = url;
            this.option = option;
            Source = source;
        }

        public virtual List<DisplayItem> Refresh()
        {
            List<DisplayItem> displayItems = new();
            try
            {
                logger.LogInformation("Refreshing rss");
                using var reader = XmlReader.Create(url);
                var feed = SyndicationFeed.Load(reader);
                var post = feed.Items.FirstOrDefault();
                AddToDisplay(displayItems, post);
                reader.Close();
            }
            catch (Exception e)
            {
                logger.LogError(e.Message + e.StackTrace);
            }

            return displayItems;
        }

        public void AddToDisplay(List<DisplayItem> displayItems, SyndicationItem post)
        {
            string text = Source == null ? string.Empty : $"{Source}: ";

            switch (option)
            {
                case Display.Title:
                    text += post.Title.Text.Trim();
                    break;
                case Display.Summary:
                    // fix to use text if summary only contains strings or is empty
                    if (post.Summary.Text.Trim().Length == 0)
                    {
                        text += post.Title.Text.Trim();
                    }
                    else
                    {
                        text += post.Summary.Text.Trim();
                    }
                    break;
            }
            text = Regex.Replace(text, @"<[^>]*>", String.Empty);
            text = text.Replace("&amp;", "&");
            text = text.Replace("\n", "");
            text = text.Replace("’", "'");

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
