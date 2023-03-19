#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <iostream>

using namespace Aws;

int main() {
  // The Aws::SDKOptions struct contains SDK configuration options.
  // An instance of Aws::SDKOptions is passed to the Aws::InitAPI and
  // Aws::ShutdownAPI methods.  The same instance should be sent to both
  // methods.
  SDKOptions options;
  options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;

  // The AWS SDK for C++ must be initialized by calling Aws::InitAPI.
  InitAPI(options);
  {
    Aws::Client::ClientConfiguration config;
    config.endpointOverride = "http://localhost:9878";
    S3::S3Client client(config);

    auto outcome = client.ListBuckets();
    if (outcome.IsSuccess()) {
      std::cout << "Found " << outcome.GetResult().GetBuckets().size()
                << " buckets\n";
      for (auto &&b : outcome.GetResult().GetBuckets()) {
        std::cout << b.GetName() << std::endl;
      }
    } else {
      std::cout << "Failed with error: " << outcome.GetError() << std::endl;
    }

    const Aws::String bucketName = "bucket1/";
    Aws::S3::Model::ListObjectsRequest request;
    request.WithBucket(bucketName);

    auto outcome_o = client.ListObjects(request);

    if (!outcome.IsSuccess()) {
      std::cerr << "Error: ListObjects: " << outcome.GetError().GetMessage()
                << std::endl;
    } else {
      std::cout << "Found " << outcome_o.GetResult().GetContents().size()
                << " objects" << std::endl;
      Aws::Vector<Aws::S3::Model::Object> objects =
          outcome_o.GetResult().GetContents();

      for (Aws::S3::Model::Object &object : objects) {
        std::cout << object.GetKey()
                  << " object.GetSize() = " << object.GetSize() << std::endl;
      }
    }

    Aws::S3::Model::GetObjectRequest request_g;
    request_g.SetBucket("bucket1/");
    request_g.SetKey("testfile_a");

    Aws::S3::Model::GetObjectOutcome outcome_g =
            client.GetObject(request_g);

    if (!outcome_g.IsSuccess()) {
        const Aws::S3::S3Error &err = outcome.GetError();
        std::cerr << "Error: GetObject: " <<
                  err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
    else {
        std::cout << "Successfully retrieved '" << "testfile_a" << "' from '"
                  << "bucket1/" << "'." << std::endl;
        outcome_g.GetResult().GetBody().rdbuf();
    }
  }

  // Before the application terminates, the SDK must be shut down.
  ShutdownAPI(options);
  return 0;
}
