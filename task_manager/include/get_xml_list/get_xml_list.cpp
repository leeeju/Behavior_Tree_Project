#include "get_xml_list.h"
#include <string>
#include <filesystem>

#ifdef _XML_LIST_PATH_
#else
#define _XML_LIST_PATH_ "/home/shh/bt_ex/xml"
#endif

void xmlListManager::setXmlListAmount() {
  unsigned int size_count = 0;
  std::string path = _XML_LIST_PATH_;
  for (const auto &entry : std::filesystem::directory_iterator(path))
    size_count++;
  xml_file_amount = size_count;
}

void xmlListManager::bringXmlList(){
  unsigned int size_count = 0;
  std::string path = _XML_LIST_PATH_;
  for (const auto &entry : std::filesystem::directory_iterator(path))
  {
    xml_file_list_test[size_count] = entry.path();
    size_count++;
  }
}
unsigned int xmlListManager::getXmlListAmount() const {
  return xml_file_amount;
}

std::vector<std::string> xmlListManager::getXmlListNames(){
  return xml_file_list_test;
}
