using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System;
using System.Net.Http;
using WebAPI.Controllers;

namespace WebAPI.Services
{
    public class JSonService
    {
        private HttpClient httpClient;
        protected ILogger<DisplayController> logger;

        public JSonService(ILogger<DisplayController> logger)
        {
            httpClient = new HttpClient();
            httpClient.Timeout = new TimeSpan(0, 0, 10);
            this.logger = logger;
        }

        public dynamic GetJson(string url)
        {
            string ResultStream = httpClient.GetStringAsync(url).Result;
            dynamic stuff = JsonConvert.DeserializeObject(ResultStream);
            return stuff;
        }
    }
}