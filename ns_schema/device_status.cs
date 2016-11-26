class device_status_t {
	std::string _id;
	DateTime created_at;
	Pump pump;
	Sensor sensor;
	std::string device;
	Uploader uploader;
	bool sensorNotActive;
	Loop loop;
}

class Pump {
	Battery battery;
	double reservoir;
	DateTime clock;
	Iob iob;
	std::string pumpID;
}

class Battery {
	int percent;
	std::string status;
	double voltage;
}

class Iob {
	double bolusiob;
	DateTime timestamp;
	double iob;
}

class Sensor {
	int sensorRemaining;
	int sensorAge;
}

class Uploader {
	int battery;
	DateTime timestamp;
	std::string name;
}

class Loop {
	std::string version;
	Recommendedtempbasal recommendedTempBasal;
	double recommendedBolus;
	DateTime timestamp;
	Predicted predicted;
	Iob1 iob;
	std::string name;
	std::string failureReason;
	Enacted enacted;
	Cob cob;
}

class Recommendedtempbasal {
	DateTime timestamp;
	double rate;
	int duration;
}

class Predicted {
	DateTime startDate;
	std::vector<int> values;
}

class Iob1 {
	DateTime timestamp;
	double iob;
}

class Enacted {
	DateTime timestamp;
	double rate;
	int duration;
	bool received;
}

class Cob {
	DateTime timestamp;
	double cob;
}

