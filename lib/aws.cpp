#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/transfer/TransferManager.h>
#include <aws/transfer/TransferHandle.h>

#include <neko.h>

using namespace Aws::Client;
using namespace Aws::Transfer;
using namespace Aws::S3;
using namespace Aws::S3::Model;

static const char* ALLOCATION_TAG = "aws-skd-neko";
static Aws::SDKOptions options;

static bool awsInited = false;
static bool awsShutdowned = false;

static value _InitAPI() {
	if (!awsInited) {
		Aws::InitAPI(options);
		awsInited = true;
	}
	return val_null;
}
DEFINE_PRIM(_InitAPI, 0);

static value _ShutdownAPI() {
	if (awsInited && !awsShutdowned) {
		Aws::ShutdownAPI(options);
		awsShutdowned = true;
	}
	return val_null;
}
DEFINE_PRIM(_ShutdownAPI, 0);

// S3Client

DEFINE_KIND(k_S3Client);

static void free_S3Client( value s3client ) {
	auto _s3client = (std::shared_ptr<S3Client>*) val_data(s3client);
	Aws::Delete(_s3client);
}

static value new_S3Client(value region, value endpoint) {
	ClientConfiguration clientConfig;
	clientConfig.followRedirects = FollowRedirectsPolicy::ALWAYS;
	clientConfig.endpointOverride = val_string(endpoint);
	clientConfig.region = val_string(region);
	clientConfig.connectTimeoutMs = 5000;
	clientConfig.requestTimeoutMs = 6000;

	auto c = Aws::New<std::shared_ptr<S3Client>>(ALLOCATION_TAG, Aws::New<S3Client>(ALLOCATION_TAG, clientConfig));
	auto handle = alloc_abstract(k_S3Client, c);
	val_gc(handle, free_S3Client);
	return handle;
}
DEFINE_PRIM(new_S3Client, 2);


// TransferManager

DEFINE_KIND(k_TransferManager);

static void free_TransferManager( value s3client ) {
	auto _s3client = (std::shared_ptr<TransferManager>*) val_data(s3client);
	Aws::Delete(_s3client);
}

static value new_TransferManager(value s3client) {
	TransferManagerConfiguration transferManagerConfig(nullptr);

	auto _s3client = * (std::shared_ptr<S3Client>*) val_data(s3client);
	transferManagerConfig.s3Client = _s3client;
	transferManagerConfig.spExecutor = Aws::MakeShared<Aws::Utils::Threading::DefaultExecutor>(ALLOCATION_TAG);

	auto c = Aws::New<std::shared_ptr<TransferManager>>(ALLOCATION_TAG, TransferManager::Create(transferManagerConfig));
	auto handle = alloc_abstract(k_TransferManager, c);
	val_gc(handle, free_TransferManager);
	return handle;
}
DEFINE_PRIM(new_TransferManager, 1);


// TransferHandle

DEFINE_KIND(k_TransferHandle);

static void free_UploadFileRequest(value uploadFileRequest) {
	auto _UploadFileRequest = (std::shared_ptr<TransferHandle>*) val_data(uploadFileRequest);
	Aws::Delete(_UploadFileRequest);
}

static void free_DownloadFileRequest(value downloadFileRequest) {
	auto _DownloadFileRequest = (std::shared_ptr<TransferHandle>*) val_data(downloadFileRequest);
	Aws::Delete(_DownloadFileRequest);
}

static value TransferHandle_GetBytesTotalSize(value transferHandle) {
	auto _transferHandle = *(std::shared_ptr<TransferHandle>*) val_data(transferHandle);
	auto _GetBytesTotalSize = _transferHandle->GetBytesTotalSize();
	return alloc_int(_GetBytesTotalSize);
}
DEFINE_PRIM(TransferHandle_GetBytesTotalSize, 1);

static value TransferHandle_GetStatus(value transferHandle) {
	auto _transferHandle = * (std::shared_ptr<TransferHandle>*) val_data(transferHandle);
	return alloc_int(_transferHandle->GetStatus());
}
DEFINE_PRIM(TransferHandle_GetStatus, 1);

static value TransferHandle_WaitUntilFinished(value transferHandle) {
	auto _transferHandle = * (std::shared_ptr<TransferHandle>*) val_data(transferHandle);
	_transferHandle->WaitUntilFinished();
	return val_null;
}
DEFINE_PRIM(TransferHandle_WaitUntilFinished, 1);

static value TransferHandle_GetLastErrorMessage(value transferHandle) {
	auto _uploadFileRequest = * (std::shared_ptr<TransferHandle>*) val_data(transferHandle);
	auto _failure = _uploadFileRequest->GetLastError().GetMessage();
	return alloc_string(_failure.c_str());
}
DEFINE_PRIM(TransferHandle_GetLastErrorMessage, 1);

static value TransferManager_UploadFile(
	value transferManager,
	value fileName,
	value bucketName,
	value keyName,
	value contentType
) {
	auto _transferManager = * (std::shared_ptr<TransferManager>*) val_data(transferManager);
	auto _uploadFileRequest = Aws::New<std::shared_ptr<TransferHandle>>(
		ALLOCATION_TAG,
		_transferManager->UploadFile(
			val_string(fileName),
			val_string(bucketName),
			val_string(keyName),
			val_string(contentType),
			{}
		)
	);

	auto r = alloc_abstract(k_TransferHandle, _uploadFileRequest);
	val_gc(r, free_UploadFileRequest);
	return r;
}
DEFINE_PRIM(TransferManager_UploadFile, 5);

static value TransferManager_DownloadFile(
	value transferManager,
	value fileName,
	value bucketName,
	value keyName
) {
	auto _transferManager = *(std::shared_ptr<TransferManager>*) val_data(transferManager);
	auto _downloadFileRequest = Aws::New<std::shared_ptr<TransferHandle>>(
		ALLOCATION_TAG,
		_transferManager->DownloadFile(
			val_string(fileName),
			val_string(bucketName),
			val_string(keyName)
		)
	);

	auto r = alloc_abstract(k_TransferHandle, _downloadFileRequest);
	val_gc(r, free_DownloadFileRequest);
	return r;
}
DEFINE_PRIM(TransferManager_DownloadFile, 4);


// DeleteObjectRequest

DEFINE_KIND(k_DeleteObjectRequest);

static void free_DeleteObjectRequest( value deleteObjectRequest ) {
	auto _deleteObjectRequest = (std::shared_ptr<DeleteObjectRequest>*) val_data(deleteObjectRequest);
	Aws::Delete(_deleteObjectRequest);
}

static value new_DeleteObjectRequest() {
	auto r = Aws::New<std::shared_ptr<DeleteObjectRequest>>(ALLOCATION_TAG, Aws::New<DeleteObjectRequest>(ALLOCATION_TAG));
	auto handle = alloc_abstract(k_DeleteObjectRequest, r);
	val_gc(handle, free_DeleteObjectRequest);
	return handle;
}
DEFINE_PRIM(new_DeleteObjectRequest, 0);

static value DeleteObjectRequest_SetBucket(value deleteObjectRequest, value val) {
	auto _deleteObjectRequest = * (std::shared_ptr<DeleteObjectRequest>*) val_data(deleteObjectRequest);
	_deleteObjectRequest->SetBucket(val_string(val));
	return val_null;
}
DEFINE_PRIM(DeleteObjectRequest_SetBucket, 2);

static value DeleteObjectRequest_SetKey(value deleteObjectRequest, value val) {
	auto _deleteObjectRequest = * (std::shared_ptr<DeleteObjectRequest>*) val_data(deleteObjectRequest);
	_deleteObjectRequest->SetKey(val_string(val));
	return val_null;
}
DEFINE_PRIM(DeleteObjectRequest_SetKey, 2);

static value S3Client_DeleteObject(value s3Client, value deleteObjectRequest) {
	auto _s3Client = * (std::shared_ptr<S3Client>*) val_data(s3Client);
	auto _deleteObjectRequest = * (std::shared_ptr<DeleteObjectRequest>*) val_data(deleteObjectRequest);
	auto _deleteObjectOutcome = _s3Client->DeleteObject(*_deleteObjectRequest);
	if (_deleteObjectOutcome.IsSuccess()) {
		return val_null;
	} else {
		neko_error();
		return val_null;
	}
}
DEFINE_PRIM(S3Client_DeleteObject, 2);
