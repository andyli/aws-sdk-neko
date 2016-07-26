package aws.s3;

using neko.Lib;

class S3Client {
	public var _handle(default, null):Dynamic;
	public function new(region:Region):Void {
		_handle = _new(region.haxeToNeko());
	}

	static var _new = Lib.load("aws", "new_S3Client", 1);
}