import Test.*;

import sys.*;
using StringTools;

import aws.s3.*;
import aws.transfer.*;

class TestTransferManager extends Test {
	function test_TransferManager_uploadFile():Void {
		var client = new TransferManager(createS3Client());
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
		var client = new TransferManager(createS3Client());
		var fileName = FileSystem.absolutePath("CMakeLists2.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		// make sure the object exists, since test methods are not run in a guaranteed order
		var u = client.uploadFile(FileSystem.absolutePath("../CMakeLists.txt"), bucketName, keyName, contentType);
		u.waitUntilFinished();
		assertEquals(null, u.getLastErrorMessage());
		var r = client.downloadFile(fileName, bucketName, keyName);
		r.waitUntilFinished();
		assertEquals(null, r.getLastErrorMessage());
		assertTrue(r.getStatus() == COMPLETED);
		assertEquals(FileSystem.stat(fileName).size, r.getBytesTotalSize());
		assertTrue(FileSystem.exists(fileName));
		FileSystem.deleteFile(fileName);
	}
}
