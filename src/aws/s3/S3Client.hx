package aws.s3;

import aws.s3.model.*;

using neko.Lib;

@:allow(aws)
class S3Client {
	var _handle(default, null):Dynamic;

	/**
		@param endpointOverride e.g. "http://localhost:9090" for a local S3-compatible service
		@param useVirtualAddressing set to false to use path-style addressing, which is usually required by local S3-compatible services
	**/
	public function new(region:Region, endpointOverride:String = null, useVirtualAddressing:Bool = true):Void {
		_handle = _new(region.haxeToNeko(), endpointOverride.haxeToNeko(), useVirtualAddressing);
	}

	public function deleteObject(req:DeleteObjectRequest):Void {
		S3Client_DeleteObject(_handle, req._handle);
	}

	static var _new = Lib.loadLazy("aws", "new_S3Client", 3);
	static var S3Client_DeleteObject = Lib.loadLazy("aws", "S3Client_DeleteObject", 2);
}