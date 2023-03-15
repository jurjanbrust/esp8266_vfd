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
            RssFeed tweakers = new RssFeed(_logger, "http://feeds.feedburner.com/tweakers/nieuws", RssFeed.Display.Summary, "Tweakers");
            RssFeed tech = new RssFeed(_logger, "https://www.nu.nl/rss/Tech", RssFeed.Display.Summary, "Tech");
            RssFeed nieuws = new RssFeed(_logger, "https://www.nu.nl/rss/Algemeen", RssFeed.Display.Summary, "Nieuws");
            RssFeed azure = new RssFeed(_logger, "https://azurecomcdn.azureedge.net/nl-nl/blog/feed/", RssFeed.Display.Summary, "Azure");
            M365Status status = new M365Status(_logger, "https://status.office365.com/api/feed/mac", M365Status.Display.Summary);
            Buienradar buienradar = new Buienradar(_logger, _configuration);
            Flitsers flitsers = new Flitsers(_logger, _configuration);
            Agenda agenda = new Agenda(_logger, _configuration);

            //_displayItems.AddRange(testdata.Refresh());
            //_displayItems.AddRange(await fitbit.RefreshAsync());
            AddWithEffect(await agenda.RefreshAsync(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(status.Refresh(), DisplayItem.DisplayModeEnum.Scroll);
            AddWithEffect(tweakers.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(tech.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(nieuws.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(azure.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(buienradar.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);
            AddWithEffect(flitsers.Refresh(), DisplayItem.DisplayModeEnum.ClearScreen);

            return _displayItems;

            void AddWithEffect(List<DisplayItem> items, DisplayItem.DisplayModeEnum effect)
            {
                _displayItems.AddRange(items);
                if (items.Count > 0)
                {
                    _displayItems.Add(new DisplayItem { DisplayMode = effect, Delay = 10 });
                    _displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.ClearScreen, Delay = 10 });
                    _displayItems.Add(new DisplayItem { DisplayMode = DisplayItem.DisplayModeEnum.SetBrightness5, Delay = 10 });
                    // added 10 ms delay, so that the VFD can process the commands (not doing this results in # chars on the display)
                }
            }
        }
    }
}
