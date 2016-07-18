#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/memory/stl/AwsStringStream.h>

#include <neko.h>

using namespace Aws::S3;
using namespace Aws::S3::Model;

// S3Client

DEFINE_KIND(k_S3Client);

static value new_S3Client() {
	S3Client* c = new S3Client();
	return alloc_abstract(k_S3Client, c);
}
DEFINE_PRIM(new_S3Client,0);


// PutObjectRequest

DEFINE_KIND(k_PutObjectRequest);

static value new_PutObjectRequest() {
	PutObjectRequest* r = new PutObjectRequest();
	return alloc_abstract(k_PutObjectRequest, r);
}
DEFINE_PRIM(new_PutObjectRequest,0);

static value PutObjectRequest_WithKey(value self, value key) {
	PutObjectRequest* r = (PutObjectRequest*) val_data(self);
	r->WithKey(val_string(key));
	return self;
}
DEFINE_PRIM(PutObjectRequest_WithKey,2);

static value PutObjectRequest_WithBucket(value self, value bucket) {
	PutObjectRequest* r = (PutObjectRequest*) val_data(self);
	r->WithBucket(val_string(bucket));
	return self;
}
DEFINE_PRIM(PutObjectRequest_WithBucket,2);
