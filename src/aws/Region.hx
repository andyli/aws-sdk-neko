package aws;

@:enum abstract Region(String) from String to String {
	var US_EAST_1 = "us-east-1";
	var US_WEST_1 = "us-west-1";
	var US_WEST_2 = "us-west-2";
	var EU_WEST_1 = "eu-west-1";
	var EU_CENTRAL_1 = "eu-central-1";
	var AP_SOUTHEAST_1 = "ap-southeast-1";
	var AP_SOUTHEAST_2 = "ap-southeast-2";
	var AP_NORTHEAST_1 = "ap-northeast-1";
	var AP_NORTHEAST_2 = "ap-northeast-2";
	var SA_EAST_1 = "sa-east-1";
	var AP_SOUTH_1 = "ap-south-1";
}