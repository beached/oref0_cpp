public class Rootobject {
	public Class1[] Property1 { get; set; }
}

public class Class1 {
	public string _id { get; set; }
	public DateTime created_at { get; set; }
	public Pump pump { get; set; }
	public Sensor sensor { get; set; }
	public string device { get; set; }
	public Uploader uploader { get; set; }
	public bool sensorNotActive { get; set; }
	public Loop loop { get; set; }
}

public class Pump {
	public Battery battery { get; set; }
	public float reservoir { get; set; }
	public DateTime clock { get; set; }
	public Iob iob { get; set; }
	public string pumpID { get; set; }
}

public class Battery {
	public int percent { get; set; }
	public string status { get; set; }
	public float voltage { get; set; }
}

public class Iob {
	public float bolusiob { get; set; }
	public DateTime timestamp { get; set; }
	public float iob { get; set; }
}

public class Sensor {
	public int sensorRemaining { get; set; }
	public int sensorAge { get; set; }
}

public class Uploader {
	public int battery { get; set; }
	public DateTime timestamp { get; set; }
	public string name { get; set; }
}

public class Loop {
	public string version { get; set; }
	public Recommendedtempbasal recommendedTempBasal { get; set; }
	public float recommendedBolus { get; set; }
	public DateTime timestamp { get; set; }
	public Predicted predicted { get; set; }
	public Iob1 iob { get; set; }
	public string name { get; set; }
	public string failureReason { get; set; }
	public Enacted enacted { get; set; }
	public Cob cob { get; set; }
}

public class Recommendedtempbasal {
	public DateTime timestamp { get; set; }
	public float rate { get; set; }
	public int duration { get; set; }
}

public class Predicted {
	public DateTime startDate { get; set; }
	public int[] values { get; set; }
}

public class Iob1 {
	public DateTime timestamp { get; set; }
	public float iob { get; set; }
}

public class Enacted {
	public DateTime timestamp { get; set; }
	public float rate { get; set; }
	public int duration { get; set; }
	public bool received { get; set; }
}

public class Cob {
	public DateTime timestamp { get; set; }
	public float cob { get; set; }
}

