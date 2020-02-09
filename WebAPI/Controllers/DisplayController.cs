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
        public IEnumerable<DisplayItem> Get()
        {
            TestData testdata= new TestData();
            _logger.LogInformation("Testdata");
            Buienradar buienradar = new Buienradar(_logger);
            _logger.LogInformation("Buienradar");
            Flitsers flitsers = new Flitsers(_logger);
            _logger.LogInformation("Flitsers");

            try
            {
                //_displayItems.AddRange(testdata.Refresh());
                _displayItems.AddRange(buienradar.Refresh());
                _displayItems.AddRange(flitsers.Refresh());
            }
            catch (Exception e)
            {
                _logger.LogError(e.Message);
            }

            return _displayItems;
        }
    }
}
