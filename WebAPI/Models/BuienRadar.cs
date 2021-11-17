
using System;

namespace WebAPI.Models
{

    public class Rootobject
    {
        public string id { get; set; }
        public Buienradar buienradar { get; set; }
        public Actual actual { get; set; }
        public Forecast forecast { get; set; }
    }

    public class Buienradar
    {
        public string id { get; set; }
        public string copyright { get; set; }
        public string terms { get; set; }
    }

    public class Actual
    {
        public string id { get; set; }
        public string actualradarurl { get; set; }
        public DateTime sunrise { get; set; }
        public DateTime sunset { get; set; }
        public Stationmeasurement[] stationmeasurements { get; set; }
    }

    public class Stationmeasurement
    {
        public string id { get; set; }
        public int stationid { get; set; }
        public string stationname { get; set; }
        public float lat { get; set; }
        public float lon { get; set; }
        public string regio { get; set; }
        public DateTime timestamp { get; set; }
        public string weatherdescription { get; set; }
        public string iconurl { get; set; }
        public string graphUrl { get; set; }
        public string winddirection { get; set; }
        public float temperature { get; set; }
        public float groundtemperature { get; set; }
        public float feeltemperature { get; set; }
        public float windgusts { get; set; }
        public float windspeed { get; set; }
        public int windspeedBft { get; set; }
        public float humidity { get; set; }
        public float precipitation { get; set; }
        public float sunpower { get; set; }
        public float rainFallLast24Hour { get; set; }
        public float rainFallLastHour { get; set; }
        public int winddirectiondegrees { get; set; }
        public float airpressure { get; set; }
        public float visibility { get; set; }
    }

    public class Forecast
    {
        public string id { get; set; }
        public Weatherreport weatherreport { get; set; }
        public Shortterm shortterm { get; set; }
        public Longterm longterm { get; set; }
        public Fivedayforecast[] fivedayforecast { get; set; }
    }

    public class Weatherreport
    {
        public string id { get; set; }
        public DateTime published { get; set; }
        public string title { get; set; }
        public string summary { get; set; }
        public string text { get; set; }
        public string author { get; set; }
        public string authorbio { get; set; }
    }

    public class Shortterm
    {
        public string id { get; set; }
        public DateTime startdate { get; set; }
        public DateTime enddate { get; set; }
        public string forecast { get; set; }
    }

    public class Longterm
    {
        public string id { get; set; }
        public DateTime startdate { get; set; }
        public DateTime enddate { get; set; }
        public string forecast { get; set; }
    }

    public class Fivedayforecast
    {
        public string id { get; set; }
        public DateTime day { get; set; }
        public string mintemperature { get; set; }
        public string maxtemperature { get; set; }
        public int mintemperatureMax { get; set; }
        public int mintemperatureMin { get; set; }
        public int maxtemperatureMax { get; set; }
        public int maxtemperatureMin { get; set; }
        public int rainChance { get; set; }
        public int sunChance { get; set; }
        public string windDirection { get; set; }
        public int wind { get; set; }
        public float mmRainMin { get; set; }
        public float mmRainMax { get; set; }
        public string weatherdescription { get; set; }
        public string iconurl { get; set; }
    }

}