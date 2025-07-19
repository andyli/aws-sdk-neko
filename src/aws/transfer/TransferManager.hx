package aws.transfer;

import aws.s3.*;

using neko.Lib;

class TransferManager {
	var _handle(default, null):Dynamic;

	public function new(s3client:S3Client):Void {
		_handle = _new(s3client._handle);
	}

	public function uploadFile(fileName:String, bucketName:String, keyName:String, contentType:String):TransferHandle {
		var request_handle = TransferManager_UploadFile(_handle, fileName.haxeToNeko(), bucketName.haxeToNeko(), keyName.haxeToNeko(), contentType.haxeToNeko());
		var r = new TransferHandle();
		r._handle = request_handle;
		return r;
	}

	public function downloadFile(fileName:String, bucketName:String, keyName:String):TransferHandle {
		var request_handle = TransferManager_DownloadFile(_handle, fileName.haxeToNeko(), bucketName.haxeToNeko(), keyName.haxeToNeko());
		var r = new TransferHandle();
		r._handle = request_handle;
		return r;
	}

	static var _new = Lib.loadLazy("aws", "new_TransferManager", 1);
	static var TransferManager_UploadFile = Lib.loadLazy("aws", "TransferManager_UploadFile", 5);
	static var TransferManager_DownloadFile = Lib.loadLazy("aws", "TransferManager_DownloadFile", 4);
}
