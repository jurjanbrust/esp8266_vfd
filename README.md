# esp8266_vfd

Using a Vacuum Fluorescent Display (VFD) with an ESP8266 as information display

The displayed VFD is the IEE 03602-122-09220, 20x2, Dot Matrix ASCII VFD module.

See a video on [youtube](https://www.youtube.com/watch?v=3-wzvdbAAj8&list=PL9SWh3l_eGl_Rr5QC0h_TrUXadgcYiZPY&index=2&t=0s)

And for the technical documentation of this VFD click [here](/Assets/VFD.pdf)

Find out more about this project on https://jurjan.info

| Picture A                    | Picture B                    |
| ---------------------------- | ---------------------------- |
| ![vfd](/Assets/IMG_5674.JPG) | ![vfd](/Assets/IMG_4308.JPG) |

### Details

The scheme displayed below shows the components involved.

![vfd](/Assets/Scheme.png)

First there is an ESP8266 that is being used as the controller of the VFD. Because the serial port of the VFD is inverted an invertor is needed to invert the bits transmitted on the serial port. I used a sn74hc04n for this, but this is probably a bit of overkill and a transistor should also do, like the picture below.

![](/Assets/2022-04-25-15-51-49.png)

For the software part I decided to keep the software on the ESP as simple as possible. So the ESP only has logic to control the display, display the time and do some fancy animation tricks.

The Web-API contains all the logic to gather and prepare the information that needs to be displayed on the VFD.

Currently the WEB-API gathers the following kinds of information:

- weather forecast
- news feeds from various sites
- traffic information
- calendar (central family)
- fitbit (weight goals of me and my partner)

Other possible future things to add are:

- Tesla car status (charge level, location)
- Spotify integration + VU meter

![](/Assets/2022-04-25-16-20-58.png)

When the ESP makes a call to the WEB API (source code is included in this repository) the following JSON is returned:

```
[{"date":"2020-02-09T15:23:25.5169582+00:00","line1":"Weersverwachting","line2":"Wisselvallig met soms regen of enkele buien. De eerste dagen nog tamelijk veel wind. De temperaturen liggen overdag aanvankelijk rond het langjarig gemiddelde, vanaf het einde van de week lopen de temperaturen op.","displayMode":4,"delay":5000}]
```

The returned JSON contains the following elements:

- date: datetime of the information
- line1: first line to display on the VFD
- line2: second line to display on the VFD
- displaymode: the way the information needs to be displayed on the VFD (see enum in the sourcecode).
  For example:
  - horizontal scroll
  - vertical scroll
  - knightrider mode.

# Authentication (WebAPI to MS Graph)

The WebAPI uses the OAuth 2.0 for both the Graph API and Fitbit API.
For the Graph API the following steps are needed:

1. Make an App-Registration in your Azure AD and select for '**Supported account-types**' : '**Accounts in any organizational directory (Any Azure AD directory - Multitenant) and personal Microsoft accounts (e.g. Skype, Xbox)**'
2. Select 'Authentication' in the menu and '**Add a platform**'
3. Select '**Web**' as the application
4. Enter 'http://localhost' as the Redirect URIs
5. Select 'Certificates & secrets'
6. Add a new client secret (description = vfd, exires = [what you want])
7. Copy the 'Value' of the secret and keep it for later use
8. Open 'Overview' and take a note of the 'Application (client) ID' for later use
9. Open [Auth-Code-Flow](https://docs[[.microsoft.com/en-us/azure/active-directory/develop/v2-oauth2-auth-code-flow) and click in the ['Run in Postman'](https://app.getpostman.com/run-collection/f77994d794bab767596d)
10. This will import the following collection into [Postman](https://www.postman.com/downloads/):

![](/Assets/2022-04-25-10-44-30.png)

12. Open the '**Authorize Request**' and change the '**client_id**' with the one you copied in the 'Overview' page
13. Change '**redirect_url**' to http://localhost/
14. Change '**scope**' to **openid%20offline_access%20User.Read%20User.Read.All%20Calendars.Read%20Calendars.Read.Shared**
15. Now we are ready to get the auth-code. Copy the URL generated by Postman (left of the Send button and paste it into a browser). Note that we cannot use postman for this step, because we need a browser.

![](/Assets/2022-04-25-10-57-27.png)

16. Once you authorized the application you should see a auth-code in the URL like: http://localhost/?code=M.R3_ABC.a1234vcde-1aaa2-11aa-aaaa-aa11112aa1111&state=12345
17. Copy this code for the next request 'Token Request - Auth Code'
18. Change the body of the request with the correct '**client_id**', ' **redirect_url**', and '**code**'. Deselect '**scope**'.

![](/Assets/2022-04-25-11-06-19.png)

19. This will return an '**access_token**' and a '**refresh_token**'
20. Copy these values into the WebAPI/Services/Agenda.cs and follow the instructions (Or create the secrets in keyvault manually)

The WebAPI will store and update the tokens into AzureKeyVault on its own.
Another usefull collection of Postman request to get a look at is: [GraphAPI calls](https://www.postman.com/microsoftgraph/workspace/microsoft-graph/collection/455214-085f7047-1bec-4570-9ed0-3a7253be148c/fork)

# Resource group

A screenshot of my resourcegroup

![](/Assets/2022-04-25-16-24-11.png)

# Keyvault

The keyvault should contain the following secrets for the Graph integration to work

![](/Assets/2022-04-25-16-05-57.png)

Since the WebAPI needs to access these secrets, the WebAPI should be added to the 'Access Policies' of the keyvault. The WebAPI will have a system assigned managed identity (if not enable it in the Identity settings
for the App service of your WebAPI)

# App service (Web API)

I set the App Service Plan to a F1:free plan.
Make sure that the application settings contains the 'VaultUri' setting with a value to your keyvault 'https://yourkeyvault-prod.vault.azure.net'

No other configurations are needed

# Ideas to add:

Tech news:

- Techcrunch
- Engadget
- CNET
- The Verge
- Ars Technica
- Wited
- Gizmodo

Other news:

- CNN, BBC, New York Times, Washington Post
- Scientific American

# Donation Button

Well thats it!
If this project help you reduce time to develop, you can give me a cup of coffee :)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=FEAWWGSBYLMZL&currency_code=EUR&source=url)
