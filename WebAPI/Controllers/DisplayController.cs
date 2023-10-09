﻿using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using WebAPI.Models;
using WebAPI.Services;
using Buienradar = WebAPI.Services.Buienradar;

namespace WebAPI.Controllers
{

    [ApiController]
    [Route("[controller]")]
    public class DisplayController : ControllerBase
    {
        public static int group = 1;    // static so that it does not reset to zero on a new http call
        private const int numberOfGroups = 1;
        private readonly ILogger<DisplayController> _logger;
        private List<DisplayItem> _displayItems;
        public IConfiguration _configuration;

        public DisplayController(ILogger<DisplayController> logger, IConfiguration configuration)
        {
            _logger = logger;
            _configuration = configuration;
            _displayItems = new List<DisplayItem>();
        }

        [HttpGet]
        public async System.Threading.Tasks.Task<IEnumerable<DisplayItem>> GetAsync()
        {
            //TestData testdata = new TestData();
            //Fitbit fitbit = new Fitbit(_logger, _configuration);
            RssFeed tweakers = new(_logger, "http://feeds.feedburner.com/tweakers/nieuws", RssFeed.Display.Summary, "Tweakers");
            RssFeed tech = new(_logger, "https://www.nu.nl/rss/Tech", RssFeed.Display.Summary, "Nu.nl Tech");
            RssFeed nieuws = new(_logger, "https://www.nu.nl/rss/Algemeen", RssFeed.Display.Summary, "Nu.nl Nieuws");
            //RssFeed engadget = new(_logger, "https://www.engadget.com/rss.xml", RssFeed.Display.Title, "Engadget");

            //RssFeed techCrunchAI = new(_logger, "https://techcrunch.com/category/artificial-intelligence/feed/", RssFeed.Display.Title, "TechCrunch AI");
            //RssFeed techCrunchMedia = new(_logger, "https://techcrunch.com/category/media-entertainmen/feed/", RssFeed.Display.Title, "TechCrunch Media");
            //RssFeed azure = new(_logger, "https://azurecomcdn.azureedge.net/nl-nl/blog/feed/", RssFeed.Display.Summary, "Azure");
            //RssFeed cnnTop = new(_logger, "http://rss.cnn.com/rss/edition.rss", RssFeed.Display.Title, "CNN Top");
            //RssFeed cnnWorld = new(_logger, "http://rss.cnn.com/rss/edition_world.rss", RssFeed.Display.Title, "CNN World");
            //RssFeed cnnEurope = new(_logger, "http://rss.cnn.com/rss/edition_europe.rss", RssFeed.Display.Title, "CNN Europe");

            M365Status status = new(_logger, "https://status.office365.com/api/feed/mac", M365Status.Display.Summary);
            Buienradar buienradar = new(_logger, _configuration);
            Flitsers flitsers = new(_logger, _configuration);
            Agenda agenda = new(_logger, _configuration);

            //_displayItems.AddRange(testdata.Refresh());
            //_displayItems.AddRange(await fitbit.RefreshAsync());
            AddWithEffect(await agenda.RefreshAsync(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(status.Refresh(), DisplayItem.DisplayModeEnum.Scroll);
            AddWithEffect(buienradar.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);

            // Only return items of the group so that the esp does not run out of memory (it will if it has to process too much data)
            switch (group)
            {
                case 1:
                    AddWithEffect(tweakers.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
                    AddWithEffect(tech.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
                    AddWithEffect(nieuws.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
                    break;
                default:
                    break;
            }

            AddWithEffect(flitsers.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            
            group++;
            if (group > numberOfGroups)
            {
                group = 1;
            }

            return _displayItems;
        }

        void AddWithEffect(List<DisplayItem> items, DisplayItem.DisplayModeEnum effect)
        {
            _displayItems.AddRange(items);
            if (items.Count > 0)
            {
                //_displayItems.Add(new DisplayItem { DisplayMode = effect, Delay = 10 });
                _displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.ClearScreen, Delay = 10 });
                //_displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.SetBrightness5, Delay = 10 });
                // added 10 ms delay, so that the VFD can process the commands (not doing this results in # chars on the display)
            }
        }
    }
}
