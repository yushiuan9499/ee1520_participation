
// ngrok http --domain ethikos.ngrok.io http://localhost:8384

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "ee1520server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "ee1520client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

// ecs36b
#include "ee1520_Common.h"
#include "JvTime.h"

using namespace jsonrpc;
using namespace std;

class Myee1520Server : public ee1520Server
{
public:
  Myee1520Server(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value participate(const std::string& action, const std::string& pass, const std::string& studentid);
};

Myee1520Server::Myee1520Server(AbstractServerConnector &connector, serverVersion_t type)
  : ee1520Server(connector, type)
{
  std::cout << "Myee1520Server Object created" << std::endl;
}

// member functions

std::string p_secret { "something" };

Json::Value
Myee1520Server::participate
(const std::string& action, const std::string& pass, const std::string& studentid)
{
  Json::Value lv_log;
  
  int i;
  int error_code = 0;
  int rc = 0;
  Json::Value result;

  char buf_fname1[256];
  bzero(buf_fname1, 256);
  snprintf(buf_fname1, 255, "./config/ee1520_students.json");

  Json::Value lv_students;

  Exception_Info * ei_ptr = NULL;
  ee1520_Exception lv_exception {};
  ee1520_Exception * lv_exception_ptr = &lv_exception;

  try {
    if (pass != p_secret)
      {
	// printf("here 1\n");
	ei_ptr = new Exception_Info {};
	ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
	ei_ptr->which_string = ("incorrect pass code");
	ei_ptr->how_code = EE1520_ERROR_NORMAL;
	ei_ptr->what_code = EE1520_ERROR_JSON_KEY_MISSING;
	(lv_exception_ptr->info_vector).push_back(ei_ptr);
	// error_code = -9;
	throw (*lv_exception_ptr);
    }
  
    if (action != "participate")
      {
	// printf("here 2\n");
	ei_ptr = new Exception_Info {};
	ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
	ei_ptr->which_string = ("action " + action + " mismatched");
	ei_ptr->how_code = EE1520_ERROR_NORMAL;
	ei_ptr->what_code = EE1520_ERROR_JSON2OBJECT_ACTION;
	(lv_exception_ptr->info_vector).push_back(ei_ptr);
	// error_code = -9;
	throw (*lv_exception_ptr);
      }

    rc = myFile2JSON(buf_fname1, &lv_students);
    if (rc != 0)
      {
	// printf("here 3\n");
	ei_ptr = new Exception_Info {};
	ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
	ei_ptr->which_string = "student file failed to open";
	ei_ptr->how_code = EE1520_ERROR_NORMAL;
	ei_ptr->what_code = EE1520_ERROR_FILE_NOT_EXIST;
	(lv_exception_ptr->info_vector).push_back(ei_ptr);
	throw (*lv_exception_ptr);
	// error_code = rc;
	// throw ee1520_Exception
	//  { ("myFile2JSON error " + std::string { buf_fname1 }) };
      }

    if((lv_students.isNull() == true) ||
       (lv_students.isArray() == false))
      {
	// // printf("here 4\n");
	ei_ptr = new Exception_Info {};
	ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
	ei_ptr->which_string = "student file not array";
	ei_ptr->how_code = EE1520_ERROR_NORMAL;
	ei_ptr->what_code = EE1520_ERROR_JSON_KEY_TYPE_MISMATCHED;
	(lv_exception_ptr->info_vector).push_back(ei_ptr);
	throw (*lv_exception_ptr);
	// error_code = -1;
	// throw ee1520_Exception
	//   { ("teams JSON format incorrect " + std::string { buf_fname1 }) };
      }

    int found = 0;
    int st_idx = -1;
    for (i = 0; i < lv_students.size(); i++)
      {
	if ((lv_students[i]["id"]).asString() == studentid)
	  {
	    // printf("here 5\n");

	    st_idx = i;
	    found = 1;

	    JvTime * my_ptr = getNowJvTime();
	    std::string tmp_s;
	    if ((lv_students[i][p_secret]).isNull() == true)
	      {
		(lv_students[i][p_secret]) = *(my_ptr->dump2JSON());
		tmp_s = "successful";
	      }
	    else
	      {
		tmp_s = "already registered";
	      }
	    result = (lv_students[i]);
	    result["time stamp"] = *(my_ptr->dump2JSON());
	    result["status"] = tmp_s;

	    Json::Value lv_log;
	    
	    lv_log["arg action"] = action;
	    lv_log["arg pass"] = pass;
	    lv_log["arg vid"] = (lv_students[i]["vname"]).asString();
	    lv_log["time"] = *(my_ptr->dump2JSON());
	    myPrintLog(lv_log.toStyledString(), "./config/ee1520server.log");

	    rc = myJSON2File(buf_fname1, &lv_students);
	    if (rc != 0)
	      {
		// printf("here 6\n");
		ei_ptr = new Exception_Info {};
		ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
		ei_ptr->which_string = "student file failed to modify";
		ei_ptr->how_code = EE1520_ERROR_NORMAL;
		ei_ptr->what_code = EE1520_ERROR_FILE_WRITE;
		(lv_exception_ptr->info_vector).push_back(ei_ptr);
		throw (*lv_exception_ptr);
	      }
	    break;
	  }
      }

    if (found == 0)
      {
	// printf("here 7\n");
	ei_ptr = new Exception_Info {};
	ei_ptr->where_code = EE1520_ERROR_JSONRPC_SERVER;
	ei_ptr->which_string = "student not found";
	ei_ptr->how_code = EE1520_ERROR_NORMAL;
	ei_ptr->what_code = EE1520_ERROR_FILE_NOT_EXIST;
	(lv_exception_ptr->info_vector).push_back(ei_ptr);
	throw (*lv_exception_ptr);
	// error_code = EE1520_ERROR_VSID_POST_ID;
	// throw ee1520_Exception
	//   { ("Student ID not found") };	
      }

  } catch (ee1520_Exception& e) {
    std::cerr << e.what() << std::endl;
    Json::Value result_json;
    int erc = produceErrorJSON(e, "./config/ee1520server_error.log", &result_json, 0);
    std::cerr << result_json << std::endl;
    result["json"] = result_json;
    result["reason"] = e.what();
    result["status"] = "failed";
  }

  // printf("here 9\n");
  lv_log["result"] = result;
  myPrintLog(lv_log.toStyledString(), "./config/ee1520server.log");
  
  return result;
}

int
main(void)
{
  JvTime * my_ptr = getNowJvTime();

  srand((my_ptr->second) * (my_ptr->minute) * (my_ptr->year));
  HttpServer httpserver(8384);
  Myee1520Server s(httpserver,
		   JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}
