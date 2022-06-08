using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using System.Net.Http.Headers;
using System.Threading.Tasks;
using WebAPI.Controllers;
using WebAPI.Helpers;
using WebAPI.Models;

namespace WebAPI.Services
{
    public class Fitbit : JSonService
    {
        public Fitbit(ILogger<DisplayController> logger, IConfiguration configuration) : base(logger, configuration)
        {
            this.logger = logger;

            keyVault.keys.Add("FitbitUser1", new Keys
            {
                Access = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Refresh = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Basic = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                UserId = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Prefix = "",
                RefreshUrl = "https://api.fitbit.com/oauth2/token"
            });

            keyVault.keys.Add("FitbitUser2", new Keys
            {
                Access = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Refresh = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Basic = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                UserId = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Prefix = "",
                RefreshUrl = "https://api.fitbit.com/oauth2/token"
            });

            //keyVault.StoreSecrets();  // only use this once to store the token above, tokens will change in the token refresh function
        }

        public async Task<List<DisplayItem>> RefreshAsync()
        {
            await keyVault.GetSecrets();
            
            List<DisplayItem> displayItems = new List<DisplayItem>();
            Keys user1 = keyVault.keys["FitbitUser1"];
            Keys user2 = keyVault.keys["FitbitUser2"];

            string line1 = await GetDataForToken(user1);
            string line2 = await GetDataForToken(user2);

            if (line1.Length > 0 || line2.Length > 0)
            {
                DisplayItem item = new DisplayItem
                {
                    Date = DateTime.Now,
                    Line1 = line1,
                    Line2 = line2,
                    DisplayMode = DisplayItem.DisplayModeEnum.Normal,
                    Delay = 2000
                };
                displayItems.Add(item);
            }
            return displayItems;
        }

        private async Task<string> GetDataForToken(Keys key)
        {
            string line = string.Empty;
            
            try
            {
                line = RefreshData(key);
            }
            catch (AggregateException e)
            {
                logger.LogWarning(e.Message);
                if (await RefreshTokens(key))
                {
                    line = RefreshData(key);
                }
            }
            catch (Exception e)
            {
                logger.LogError("Fitbit: " + e.Message + e.StackTrace);
            }
            return line;
        }

        private string RefreshData(Keys tokens)
        {
            logger.LogInformation("Refreshing fitbit data");

            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", tokens.Access);

            dynamic mass = GetJson(String.Format("https://api.fitbit.com/1/user/{0}/body/log/weight/date/{1}/1d.json", tokens.UserId, DateTime.Now.ToString("yyyy-MM-dd")));
            dynamic goal = GetJson(String.Format("https://api.fitbit.com/1/user/{0}/body/log/weight/goal.json", tokens.UserId));

            return ToGraphBar(mass, goal, tokens.Prefix);
        }

        private static string ToGraphBar(dynamic mass, dynamic goal, string prefix)
        {
            var m = mass.weight[0].weight;
            var g = goal.goal.weight;
            int size = 19 - prefix.Length;

            int stars;
            if (m - g > size)
            {
                stars = size;
            }
            else
            {
                stars = m - g;
            }

            string line = prefix + " ";
            for (int i = 0; i < stars; i++)
            {
                line += "#";
            }
            for (int i = stars; i < size; i++)
            {
                line += " ";
            }
            line += "";
            return line;
        }
    }
}
