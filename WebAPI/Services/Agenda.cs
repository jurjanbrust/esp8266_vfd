using System;
using System.Linq;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Globalization;
using System.Net.Http;
using System.Threading.Tasks;
using WebAPI.Controllers;
using WebAPI.Helpers;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class Agenda : JSonService
    {
        private Keys token;

        private const int CALENDAR_DAYS_LOOKAHEAD_1 = 2;
        private const int CALENDAR_DAYS_LOOKAHEAD_2 = 2;
        private const int CALENDAR_DAYS_LOOKAHEAD_3 = 3;
        private const int CALENDAR_DAYS_LOOKAHEAD_7 = 7;
        private const int MAX_ITEMS_TO_RETURN = 6;

        private static List<Rootobject_Calendar> calendarItems;
        private static List<string> queries;
        private readonly CultureInfo culture = new CultureInfo("nl-NL");

        public Agenda(ILogger<DisplayController> logger, IConfiguration configuration) : base(logger, configuration)
        {
            logger.LogInformation("Refreshing agenda");
            this.logger = logger;

            token = new Keys
            {
                Access = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Refresh = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Basic = "",
                TenantID = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                UserId = "",
                Prefix = "",
                RefreshUrl = "https://login.microsoftonline.com/common/oauth2/v2.0/token",
                ClientID = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                ClientSecret = "put your token here for first authorization and uncomment keyVault.StoreSecrets()"
            };
            keyVault.keys.Add("Graph", token);

            //keyVault.StoreSecrets();  // only use this once to store the token above, tokens will change in the token refresh function

            calendarItems = new List<Rootobject_Calendar>();
            queries = new List<string>();

            string query1 = String.Format("https://graph.microsoft.com/v1.0/me/calendars('AQMkADAwATE0YjEwLWFhADY2AC04MTJlLTAwAi0wMAoARgAAA6pTgb6w9M5IrqmqSLezaHIHANFVJ6ZDePlEroLNqPG9X4cAAAIBBgAAANFVJ6ZDePlEroLNqPG9X4cAAAGW6loAAAA=')/calendarview?startdatetime={0}&enddatetime={1}",
                                    DateTime.Now.ToString("yyyy-MM-ddTHH:mm:ss.fffZ"),
                                    DateTime.Now.AddDays(CALENDAR_DAYS_LOOKAHEAD_7).ToString("yyyy-MM-ddTHH:mm:ss.fffZ"));

            string query2 = String.Format("https://graph.microsoft.com/v1.0/me/calendarview?startdatetime={0}&enddatetime={1}",
                                    DateTime.Now.ToString("yyyy-MM-ddTHH:mm:ss.fffZ"),
                                    DateTime.Now.AddDays(CALENDAR_DAYS_LOOKAHEAD_7).ToString("yyyy-MM-ddTHH:mm:ss.fffZ"));

            string query3 = "https://graph.microsoft.com/v1.0/me/calendars('AQMkADAwATM0MDAAMS1mYTliLTgxMDItMDACLTAwCgBGAAAD3J4hd426JkeLiDCyHDVBkgcABhvvQclRTUGa5USHwWs0QAAAAgEGAAAABhvvQclRTUGa5USHwWs0QAAAAhT2AAAA')/calendarview?" +
                             "startdatetime=" + DateTime.Now.ToString("yyyy-MM-ddTHH:mm:ss.fffZ") +
                             "&enddatetime=" + DateTime.Now.AddDays(CALENDAR_DAYS_LOOKAHEAD_7).ToString("yyyy-MM-ddTHH:mm:ss.fffZ") +
                             "&$orderby=Start/DateTime" +
                             "&$top" + 1;

            queries.Add(query1);
            queries.Add(query2);
            //queries.Add(query3);
        }

        public async Task<List<DisplayItem>> RefreshAsync()
        {
            await keyVault.GetSecrets();

            List<DisplayItem> displayItems = new List<DisplayItem>();
            try
            {
                calendarItems.Clear();
                foreach (string query in queries)
                {
                    HttpResponseMessage httpResponse = await GetCalendarItemsAsync(query);
                    if (httpResponse.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                    {
                        await RefreshTokens(token);
                        httpResponse = await GetCalendarItemsAsync(query);
                    }

                    if (httpResponse.StatusCode == System.Net.HttpStatusCode.OK)
                    {
                        string responseAsString = await httpResponse.Content.ReadAsStringAsync();
                        Rootobject_Calendar calendarItem = new Rootobject_Calendar();
                        JsonConvert.PopulateObject(responseAsString, calendarItem);
                        displayItems.AddRange(CreateDisplayItem(calendarItem));
                    }
                    else
                    {
                        displayItems.Add(new DisplayItem { Date = DateTime.Now, Line1 = "Agenda foutmelding", Line2 = "Log opnieuw in", DisplayMode = DisplayItem.DisplayModeEnum.Normal, Delay = 2000 });
                        logger.LogError("RefreshCalendarItems failed: " + httpResponse.StatusCode);
                    }
                }
            }
            catch (Exception e)
            {
                logger.LogError("Agenda: " + e.Message + e.StackTrace);
            }

            if(displayItems.Count == 0)
            {

                displayItems.Add(new DisplayItem { Date = DateTime.Now, Line1 = "Geen komende agenda", Line2 = "afspraken.", DisplayMode = DisplayItem.DisplayModeEnum.Normal, Delay=2000 });
            } 

            return displayItems.OrderBy(x => x.Date).Take(MAX_ITEMS_TO_RETURN).ToList();
        }

        private List<DisplayItem> CreateDisplayItem(Rootobject_Calendar calendarItem)
        {
            string firstLine;
            string secondLine;
            List<DisplayItem> displayItems = new List<DisplayItem>();

            if (calendarItem == null) return null;

            foreach (var item in calendarItem.value)
            {
                DateTime time = item.Start.DateTime.AddHours(2);        // TODO timezone, zomertijd/wintertijd...
                string dag;
                if (time.Date == DateTime.Today)
                {
                    dag = "Vandaag";
                }
                else if (time.Date == DateTime.Now.AddDays(1).Date)
                {
                    dag = "Morgen";
                }
                else if (time.Date == DateTime.Now.AddDays(2).Date)
                {
                    dag = "Overmorgen";
                }
                else
                {
                    dag = CultureInfo.CurrentCulture.TextInfo.ToTitleCase(time.ToString("dddd", culture));
                }

                string subject = item.Subject.Trim();
                subject = Textual.RemoveDiacritics(subject);

                firstLine = dag + " " + time.ToString("dd MMMM", culture).ToString().Trim();
                secondLine = (time.ToString("HH:mm", culture).Replace("00:00", "").Replace("01:00", "").Replace("02:00", "") + " " + subject);

                displayItems.Add(new DisplayItem
                {
                    Date = time,
                    Line1 = firstLine,
                    Line2 = secondLine,
                    DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                    Delay = 2000
                });
            }
            return displayItems;
        }

        public async Task<HttpResponseMessage> GetCalendarItemsAsync(string query)
        {
            httpClient.DefaultRequestHeaders.Clear();
            httpClient.DefaultRequestHeaders.Add("Accept", "*/*");
            httpClient.DefaultRequestHeaders.Add("Cache-Control", "no-cache");
            httpClient.DefaultRequestHeaders.Add("Host", "graph.microsoft.com");
            httpClient.DefaultRequestHeaders.Add("Connection", "keep-alive");
            httpClient.DefaultRequestHeaders.Add("Authorization", "Bearer " + token.Access);
            HttpResponseMessage httpResponse = await httpClient.GetAsync(query);
            return httpResponse;
        }
    }
}
