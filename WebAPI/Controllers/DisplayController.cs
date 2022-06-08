using System;
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
            RssFeed tweakers = new RssFeed(_logger, "http://feeds.feedburner.com/tweakers/nieuws", RssFeed.Display.Summary);
            RssFeed tech = new RssFeed(_logger, "https://www.nu.nl/rss/Tech", RssFeed.Display.Summary);
            RssFeed nieuws = new RssFeed(_logger, "https://www.nu.nl/rss/Algemeen", RssFeed.Display.Summary);
            Buienradar buienradar = new Buienradar(_logger, _configuration);
            Flitsers flitsers = new Flitsers(_logger, _configuration);
            Agenda agenda = new Agenda(_logger, _configuration);
            
            try
            {
                //_displayItems.AddRange(testdata.Refresh());
                //_displayItems.AddRange(await fitbit.RefreshAsync());
                AddWithEffect(await agenda.RefreshAsync(), DisplayItem.DisplayModeEnum.KnightRider);
                AddWithEffect(tweakers.Refresh(), DisplayItem.DisplayModeEnum.FadeOut);
                AddWithEffect(tech.Refresh(), DisplayItem.DisplayModeEnum.FadeOut);
                AddWithEffect(nieuws.Refresh(), DisplayItem.DisplayModeEnum.FadeOut);
                AddWithEffect(buienradar.Refresh(), DisplayItem.DisplayModeEnum.FadeOut);
                AddWithEffect(flitsers.Refresh(), DisplayItem.DisplayModeEnum.FadeOut);
            }
            catch (Exception e)
            {
                _logger.LogError(e.Message + e.StackTrace);
            }

            return _displayItems;

            void AddWithEffect(List<DisplayItem> items, DisplayItem.DisplayModeEnum effect)
            {
                _displayItems.AddRange(items);
                if (items.Count > 0)
                {
                    _displayItems.Add(new DisplayItem { DisplayMode = effect });
                    _displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.ClearScreen });
                    _displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.SetBrightness0 });
                }
            }
        }
    }
}
