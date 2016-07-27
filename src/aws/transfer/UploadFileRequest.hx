package aws.transfer;

using neko.Lib;

@:allow(aws)
class UploadFileRequest {
	public var _handle(default, null):Dynamic;

	function new():Void {}

	public function isDone():Bool {
		return UploadFileRequest_IsDone(_handle);
	}

	public function getFailure():Null<String> {
		return UploadFileRequest_GetFailure(_handle);
	}

	static var UploadFileRequest_IsDone = Lib.loadLazy("aws", "UploadFileRequest_IsDone", 1);
	static var UploadFileRequest_GetFailure = Lib.loadLazy("aws", "UploadFileRequest_GetFailure", 1);
}