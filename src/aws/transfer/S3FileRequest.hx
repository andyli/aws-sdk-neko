package aws.transfer;

using neko.Lib;

@:allow(aws)
class S3FileRequest {
	public var _handle(default, null):Dynamic;

	function new():Void {}

	public function isDone():Bool {
		return S3FileRequest_IsDone(_handle) > 0;
	}

	public function completedSuccessfully():Bool {
		return S3FileRequest_CompletedSuccessfully(_handle) > 0;
	}

	public function waitUntilDone():Bool {
		return S3FileRequest_WaitUntilDone(_handle) > 0;
	}

	public function getFailure():Null<String> {
		return switch (Lib.nekoToHaxe(S3FileRequest_GetFailure(_handle))) {
			case "": null;
			case f: f;
		};
	}

	public function getFileSize():Int {
		return S3FileRequest_GetFileSize(_handle);
	}

	static var S3FileRequest_IsDone = Lib.loadLazy("aws", "S3FileRequest_IsDone", 1);
	static var S3FileRequest_WaitUntilDone = Lib.loadLazy("aws", "S3FileRequest_WaitUntilDone", 1);
	static var S3FileRequest_CompletedSuccessfully = Lib.loadLazy("aws", "S3FileRequest_CompletedSuccessfully", 1);
	static var S3FileRequest_GetFailure = Lib.loadLazy("aws", "S3FileRequest_GetFailure", 1);
	static var S3FileRequest_GetFileSize = Lib.loadLazy("aws", "S3FileRequest_GetFileSize", 1);
}