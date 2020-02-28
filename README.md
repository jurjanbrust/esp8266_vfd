# esp8266_vfd
Using a Vacuum Fluorescent Display (VFD) with an ESP8266 as information display

See a video on [youtube](https://www.youtube.com/watch?v=3-wzvdbAAj8&list=PL9SWh3l_eGl_Rr5QC0h_TrUXadgcYiZPY&index=2&t=0s)

And for the technical documentation of this VFD click [here](/Assets/VFD.pdf)

Find out more about this project on https://jurjan.info

Picture A | Picture B
---------|----------
 ![vfd](/Assets/IMG_5674.JPG) | ![vfd](/Assets/IMG_4308.JPG)

### Details

The scheme displayed below shows the components involved.
First there is an ESP8266 that is being used as the controller of the VFD. Because the serial port of the VFD is inverted an invertor is needed to invert the bits transmitted on the serial port. I used a sn74hc04n for this, but this is probably a bit of overkill.

For the software part I decided to keep the software on the ESP as simple as possible. So the ESP only has logic to control the display, display the time and do some fancy animation tricks.

The Web-API contains all the logic to gather and prepare the information that needs to be displayed on the VFD.
Currently the WEB-API gathers the following kinds of information: 
* weather forecast
* traffic information

Other possible future things to add are:
* calendar (central family)
* Tesla car status (charge level, location)

![vfd](/Assets/Scheme.png)

When the ESP makes a call to the WEB API (source code is included in this repository) the following JSON is returned:

```
[{"date":"2020-02-09T15:23:25.5169582+00:00","line1":"Weersverwachting","line2":"Wisselvallig met soms regen of enkele buien. De eerste dagen nog tamelijk veel wind. De temperaturen liggen overdag aanvankelijk rond het langjarig gemiddelde, vanaf het einde van de week lopen de temperaturen op.","displayMode":4,"delay":5000}]
```

The returned JSON contains the following elements:
* date: datetime of the information
* line1: first line to display on the VFD
* line2: second line to display on the VFD
* displaymode: the way the information needs to be dislayed on the VFD (see enum in the sourcecode). 
    For example:
    * horizontal scroll
    * vertical scroll
    * knightrider mode. 

If this project help you reduce time to develop, you can give me a cup of coffee :)

# Donation Button
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=FEAWWGSBYLMZL&currency_code=EUR&source=url)
