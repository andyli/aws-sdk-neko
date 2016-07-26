import haxe.unit.*;
using neko.Lib;

import aws.*;
import aws.s3.*;
import aws.transfer.*;

class Test extends TestCase {
	override function setup():Void {
		Aws.initAPI();
	}

	override function tearDown():Void {
		Aws.shutdownAPI();
	}

	function test_S3Client():Void {
		var client = new S3Client(EU_WEST_1);
		assertTrue(true);
	}

	function test_TransferClient_uploadFile():Void {
		var client = new TransferClient(new S3Client(EU_WEST_1));
		var fileName = sys.FileSystem.fullPath("../CMakeLists.txt");
		var bucketName = "lib.haxe.org";
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