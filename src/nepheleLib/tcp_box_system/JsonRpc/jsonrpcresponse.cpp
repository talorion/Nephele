#include "jsonrpcresponse.hpp"


namespace talorion {

  JsonRpcResponse::JsonRpcResponse(const QByteArray &rdta):
      m_rawData(rdta),
      m_result(),
      m_id(),
      m_error()
  {

  }

} // namespace talorion
