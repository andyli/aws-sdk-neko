package aws.transfer;

import aws.s3.*;
using neko.Lib;

class TransferClient {
	public var _handle(default, null):Dynamic;

	public function new(s3client:S3Client):Void {
		_handle = _new(s3client._handle);
	}

	public function uploadFile(fileName:String, bucketName:String, keyName:String, contentType:String):UploadFileRequest {
		var request_handle = TransferClient_UploadFile(
			_handle,
			fileName.haxeToNeko(),
			bucketName.haxeToNeko(),
			keyName.haxeToNeko(),
			contentType.haxeToNeko()
		);
		var r = new UploadFileRequest();
		r._handle = request_handle;
		return r;
	}

	static var _new = Lib.loadLazy("aws", "new_TransferClient", 1);
	static var TransferClient_UploadFile = Lib.loadLazy("aws", "TransferClient_UploadFile", 5);
}