using Newtonsoft.Json;
using System;
using System.Collections.Generic;

public class RootObject_Todo<T>
{
    [JsonProperty("@odata.context")]
    public string odatacontext { get; set; }
    public List<T> value { get; set; }
}

public class TodoList
{
    [JsonProperty("@odata.etag")]
    public string odataetag { get; set; }
    public string displayName { get; set; }
    public bool isOwner { get; set; }
    public bool isShared { get; set; }
    public string wellknownListName { get; set; }
    public string id { get; set; }
}

public class TodoBody
{
    public string content { get; set; }
    public string contentType { get; set; }
}

public class TodoItem
{
    [JsonProperty("@odata.etag")]
    public string odataetag { get; set; }
    public string importance { get; set; }
    public bool isReminderOn { get; set; }
    public string status { get; set; }
    public string title { get; set; }
    public DateTime createdDateTime { get; set; }
    public DateTime lastModifiedDateTime { get; set; }
    public bool hasAttachments { get; set; }
    public List<object> categories { get; set; }
    public string id { get; set; }
    public Body body { get; set; }
}

