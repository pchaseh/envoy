#include "test/mocks/server/transport_socket_factory_context.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Server {
namespace Configuration {

using ::testing::ReturnRef;

MockTransportSocketFactoryContext::MockTransportSocketFactoryContext()
    : secret_manager_(std::make_unique<Secret::SecretManagerImpl>(config_tracker_)) {
  ON_CALL(*this, serverFactoryContext()).WillByDefault(ReturnRef(server_context_));
  ON_CALL(*this, clusterManager()).WillByDefault(ReturnRef(cluster_manager_));
  ON_CALL(*this, messageValidationVisitor())
      .WillByDefault(ReturnRef(ProtobufMessage::getStrictValidationVisitor()));
  ON_CALL(*this, sslContextManager()).WillByDefault(ReturnRef(context_manager_));
  ON_CALL(*this, statsScope()).WillByDefault(ReturnRef(*store_.rootScope()));
}

MockTransportSocketFactoryContext::~MockTransportSocketFactoryContext() = default;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
