using System;
using System.Linq;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using Azure.Security.KeyVault.Secrets;
using Azure.Identity;

namespace WebAPI.Helpers
{
    public class Keys
    {
        public string Refresh { get; set; }
        public string Access { get; set; }
        public string Basic { get; set; }
        public string UserId { get; internal set; }
        public string Prefix { get; internal set; }
        public string RefreshUrl { get; internal set; }
        public string ClientID { get; internal set; }
        public string ClientSecret { get; internal set; }
    }

    public class KeyVault
    {
        public Dictionary<string, Keys> keys;
        public SecretClient keyVaultClient;
        protected IConfiguration Configuration { get; set; }
        private readonly string KeyVaultUrl;

        public KeyVault(ILogger<Controllers.DisplayController> logger, IConfiguration configuration)
        {
            KeyVaultUrl = configuration.GetSection("VaultUri").Value;
            keyVaultClient = new SecretClient(new Uri(KeyVaultUrl), new DefaultAzureCredential());
            keys = new Dictionary<string, Keys>();
            logger.LogInformation("KeyVaultUrl = " + KeyVaultUrl);
        }

        public void SetSecret(string keyName, string keyValue)
        {
            var secret = new KeyVaultSecret(keyName, keyValue);
            keyVaultClient.SetSecret(secret);
        }

        public async Task<string> GetSecret(string keyName)
        {
            try
            {
                var secret = await keyVaultClient.GetSecretAsync(keyName).ConfigureAwait(false);
                return secret.Value.Value;
            }
            catch (Exception)
            {
                return string.Empty;
            }
        }

        public void StoreSecrets()
        {
            keys.ToList().ForEach(i =>
            {
                if (!string.IsNullOrEmpty(i.Value.Refresh)) SetSecret(i.Key + "Refresh", i.Value.Refresh);
                if (!string.IsNullOrEmpty(i.Value.Access)) SetSecret(i.Key + "Access", i.Value.Access);
                if (!string.IsNullOrEmpty(i.Value.Basic)) SetSecret(i.Key + "Basic", i.Value.Basic);
                if (!string.IsNullOrEmpty(i.Value.UserId)) SetSecret(i.Key + "UserId", i.Value.UserId);
                if (!string.IsNullOrEmpty(i.Value.ClientID)) SetSecret(i.Key + "ClientID", i.Value.ClientID);
                if (!string.IsNullOrEmpty(i.Value.ClientSecret)) SetSecret(i.Key + "ClientSecret", i.Value.ClientSecret);
                if (!string.IsNullOrEmpty(i.Value.Prefix)) SetSecret(i.Key + "Prefix", i.Value.Prefix);
            });
        }

        public async Task GetSecrets()
        {
            foreach (var item in keys)
            {
                item.Value.Refresh = await GetSecret(item.Key + "Refresh");
                item.Value.Access = await GetSecret(item.Key + "Access");
                item.Value.Basic = await GetSecret(item.Key + "Basic");
                item.Value.UserId = await GetSecret(item.Key + "UserId");
                item.Value.ClientID = await GetSecret(item.Key + "ClientID");
                item.Value.ClientSecret = await GetSecret(item.Key + "ClientSecret");
                item.Value.Prefix = await GetSecret(item.Key + "Prefix");
            }
        }
    }
}
