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
            RssFeed tech = new RssFeed(_logger, "https://www.nu.nl/rss/Tech", RssFeed.Display.Title);
            RssFeed nieuws = new RssFeed(_logger, "https://www.nu.nl/rss/Algemeen", RssFeed.Display.Title);
            Buienradar buienradar = new Buienradar(_logger, _configuration);
            Flitsers flitsers = new Flitsers(_logger, _configuration);
            Agenda agenda = new Agenda(_logger, _configuration);
            
            try
            {
                //_displayItems.AddRange(testdata.Refresh());
                //_displayItems.AddRange(await fitbit.RefreshAsync());
                _displayItems.AddRange(tweakers.Refresh());
                _displayItems.AddRange(tech.Refresh());
                _displayItems.AddRange(nieuws.Refresh());
                _displayItems.AddRange(buienradar.Refresh());
                _displayItems.AddRange(flitsers.Refresh());
                _displayItems.AddRange(await agenda.RefreshAsync());
            }
            catch (Exception e)
            {
                _logger.LogError(e.Message + e.StackTrace);
            }

            return _displayItems;
        }
    }
}
