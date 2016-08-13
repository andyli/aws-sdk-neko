#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/transfer/TransferClient.h>
#include <aws/transfer/UploadFileRequest.h>

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

static value new_S3Client(value region) {
	ClientConfiguration clientConfig;
	clientConfig.followRedirects = true;
	clientConfig.region = val_string(region);
	clientConfig.connectTimeoutMs = 5000;
	clientConfig.requestTimeoutMs = 6000;

	auto c = Aws::New<std::shared_ptr<S3Client>>(ALLOCATION_TAG, Aws::New<S3Client>(ALLOCATION_TAG, clientConfig, false));
	auto handle = alloc_abstract(k_S3Client, c);
	val_gc(handle, free_S3Client);
	return handle;
}
DEFINE_PRIM(new_S3Client, 1);


// TransferClient

DEFINE_KIND(k_TransferClient);

static void free_TransferClient( value s3client ) {
	auto _s3client = (std::shared_ptr<TransferClient>*) val_data(s3client);
	Aws::Delete(_s3client);
}

static value new_TransferClient(value s3client) {
	TransferClientConfiguration transferClientConfig;
	auto _s3client = * (std::shared_ptr<S3Client>*) val_data(s3client);
	auto c = Aws::New<std::shared_ptr<TransferClient>>(ALLOCATION_TAG, Aws::New<TransferClient>(ALLOCATION_TAG, _s3client, transferClientConfig));
	auto handle = alloc_abstract(k_TransferClient, c);
	val_gc(handle, free_TransferClient);
	return handle;
}
DEFINE_PRIM(new_TransferClient, 1);


// S3FileRequest

DEFINE_KIND(k_S3FileRequest);

static void free_UploadFileRequest(value uploadFileRequest) {
	auto _UploadFileRequest = (std::shared_ptr<UploadFileRequest>*) val_data(uploadFileRequest);
	Aws::Delete(_UploadFileRequest);
}

static void free_DownloadFileRequest(value downloadFileRequest) {
	auto _DownloadFileRequest = (std::shared_ptr<DownloadFileRequest>*) val_data(downloadFileRequest);
	Aws::Delete(_DownloadFileRequest);
}

static value S3FileRequest_GetFileSize(value s3FileRequest) {
	auto _s3FileRequest = * (std::shared_ptr<S3FileRequest>*) val_data(s3FileRequest);
	auto _GetFileSize = _s3FileRequest->GetFileSize();
	return alloc_int(_GetFileSize);
}
DEFINE_PRIM(S3FileRequest_GetFileSize, 1);

static value S3FileRequest_IsDone(value s3FileRequest) {
	auto _s3FileRequest = * (std::shared_ptr<S3FileRequest>*) val_data(s3FileRequest);
	auto _IsDone = _s3FileRequest->IsDone();
	return alloc_int(_IsDone);
}
DEFINE_PRIM(S3FileRequest_IsDone, 1);

static value S3FileRequest_CompletedSuccessfully(value s3FileRequest) {
	auto _s3FileRequest = * (std::shared_ptr<S3FileRequest>*) val_data(s3FileRequest);
	auto _CompletedSuccessfully = _s3FileRequest->CompletedSuccessfully();
	return alloc_int(_CompletedSuccessfully);
}
DEFINE_PRIM(S3FileRequest_CompletedSuccessfully, 1);

static value S3FileRequest_WaitUntilDone(value s3FileRequest) {
	auto _s3FileRequest = * (std::shared_ptr<S3FileRequest>*) val_data(s3FileRequest);
	auto _WaitUntilDone = _s3FileRequest->WaitUntilDone();
	return alloc_int(_WaitUntilDone);
}
DEFINE_PRIM(S3FileRequest_WaitUntilDone, 1);

static value S3FileRequest_GetFailure(value s3FileRequest) {
	auto _uploadFileRequest = * (std::shared_ptr<S3FileRequest>*) val_data(s3FileRequest);
	auto _failure = _uploadFileRequest->GetFailure();
	return alloc_string(_failure.c_str());
}
DEFINE_PRIM(S3FileRequest_GetFailure, 1);

static value TransferClient_UploadFile(
	value transferClient,
	value fileName,
	value bucketName,
	value keyName,
	value contentType
) {
	auto _transferClient = * (std::shared_ptr<TransferClient>*) val_data(transferClient);
	auto _uploadFileRequest = Aws::New<std::shared_ptr<UploadFileRequest>>(
		ALLOCATION_TAG,
		_transferClient->UploadFile(
			val_string(fileName),
			val_string(bucketName),
			val_string(keyName),
			val_string(contentType)
		)
	);

	auto r = alloc_abstract(k_S3FileRequest, _uploadFileRequest);
	val_gc(r, free_UploadFileRequest);
	return r;
}
DEFINE_PRIM(TransferClient_UploadFile, 5);

static value TransferClient_DownloadFile(
	value transferClient,
	value fileName,
	value bucketName,
	value keyName
) {
	auto _transferClient = * (std::shared_ptr<TransferClient>*) val_data(transferClient);
	auto _downloadFileRequest = Aws::New<std::shared_ptr<DownloadFileRequest>>(
		ALLOCATION_TAG,
		_transferClient->DownloadFile(
			val_string(fileName),
			val_string(bucketName),
			val_string(keyName)
		)
	);

	auto r = alloc_abstract(k_S3FileRequest, _downloadFileRequest);
	val_gc(r, free_DownloadFileRequest);
	return r;
}
DEFINE_PRIM(TransferClient_DownloadFile, 4);


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
