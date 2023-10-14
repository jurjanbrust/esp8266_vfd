using System;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;
using WebAPI.Controllers;
using WebAPI.Helpers;

namespace WebAPI.Services
{
    public class JSonService
    {
        protected Keys token;
        protected HttpClient httpClient;
        protected KeyVault keyVault;
        protected ILogger<DisplayController> logger;

        public JSonService(ILogger<DisplayController> logger, IConfiguration configuration)
        {
            httpClient = new HttpClient();
            keyVault = new KeyVault(logger, configuration);

            httpClient.Timeout = new TimeSpan(0, 0, 10);
            this.logger = logger;

            token = new Keys
            {
                ClientID = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                ClientSecret = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Access = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                Refresh = "put your token here for first authorization and uncomment keyVault.StoreSecrets()",
                RefreshUrl = "https://login.microsoftonline.com/common/oauth2/v2.0/token",
                Basic = "",
                UserId = "",
                Prefix = "",
            };
            keyVault.keys.Add("Graph", token);
            //keyVault.StoreSecrets();  // only use this once to store the token above, tokens will change in the token refresh function
        }

        public dynamic GetJson(string url)
        {
            string result = GetStringAsync(url);
            return JsonConvert.DeserializeObject(result);
        }

        public T GetJson<T>(string url)
        {
            string result = GetStringAsync(url);
            return JsonConvert.DeserializeObject<T>(result);
        }

        public string GetStringAsync(string url)
        {
            return httpClient.GetStringAsync(url).Result;
        }

        public async Task<bool> RefreshTokens(Keys token)
        {
            logger.LogInformation($"Refreshing token for: {token.Prefix}");

            httpClient.DefaultRequestHeaders.Clear();
            if (!string.IsNullOrEmpty(token.Basic))
            {
                httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", token.Basic);
            }

            var content = new Dictionary<string, string>
            {
                { "grant_type", "refresh_token" },
                { "redirect_uri", "http://localhost" },
                { "refresh_token", token.Refresh }
            };

            if (!string.IsNullOrEmpty(token.ClientID))
            {
               content.Add("client_id", token.ClientID);
            }
            if (!string.IsNullOrEmpty(token.ClientSecret))
            {
              content.Add("client_secret", token.ClientSecret);
            }

            var encoded = new FormUrlEncodedContent(content);

            HttpResponseMessage httpResponse = await httpClient.PostAsync(token.RefreshUrl, encoded);

            if (httpResponse.StatusCode == System.Net.HttpStatusCode.OK)
            {
                string responseAsString = await httpResponse.Content.ReadAsStringAsync();
                Rootobject_RefreshToken jsonResponse = new();
                JsonConvert.PopulateObject(responseAsString, jsonResponse);
                token.Access = jsonResponse.access_token;
                token.Refresh = jsonResponse.refresh_token;
                keyVault.StoreSecrets();
            }
            else
            {
                string message = $"Error refreshing tokens: http {httpResponse.StatusCode} {httpResponse.Content.ReadAsStringAsync().Result}";
                logger.LogError(message);
            }
            return httpResponse.StatusCode == System.Net.HttpStatusCode.OK;
        }

        public async Task<HttpResponseMessage> GetItemsAsync(string query)
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
