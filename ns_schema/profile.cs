public class Class1 {
	public string _id { get; set; }
	public string defaultProfile { get; set; }
	public Store store { get; set; }
	public DateTime startDate { get; set; }
	public string mills { get; set; }
	public DateTime created_at { get; set; }
	public string dia { get; set; }
	public Carbratio1[] carbratio { get; set; }
	public string carbs_hr { get; set; }
	public string delay { get; set; }
	public Sen1[] sens { get; set; }
	public string timezone { get; set; }
	public Basal1[] basal { get; set; }
	public Target_Low1[] target_low { get; set; }
	public Target_High1[] target_high { get; set; }
	public string units { get; set; }
}

public class Store {
	public Default Default { get; set; }
}

public class Default {
	public string dia { get; set; }
	public Carbratio[] carbratio { get; set; }
	public string carbs_hr { get; set; }
	public string delay { get; set; }
	public Sen[] sens { get; set; }
	public string timezone { get; set; }
	public Basal[] basal { get; set; }
	public Target_Low[] target_low { get; set; }
	public Target_High[] target_high { get; set; }
	public DateTime startDate { get; set; }
	public string units { get; set; }
}

public class Carbratio {
	public string time { get; set; }
	public string value { get; set; }
	public string timeAsSeconds { get; set; }
}

public class Sen {
	public string time { get; set; }
	public string value { get; set; }
	public string timeAsSeconds { get; set; }
}

public class Basal {
	public string time { get; set; }
	public string value { get; set; }
	public string timeAsSeconds { get; set; }
}

public class Target_Low {
	public string time { get; set; }
	public string value { get; set; }
	public string timeAsSeconds { get; set; }
}

public class Target_High {
	public string time { get; set; }
	public string value { get; set; }
	public string timeAsSeconds { get; set; }
}

public class Carbratio1 {
	public string time { get; set; }
	public string value { get; set; }
}

public class Sen1 {
	public string time { get; set; }
	public string value { get; set; }
}

public class Basal1 {
	public string time { get; set; }
	public string value { get; set; }
}

public class Target_Low1 {
	public string time { get; set; }
	public string value { get; set; }
}

public class Target_High1 {
	public string time { get; set; }
	public string value { get; set; }
}

