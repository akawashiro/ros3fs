cmake_minimum_required(VERSION 3.16)
project(ros3fs)

# Run ./build-and-install-aws-sdk-cpp.sh before running cmake.
list(APPEND CMAKE_PREFIX_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/AWSSDK
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-core
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-crt-cpp/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-http/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-mqtt/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-mqtt/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-io/cmake/static
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-io/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-s3/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-s3/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-sdkutils/cmake/static
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-sdkutils/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/s2n/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/s2n/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/s2n/cmake/modules
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-common/cmake/static
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-common/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-cal/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-cal/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-cal/cmake/modules
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-compression/cmake/static
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-compression/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-auth/cmake/static
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-auth/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-checksums/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-checksums/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-crt-cpp/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-crt-cpp/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-event-stream/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-event-stream/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-http/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/aws-c-http/cmake/static
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-firehose
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mgn
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime-sdk-messaging
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot1click-projects
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-migrationhubstrategy
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dms
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-servicecatalog
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dynamodb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lambda
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codepipeline
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rds-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-qldb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-amplifybackend
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cognito-identity
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-robomaker
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-s3-encryption
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-frauddetector
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lookoutvision
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-controltower
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot-roborunner
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lakeformation
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-fsx
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ram
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-apigateway
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-globalaccelerator
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codestar-notifications
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ssm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appsync
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sts
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-fms
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sso
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-swf
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-alexaforbusiness
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ivs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-apprunner
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pinpoint
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ssm-sap
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediatailor
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-s3
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-finspace-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime-sdk-voice
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-transfer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-importexport
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53resolver
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticloadbalancing
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-wafv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticloadbalancingv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-macie2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-m2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-customer-profiles
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudtrail
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-honeycode
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-license-manager-user-subscriptions
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-fis
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-emr-serverless
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-xray
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudtrail-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesisanalytics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kms
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/testing-resources
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudsearchdomain
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lexv2-runtime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticbeanstalk
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-connectcampaigns
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-migrationhub-config
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codeartifact
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-gamelift
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iam
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appmesh
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ivschat
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediapackage-vod
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-s3control
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-backup-gateway
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-snow-device-management
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime-sdk-media-pipelines
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-auditmanager
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-timestream-write
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticache
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-emr-containers
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codestar-connections
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-application-autoscaling
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-waf
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-identitystore
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kendra
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-meteringmarketplace
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-wisdom
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotwireless
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-secretsmanager
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kendra-ranking
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-redshift-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-grafana
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-batch
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-marketplace-entitlement
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudsearch
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotsitewise
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-healthlake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-personalize-runtime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-transcribestreaming
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mq
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sqs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ssm-incidents
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cognito-sync
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-workspaces-web
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-eventbridge
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-email
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-voice-id
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appconfigdata
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-databrew
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-simspaceweaver
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-network-firewall
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-schemas
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-transcribe
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-budgets
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ssm-contacts
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pipes
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-machinelearning
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-textract
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cleanrooms
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-billingconductor
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-s3-crt
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mturk-requester
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-account
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloud9
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-groundstation
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appflow
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-greengrassv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-waf-regional
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-braket
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ec2-instance-connect
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kafkaconnect
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-amplify
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot-jobs-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-forecastquery
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-shield
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-featurestore-runtime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudhsm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-serverlessrepo
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lex-models
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-migrationhuborchestrator
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pi
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudformation
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/AWSSDK
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/AWSSDK/platform
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/AWSSDK
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotfleetwise
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-personalize-events
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-opensearch
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-text-to-speech
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iottwinmaker
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sdb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codeguruprofiler
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-scheduler
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sso-admin
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-glue
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53-recovery-cluster
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ds
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-worklink
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime-sdk-meetings
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-qldb-session
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-comprehend
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ec2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cur
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesis-video-webrtc-storage
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-geospatial
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediapackage
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-securityhub
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-service-quotas
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesisvideo
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediaconnect
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rolesanywhere
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotdeviceadvisor
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-workspaces
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-access-management
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-resourcegroupstaggingapi
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesis
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-nimble
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-devicefarm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rekognition
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-memorydb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-docdb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-devops-guru
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-athena
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mobile
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-proton
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-events
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-personalize
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-synthetics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-connectparticipant
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-marketplace-catalog
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mwaa
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sso-oidc
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-managedblockchain
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-greengrass
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iot1click-devices
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-opsworkscm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lookoutequipment
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-oam
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-config
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-location
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesis-video-media
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-queues
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-securitylake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-quicksight
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pinpoint-sms-voice-v2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-medialive
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ecr-public
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-edge
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-neptune
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-comprehendmedical
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-tnb
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-AWSMigrationHub
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudfront
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-accessanalyzer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediastore-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-resource-explorer-2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-translate
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-timestream-query
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ecs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-datasync
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-macie
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codecommit
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rbin
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotfleethub
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-detective
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codedeploy
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticmapreduce
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-keyspaces
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lookoutmetrics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-marketplacecommerceanalytics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesis-video-archived-media
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-license-manager
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediastore
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-amp
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-chime-sdk-identity
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-inspector
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-arc-zonal-shift
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53-recovery-control-config
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotthingsgraph
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-awstransfer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dlm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-acm-pca
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-s3outposts
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sms
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-es
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-backupstorage
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elastic-inference
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-wellarchitected
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-inspector2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-directconnect
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pricing
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-evidently
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appintegrations
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotevents
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-guardduty
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lexv2-models
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dax
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-mediaconvert
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-snowball
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ebs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-connect-contact-lens
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-resiliencehub
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dynamodbstreams
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-workdocs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-license-manager-linux-subscriptions
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudhsmv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-apigatewaymanagementapi
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-support-app
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elasticfilesystem
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codeguru-reviewer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-imagebuilder
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-panorama
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rum
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-compute-optimizer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-connect
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-servicediscovery
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-polly
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codecatalyst
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lex
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sesv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-eks
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-monitoring
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-organizations
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-outposts
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53domains
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-backup
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cloudcontrol
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-redshift-serverless
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-states
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-workmail
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotsecuretunneling
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-health
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-savingsplans
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-metrics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotanalytics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-omics
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-dataexchange
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-resource-groups
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-glacier
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesisanalyticsv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-runtime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-opsworks
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kinesis-video-signaling
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-logs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-migration-hub-refactor-spaces
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-internetmonitor
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-support
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-servicecatalog-appregistry
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-forecast
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-networkmanager
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-pinpoint-email
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appconfig
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-amplifyuibuilder
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-elastictranscoder
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-clouddirectory
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ecr
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-identity-management
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-iotevents-data
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-rds
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codestar
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-acm
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-signer
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-connectcases
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-kafka
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sns
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-autoscaling-plans
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-storagegateway
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-application-insights
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sms-voice
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-docdb-elastic
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-sagemaker-a2i-runtime
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-workmailmessageflow
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-autoscaling
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-codebuild
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-finspace
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-ce
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-opensearchserverless
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-discovery
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-apigatewayv2
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-lightsail
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-route53-recovery-readiness
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-drs
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-datapipeline
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-cognito-idp
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-privatenetworks
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-applicationcostprofiler
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-core
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-redshift
    # ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-appstream
    ${CMAKE_CURRENT_SOURCE_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-gamesparks)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(PkgConfig)

include(third-party)
get_third_party(glog)
set(WITH_GFLAGS
    OFF
    CACHE INTERNAL "" FORCE)
set(WITH_UNWIND OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/glog
                 ${CMAKE_CURRENT_BINARY_DIR}/glog EXCLUDE_FROM_ALL)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/glog
                    ${CMAKE_CURRENT_BINARY_DIR}/glog/src)
add_definitions(-DC10_USE_GLOG=1)

pkg_check_modules(LIBFUSE3 REQUIRED fuse3)
message(STATUS "Found fuse3: ${LIBFUSE3_INCLUDE_DIRS} ${LIBFUSE3_LIBRARIES}")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(AWSSDK REQUIRED COMPONENTS s3)
add_executable(s3-example "s3-example.cc")
target_link_libraries(s3-example ${AWSSDK_LINK_LIBRARIES})

add_executable(hello_fs hello.cc)
target_link_libraries(hello_fs ${LIBFUSE3_LIBRARIES} glog)
target_include_directories(hello_fs PRIVATE ${LIBFUSE3_INCLUDE_DIRS})
target_compile_options(hello_fs PUBLIC -Wall -Werror)

add_executable(ros3fs ros3fs.cc)
target_link_libraries(ros3fs ${AWSSDK_LINK_LIBRARIES} ${LIBFUSE3_LIBRARIES} glog  nlohmann_json::nlohmann_json)
target_include_directories(ros3fs PRIVATE ${LIBFUSE3_INCLUDE_DIRS})
target_compile_options(ros3fs PUBLIC -Wall -Werror)

add_executable(ls_test ls_test.cc)
target_link_libraries(ls_test PRIVATE nlohmann_json::nlohmann_json)
