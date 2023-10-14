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
using System.Collections;

namespace WebAPI.Services
{
    public class Todo : JSonService
    {
        public Todo(ILogger<DisplayController> logger, IConfiguration configuration) : base(logger, configuration)
        {
            logger.LogInformation("Refreshing todo");
            this.logger = logger;
        }

        public async Task<List<DisplayItem>> RefreshAsync()
        {
            await keyVault.GetSecrets();

            List<DisplayItem> displayItems = new();
            try
            {
                string query = "https://graph.microsoft.com/v1.0/me/todo/lists";

                HttpResponseMessage httpResponse = await GetItemsAsync(query);
                if (httpResponse.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                {
                    await RefreshTokens(token);
                    httpResponse = await GetItemsAsync(query);
                }

                if (httpResponse.StatusCode == System.Net.HttpStatusCode.OK)
                {
                    string responseAsString = await httpResponse.Content.ReadAsStringAsync();
                    RootObject_Todo<TodoList> lists = new();
                    JsonConvert.PopulateObject(responseAsString, lists);
                    foreach (var item in lists.value)
                    {
                        var tasks = await GetTasksForListAsync(item.id);
                        displayItems.AddRange(CreateDisplayItem(tasks, item.displayName));
                    }
                }
                else
                {
                    displayItems.Add(new DisplayItem { Date = DateTime.Now, Line1 = "Todo foutmelding", Line2 = "Log opnieuw in", DisplayMode = DisplayItem.DisplayModeEnum.Normal, Delay = 2000 });
                    logger.LogError("Todo failed: " + httpResponse.StatusCode);
                }
            }
            catch (Exception e)
            {
                logger.LogError("Todo: " + e.Message + e.StackTrace);
            }

            return displayItems.OrderBy(x => x.Date).ToList();
        }

        private async Task<RootObject_Todo<TodoItem>> GetTasksForListAsync(string id)
        {
            string query = $"https://graph.microsoft.com/v1.0/me/todo/lists/{id}/tasks";
            HttpResponseMessage httpResponse = await GetItemsAsync(query);
            RootObject_Todo<TodoItem> todoitems = new();

            if (httpResponse.StatusCode == System.Net.HttpStatusCode.OK)
            {
                string responseAsString = await httpResponse.Content.ReadAsStringAsync();
                JsonConvert.PopulateObject(responseAsString, todoitems);
            }
            return todoitems;
        }

        private List<DisplayItem> CreateDisplayItem(RootObject_Todo<TodoItem> todoItems, string listName)
        {
            string firstLine;
            string secondLine;
            List<DisplayItem> displayItems = new();

            if (todoItems == null) return null;

            foreach (var item in todoItems.value)
            {
                firstLine = $"Todo: {listName}";
                secondLine = item.title + " " + item.importance; 

                displayItems.Add(new DisplayItem
                {
                    Date = DateTime.Now,
                    Line1 = firstLine,
                    Line2 = secondLine,
                    DisplayMode = DisplayItem.DisplayModeEnum.HorizontalScroll,
                    Delay = 2000
                });
            }
            return displayItems;
        }
    }
}
