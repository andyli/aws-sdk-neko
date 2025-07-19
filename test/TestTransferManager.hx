import Test.*;

import sys.*;
using StringTools;

import aws.s3.*;
import aws.transfer.*;

class TestTransferManager extends Test {
	function test_TransferManager_uploadFile():Void {
		var client = new TransferManager(new S3Client(AWS_DEFAULT_REGION));
		var fileName = FileSystem.absolutePath("../CMakeLists.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.uploadFile(fileName, bucketName, keyName, contentType);
		r.waitUntilFinished();
		assertEquals(null, r.getLastErrorMessage());
		assertTrue(r.getStatus() == COMPLETED);
		assertEquals(FileSystem.stat(fileName).size, r.getBytesTotalSize());
	}

	function test_TransferManager_downloadFile():Void {
		var client = new TransferManager(new S3Client(AWS_DEFAULT_REGION));
		var fileName = FileSystem.absolutePath("CMakeLists2.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.downloadFile(fileName, bucketName, keyName);
		r.waitUntilFinished();
		assertEquals(null, r.getLastErrorMessage());
		assertTrue(r.getStatus() == COMPLETED);
		assertEquals(FileSystem.stat(fileName).size, r.getBytesTotalSize());
		assertTrue(FileSystem.exists(fileName));
		FileSystem.deleteFile(fileName);
	}
}
