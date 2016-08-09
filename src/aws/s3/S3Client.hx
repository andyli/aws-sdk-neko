package aws.s3;

import aws.s3.model.*;

using neko.Lib;

@:allow(aws)
class S3Client {
	var _handle(default, null):Dynamic;

	public function new(region:Region):Void {
		_handle = _new(region.haxeToNeko());
	}

	public function deleteObject(req:DeleteObjectRequest):Void {
		S3Client_DeleteObject(_handle, req._handle);
	}

	static var _new = Lib.loadLazy("aws", "new_S3Client", 1);
	static var S3Client_DeleteObject = Lib.loadLazy("aws", "S3Client_DeleteObject", 2);
}