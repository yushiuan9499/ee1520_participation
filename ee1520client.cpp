
// ecs36b first program

#include "ee1520_Exception.h"
#include <fstream>
#include <iostream>

// JSON RPC part
#include "ee1520client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int main(int argc, char *argv[]) {
  // test for yourself
  // HttpClient httpclient("http://127.0.0.1:8384");
  const string HOME = getenv("HOME");
  const string LOGS_FILE =
      HOME + "/.local/share/ee1520_participation/logs.json";

  system("mkdir -p ~/.local/share/ee1520_participation");
  if (argc != 2) {
    cerr << "must provide exactly a message" << endl;
    return 1;
  }
  cout << "Sending msg: " << argv[1] << endl;
  {
    ifstream responseFile(LOGS_FILE);
    if (responseFile.good()) {
      Json::Value prevResponse;
      responseFile >> prevResponse;
      if (prevResponse.isMember(argv[1])) {
        cout << "Already participated in this survey" << endl;
        return 0;
      }
    }
    responseFile.close();
  }
  // connecting to your instructor's laptop
  HttpClient httpclient("https://ethikos.ngrok.io");
  ee1520Client myClient(httpclient, JSONRPC_CLIENT_V2);

  Json::Value myv;
  Json::Value jv_list;

  //
  try {
    myv = myClient.participate("participate", argv[1], "E24136542");
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }

  cout << "hit" << endl;
  if (myv["status"] == "successful" || myv["status"] == "already registered") {
    ofstream responseFile(LOGS_FILE);
    if (!responseFile.good()) {
      cerr << "Failed to write to file" << endl;
      return 1;
    }
    responseFile << myv.toStyledString();
    responseFile.close();
  }
  std::cout << myv.toStyledString() << std::endl;
  return 0;
}
