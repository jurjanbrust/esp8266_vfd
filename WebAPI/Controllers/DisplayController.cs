using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using WebAPI.Models;
using WebAPI.Services;

namespace WebAPI.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class DisplayController : ControllerBase
    {
        private readonly ILogger<DisplayController> _logger;
        private List<DisplayItem> _displayItems;

        public DisplayController(ILogger<DisplayController> logger)
        {
            _logger = logger;
            _displayItems = new List<DisplayItem>();
        }

        [HttpGet]
        public async System.Threading.Tasks.Task<IEnumerable<DisplayItem>> GetAsync()
        {
            TestData testdata= new TestData();
            Buienradar buienradar = new Buienradar(_logger);
            Flitsers flitsers = new Flitsers(_logger);
            Agenda agenda = new Agenda(_logger);
            Fitbit fitbit = new Fitbit(_logger);
            
            try
            {
                //_displayItems.AddRange(testdata.Refresh());
                _displayItems.AddRange(await fitbit.RefreshAsync());
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
