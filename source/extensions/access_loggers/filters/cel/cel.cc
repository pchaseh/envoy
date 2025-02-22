#include "source/extensions/access_loggers/filters/cel/cel.h"

namespace Envoy {
namespace Extensions {
namespace AccessLoggers {
namespace Filters {
namespace CEL {

namespace Expr = Envoy::Extensions::Filters::Common::Expr;

CELAccessLogExtensionFilter::CELAccessLogExtensionFilter(
    Expr::Builder& builder, const google::api::expr::v1alpha1::Expr& input_expr)
    : parsed_expr_(input_expr) {
  compiled_expr_ = Expr::createExpression(builder, parsed_expr_);
}

bool CELAccessLogExtensionFilter::evaluate(const StreamInfo::StreamInfo& stream_info,
                                           const Http::RequestHeaderMap& request_headers,
                                           const Http::ResponseHeaderMap& response_headers,
                                           const Http::ResponseTrailerMap& response_trailers,
                                           AccessLog::AccessLogType) const {
  Protobuf::Arena arena;
  auto eval_status = Expr::evaluate(*compiled_expr_, arena, stream_info, &request_headers,
                                    &response_headers, &response_trailers);
  if (!eval_status.has_value() || eval_status.value().IsError()) {
    return false;
  }
  auto result = eval_status.value();
  return result.IsBool() ? result.BoolOrDie() : false;
}

} // namespace CEL
} // namespace Filters
} // namespace AccessLoggers
} // namespace Extensions
} // namespace Envoy
