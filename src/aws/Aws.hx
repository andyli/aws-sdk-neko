package aws;

import neko.Lib;

class Aws {
	static public function initAPI():Void {
		_InitAPI();
	}
	static public function shutdownAPI():Void {
		_ShutdownAPI();
	}

	static var _InitAPI = Lib.load("aws", "_InitAPI", 0);
	static var _ShutdownAPI = Lib.load("aws", "_ShutdownAPI", 0);
}