import haxe.unit.*;

import sys.io.*;

import aws.*;
import aws.s3.*;
import aws.transfer.*;

class Test extends TestCase {
	static public var AWS_DEFAULT_REGION = switch (Sys.getEnv("AWS_DEFAULT_REGION")) {
		case null, "": throw 'AWS_DEFAULT_REGION is not set';
		case v: v;
	};
	static public var S3BUCKET_NAME = switch (Sys.getEnv("S3BUCKET_NAME")) {
		case null, "": throw 'S3BUCKET_NAME is not set';
		case v: v;
	};

	function test_S3Client():Void {
		var client = new S3Client(AWS_DEFAULT_REGION);
		assertTrue(true);
	}

	function test_TransferClient_uploadFile():Void {
		var client = new TransferClient(new S3Client(AWS_DEFAULT_REGION));
		var fileName = sys.FileSystem.absolutePath("../CMakeLists.txt");
		var bucketName = S3BUCKET_NAME;
		var keyName = "CMakeLists.txt";
		var contentType = "application/octet-stream";
		var r = client.uploadFile(fileName, bucketName, keyName, contentType);
		while (!r.isDone()) {
			Sys.sleep(0.5);
		}
		assertEquals(null, r.getFailure());
	}

	static function uploadNdll():Void {
		var fileName = sys.FileSystem.absolutePath("../bin/aws.ndll");
		var sha = {
			var p = new Process("git", ["rev-parse", "--short", "HEAD"]);
			var out = p.stdout.readAll().toString();
			p.close();
			out;
		}
		var date = {
			var p = new Process("git", ["show", "-s", "--format=%ct", "HEAD"]);
			var out = p.stdout.readAll().toString();
			p.close();
			datetime.DateTime.fromTime(Std.parseFloat(out));
		}
		var dateString = date.format("%Y%m%d%H%M%S");
		var s64 = switch (Sys.systemName()) {
			case "Windows": "";
			case _:
				var p = new Process("file", [fileName]);
				var out = p.stdout.readAll().toString();
				p.close();
				if (out.indexOf("x86_64") > -1)
					"64";
				else
					"";
		}
		var platform = switch (Sys.systemName()) {
			case "Windows": "Windows";
			case "Mac": "Mac" + s64;
			case "Linux": "Linux" + s64;
			case systemName: throw 'unknown systemName: $systemName';
		}
		var keyName = 'artifacts/${dateString}_${sha}/ndll/${platform}/aws.ndll';

		var client = new TransferClient(new S3Client(AWS_DEFAULT_REGION));
		var bucketName = S3BUCKET_NAME;
		var contentType = "application/octet-stream";
		var r = client.uploadFile(fileName, bucketName, keyName, contentType);
		while (!r.isDone()) {
			Sys.sleep(0.5);
		}
		switch (r.getFailure()) {
			case null:
				Sys.println('uploaded ${fileName}');
			case f:
				throw f;
		}
	}

	static function main():Void {
		var runner = new TestRunner();
		runner.add(new Test());
		Aws.initAPI();
		var succeeded = runner.run();
		if (!succeeded) {
			Aws.shutdownAPI();
			Sys.exit(1);
		} else {
			uploadNdll();
			Aws.shutdownAPI();
		}
	}
}
