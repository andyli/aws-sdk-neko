#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/transfer/TransferClient.h>
#include <aws/transfer/UploadFileRequest.h>

#include <neko.h>

using namespace Aws::Client;
using namespace Aws::Transfer;
using namespace Aws::S3;
// using namespace Aws::S3::Model;

static const char* ALLOCATION_TAG = "aws-skd-neko";
static Aws::SDKOptions options;

static value _InitAPI() {
	Aws::InitAPI(options);
	return val_null;
}
DEFINE_PRIM(_InitAPI, 0);

static value _ShutdownAPI() {
	Aws::ShutdownAPI(options);
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
	clientConfig.region = Aws::RegionMapper::GetRegionFromName(val_string(region));
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


// UploadFileRequest

DEFINE_KIND(k_UploadFileRequest);

static void free_UploadFileRequest(value uploadFileRequest) {
	auto _UploadFileRequest = (std::shared_ptr<UploadFileRequest>*) val_data(uploadFileRequest);
	Aws::Delete(_UploadFileRequest);
}

static value UploadFileRequest_IsDone(value uploadFileRequest) {
	auto _uploadFileRequest = * (std::shared_ptr<UploadFileRequest>*) val_data(uploadFileRequest);
	auto _IsDone = _uploadFileRequest->IsDone();
	return alloc_bool(_IsDone);
}
DEFINE_PRIM(UploadFileRequest_IsDone, 1);

static value UploadFileRequest_GetFailure(value uploadFileRequest) {
	auto _uploadFileRequest = * (std::shared_ptr<UploadFileRequest>*) val_data(uploadFileRequest);
	auto _failure = _uploadFileRequest->GetFailure();
	return _failure.length() > 0 ? alloc_string(_failure.c_str()) : val_null;
}
DEFINE_PRIM(UploadFileRequest_GetFailure, 1);

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

	auto r = alloc_abstract(k_UploadFileRequest, _uploadFileRequest);
	val_gc(r, free_UploadFileRequest);
	return r;
}
DEFINE_PRIM(TransferClient_UploadFile, 5);
