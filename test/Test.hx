import haxe.unit.*;
using neko.Lib;

import aws.*;
import aws.s3.*;
import aws.transfer.*;

class Test extends TestCase {
	public var AWS_DEFAULT_REGION = switch (Sys.getEnv("AWS_DEFAULT_REGION")) {
		case null, "": throw 'AWS_DEFAULT_REGION is not set';
		case v: v;
	};
	public var S3BUCKET_NAME = switch (Sys.getEnv("S3BUCKET_NAME")) {
		case null, "": throw 'S3BUCKET_NAME is not set';
		case v: v;
	};

	override function setup():Void {
		Aws.initAPI();
	}

	override function tearDown():Void {
		Aws.shutdownAPI();
	}

	function test_S3Client():Void {
		var client = new S3Client(AWS_DEFAULT_REGION);
		assertTrue(true);
	}

	function test_TransferClient_uploadFile():Void {
		var client = new TransferClient(new S3Client(AWS_DEFAULT_REGION));
		var fileName = sys.FileSystem.fullPath("../CMakeLists.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.uploadFile(fileName, bucketName, keyName, contentType);
		while (!r.isDone()) {
			Sys.sleep(0.5);
		}
		assertEquals(null, r.getFailure());
	}

	static function main():Void {
		var runner = new TestRunner();
		runner.add(new Test());
		var succeeded = runner.run();
		if (!succeeded) {
			Sys.exit(1);
		}
	}
}