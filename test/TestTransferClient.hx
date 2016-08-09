import Test.*;

import sys.*;
import sys.io.*;
using StringTools;

import aws.*;
import aws.s3.*;
import aws.s3.model.*;
import aws.transfer.*;

class TestTransferClient extends Test {
	function test_TransferClient_uploadFile():Void {
		var client = new TransferClient(new S3Client(AWS_DEFAULT_REGION));
		var fileName = FileSystem.absolutePath("../CMakeLists.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.uploadFile(fileName, bucketName, keyName, contentType);
		assertTrue(r.waitUntilDone());
		assertTrue(r.isDone());
		assertEquals(null, r.getFailure());
		assertTrue(r.completedSuccessfully());
		assertEquals(FileSystem.stat(fileName).size, r.getFileSize());
	}

	function test_TransferClient_downloadFile():Void {
		var client = new TransferClient(new S3Client(AWS_DEFAULT_REGION));
		var fileName = FileSystem.absolutePath("CMakeLists2.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.downloadFile(fileName, bucketName, keyName);
		assertTrue(r.waitUntilDone());
		assertTrue(r.isDone());
		assertEquals(null, r.getFailure());
		assertTrue(r.completedSuccessfully());
		assertEquals(FileSystem.stat(fileName).size, r.getFileSize());
		assertTrue(FileSystem.exists(fileName));
		FileSystem.deleteFile(fileName);
	}
}