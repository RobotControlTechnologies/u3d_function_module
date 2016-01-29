/*
* File: u3d_function_module.h
* Author: m79lol, iskinmike
*
*/

#ifndef U3D_FUNCTION_MODULE_H
#define U3D_FUNCTION_MODULE_H

struct BoxOfMessagesData;
struct MutexAndBoxVector;

class u3dFunctionModule : public FunctionModule {
  FunctionData **u3d_functions;
  colorPrintfModuleVA_t *colorPrintf_p;

  bool is_world_initialized;
  bool is_initialized;
  int postmans_uniq_id;

  std::vector<int> ids_of_created_objects;
  char *recv_message;
  std::string *part_message_buffer;

  void modulePostmanThread();
  void moduleRecieverThread();
  void read_handler(boost::asio::ip::tcp::socket *handler_socket,
                    const boost::system::error_code &ec,
                    std::size_t bytes_transferred);
  void write_handler(const boost::system::error_code &ec,
                     std::size_t bytes_transferred);
  void connect_handler(const boost::system::error_code &ec);

  boost::mutex postman_exit_mutex;
  bool postman_thread_exit;

  boost::asio::io_service robot_io_service_;
  boost::asio::ip::tcp::socket module_socket;

  // Postman thread variables
  boost::thread *module_postman_thread;
  boost::mutex postman_thread_mutex;
  bool postman_thread_waker_flag;
  boost::condition_variable postman_thread_waker;

  boost::thread *module_reciever_thread;
  boost::mutex module_mutex;

  MutexAndBoxVector *data_for_shared_memory;
  std::vector<BoxOfMessagesData *> *box_of_messages;
  std::map<int, BoxOfMessagesData *> postmans_map_of_mailed_messages;

  ModuleInfo *mi;

 public:
  u3dFunctionModule();

  // init
  const struct ModuleInfo &getModuleInfo();
  void prepare(colorPrintfModule_t *colorPrintf_p,
               colorPrintfModuleVA_t *colorPrintfVA_p);

  // compiler only
  FunctionData **getFunctions(unsigned int *count_functions);
  void *writePC(unsigned int *buffer_length);

  // intepreter - program & lib
  void readPC(void *buffer, unsigned int buffer_length);

  // intepreter - program
  FunctionResult *executeFunction(system_value function_index, void **args);
  int startProgram(int uniq_index);
  int endProgram(int uniq_index);

  // destructor
  void destroy();
  ~u3dFunctionModule(){};

  void colorPrintf(ConsoleColor colors, const char *mask, ...);
};

#endif /* U3D_FUNCTION_MODULE_H  */
