using Microsoft.Azure.KeyVault;
using Microsoft.Azure.Services.AppAuthentication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.Extensions.Configuration;

namespace WebAPI.Helpers
{
    public class Keys
    {
        public string Refresh { get; set; }
        public string Access { get; set; }
        public string Basic { get; set; }
        public string UserId { get; internal set; }
        public string Prefix { get; internal set; }
        public string TenantID { get; internal set; }
        public string RefreshUrl { get; internal set; }
        public string ClientID { get; internal set; }
        public string ClientSecret { get; internal set; }
    }

    public class KeyVault
    {
        public Dictionary<string, Keys> keys;
        protected AzureServiceTokenProvider azureServiceTokenProvider;
        protected KeyVaultClient keyVaultClient;
        protected IConfiguration Configuration { get; set; }
        private readonly string KeyVaultUrl;

        public KeyVault(IConfiguration configuration)
        {
            azureServiceTokenProvider = new AzureServiceTokenProvider();
            keyVaultClient = new KeyVaultClient(new KeyVaultClient.AuthenticationCallback(azureServiceTokenProvider.KeyVaultTokenCallback));
            keys = new Dictionary<string, Keys>();

            KeyVaultUrl = configuration.GetSection("AzureWebUrl").Value;
        }

        public async Task SetSecretAsync(string keyName, string keyValue)
        {
            await keyVaultClient.SetSecretAsync(KeyVaultUrl, keyName, keyValue);
        }

        public async Task<string> GetSecret(string keyName)
        {
            try
            {
                var secret = await keyVaultClient.GetSecretAsync($"{KeyVaultUrl}/secrets/" + keyName).ConfigureAwait(false);
                return secret.Value;
            }
            catch (Exception)
            {
                return string.Empty;
            }
        }

        public void StoreSecrets()
        {
            keys.ToList().ForEach(async i =>
            {
                if(!string.IsNullOrEmpty(i.Value.Refresh)) await SetSecretAsync(i.Key + "Refresh", i.Value.Refresh);
                if (!string.IsNullOrEmpty(i.Value.Access)) await SetSecretAsync(i.Key + "Access", i.Value.Access);
                if (!string.IsNullOrEmpty(i.Value.Basic)) await SetSecretAsync(i.Key + "Basic", i.Value.Basic);
                if (!string.IsNullOrEmpty(i.Value.UserId)) await SetSecretAsync(i.Key + "UserId", i.Value.UserId);
                if (!string.IsNullOrEmpty(i.Value.TenantID)) await SetSecretAsync(i.Key + "TenantID", i.Value.TenantID);
                if (!string.IsNullOrEmpty(i.Value.ClientID)) await SetSecretAsync(i.Key + "ClientID", i.Value.ClientID);
                if (!string.IsNullOrEmpty(i.Value.ClientSecret)) await SetSecretAsync(i.Key + "ClientSecret", i.Value.ClientSecret);
                if (!string.IsNullOrEmpty(i.Value.Prefix)) await SetSecretAsync(i.Key + "Prefix", i.Value.Prefix);
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
                item.Value.TenantID = await GetSecret(item.Key + "TenantID");
                item.Value.ClientID = await GetSecret(item.Key + "ClientID");
                item.Value.ClientSecret = await GetSecret(item.Key + "ClientSecret");
                item.Value.Prefix = await GetSecret(item.Key + "Prefix");
            }
        }
    }
}
