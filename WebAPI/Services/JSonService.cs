﻿using System;
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
        protected HttpClient httpClient;
        protected ILogger<DisplayController> logger;
        protected KeyVault keyVault;

        public JSonService(ILogger<DisplayController> logger, IConfiguration configuration)
        {
            httpClient = new HttpClient();
            keyVault = new KeyVault(logger, configuration);

            httpClient.Timeout = new TimeSpan(0, 0, 10);
            this.logger = logger;
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
    }
}
