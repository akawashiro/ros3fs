#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/s3/S3Client.h>
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
  }

  // Before the application terminates, the SDK must be shut down.
  ShutdownAPI(options);
  return 0;
}
