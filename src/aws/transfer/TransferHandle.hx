package aws.transfer;

using neko.Lib;

enum abstract TransferStatus(Int) {
	final EXACT_OBJECT_ALREADY_EXISTS;
	final NOT_STARTED;
	final IN_PROGRESS;
	final CANCELED;
	final FAILED;
	final COMPLETED;
	final ABORTED;
}

@:allow(aws)
class TransferHandle {
	var _handle(default, null):Dynamic;

	function new():Void {}

	public function getStatus():TransferStatus {
		return TransferHandle_GetStatus(_handle);
	}

	public function waitUntilFinished():Void {
		TransferHandle_WaitUntilFinished(_handle);
	}

	public function getLastErrorMessage():Null<String> {
		return switch (Lib.nekoToHaxe(TransferHandle_GetLastErrorMessage(_handle))) {
			case "": null;
			case f: f;
		};
	}

	public function getBytesTotalSize():Int {
		return TransferHandle_GetBytesTotalSize(_handle);
	}

	static var TransferHandle_WaitUntilFinished = Lib.loadLazy("aws", "TransferHandle_WaitUntilFinished", 1);
	static var TransferHandle_GetStatus = Lib.loadLazy("aws", "TransferHandle_GetStatus", 1);
	static var TransferHandle_GetLastErrorMessage = Lib.loadLazy("aws", "TransferHandle_GetLastErrorMessage", 1);
	static var TransferHandle_GetBytesTotalSize = Lib.loadLazy("aws", "TransferHandle_GetBytesTotalSize", 1);
}
