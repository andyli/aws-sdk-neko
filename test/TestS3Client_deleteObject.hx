import Test.*;

import sys.*;
import sys.io.*;
using StringTools;

import aws.*;
import aws.s3.*;
import aws.s3.model.*;
import aws.transfer.*;

class TestS3Client_deleteObject extends Test {
	function test_DeleteObjectRequest():Void {
		var req = new DeleteObjectRequest();
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		req.setBucket(bucketName);
		req.setKey(keyName);
		assertTrue(true);
	}

	function test_S3Client_deleteObject():Void {
		var req = new DeleteObjectRequest();
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		req.setBucket(bucketName);
		req.setKey(keyName);
		var client = new S3Client(AWS_DEFAULT_REGION);
		client.deleteObject(req);
		assertTrue(true);
	}

	function test_S3Client_deleteObjectFail():Void {
		var req = new DeleteObjectRequest();
		var bucketName = "asdfas";
		var keyName = "asdgsag";
		req.setBucket(bucketName);
		req.setKey(keyName);
		var client = new S3Client(AWS_DEFAULT_REGION);
		var error = try {
			client.deleteObject(req);
			null;
		} catch(e:Dynamic) {
			e;
		}
		assertTrue(error != null);
	}
}