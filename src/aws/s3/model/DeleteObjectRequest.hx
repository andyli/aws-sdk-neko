package aws.s3.model;

using neko.Lib;

@:allow(aws)
class DeleteObjectRequest {
	var _handle(default, null):Dynamic;

	public function new():Void {
		_handle = _new();
	}

	public function setBucket(value:String):Void {
		DeleteObjectRequest_SetBucket(_handle, value.haxeToNeko());
	}

	public function setKey(value:String):Void {
		DeleteObjectRequest_SetKey(_handle, value.haxeToNeko());
	}

	static var _new = Lib.loadLazy("aws", "new_DeleteObjectRequest", 0);
	static var DeleteObjectRequest_SetBucket = Lib.loadLazy("aws", "DeleteObjectRequest_SetBucket", 2);
	static var DeleteObjectRequest_SetKey = Lib.loadLazy("aws", "DeleteObjectRequest_SetKey", 2);
}