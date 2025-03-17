
// ecs36b first program

#include <iostream>
#include "ee1520_Exception.h"

// JSON RPC part
#include "ee1520client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int
main()
{
  // test for yourself
  // HttpClient httpclient("http://127.0.0.1:8384");

  // connecting to your instructor's laptop
  HttpClient httpclient("https://ethikos.ngrok.io");
  
  ee1520Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;
  Json::Value jv_list;

  //
  try {
    myv = myClient.participate("participate", "something", "E12345678");
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }

  std::cout << myv.toStyledString() << std::endl;
  return 0;
}
