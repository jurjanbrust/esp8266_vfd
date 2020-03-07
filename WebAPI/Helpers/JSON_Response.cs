
using System;

public class Rootobject_Calendar
{
    public Value[] value { get; set; }
}

public class Value
{
    public string Id { get; set; }
    public DateTime CreatedDateTime { get; set; }
    public DateTime LastModifiedDateTime { get; set; }
    public string ChangeKey { get; set; }
    public object[] Categories { get; set; }
    public string OriginalStartTimeZone { get; set; }
    public string OriginalEndTimeZone { get; set; }
    public Responsestatus ResponseStatus { get; set; }
    public string iCalUId { get; set; }
    public int ReminderMinutesBeforeStart { get; set; }
    public bool IsReminderOn { get; set; }
    public bool HasAttachments { get; set; }
    public string Subject { get; set; }
    public Body Body { get; set; }
    public string BodyPreview { get; set; }
    public string Importance { get; set; }
    public string Sensitivity { get; set; }
    public Start Start { get; set; }
    public End End { get; set; }
    public Location Location { get; set; }
    public bool IsAllDay { get; set; }
    public bool IsCancelled { get; set; }
    public bool IsOrganizer { get; set; }
    public object Recurrence { get; set; }
    public bool ResponseRequested { get; set; }
    public string SeriesMasterId { get; set; }
    public string ShowAs { get; set; }
    public string Type { get; set; }
    public object[] Attendees { get; set; }
    public Organizer Organizer { get; set; }
    public string WebLink { get; set; }
    public object OnlineMeetingUrl { get; set; }
}

public class Responsestatus
{
    public string Response { get; set; }
    public DateTime Time { get; set; }
}

public class Body
{
    public string ContentType { get; set; }
    public string Content { get; set; }
}

public class Start
{
    public DateTime DateTime { get; set; }
    public string TimeZone { get; set; }
}

public class End
{
    public DateTime DateTime { get; set; }
    public string TimeZone { get; set; }
}

public class Location
{
    public string DisplayName { get; set; }
    public Address Address { get; set; }
    public Coordinates Coordinates { get; set; }
}

public class Address
{
}

public class Coordinates
{
}

public class Organizer
{
    public Emailaddress EmailAddress { get; set; }
}

public class Emailaddress
{
    public string Name { get; set; }
    public string Address { get; set; }
}
