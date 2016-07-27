package aws;

import neko.Lib;

class Aws {
	static public function initAPI():Void {
		_InitAPI();
	}
	static public function shutdownAPI():Void {
		_ShutdownAPI();
	}

	static var _InitAPI = Lib.loadLazy("aws", "_InitAPI", 0);
	static var _ShutdownAPI = Lib.loadLazy("aws", "_ShutdownAPI", 0);
}