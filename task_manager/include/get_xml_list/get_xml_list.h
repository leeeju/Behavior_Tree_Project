#ifndef BT_EX_INCLUDE_GET_XML_LIST_H_
#define BT_EX_INCLUDE_GET_XML_LIST_H_

#endif //BT_EX_INCLUDE_GET_XML_LIST_H_

#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

class xmlListManager
{
 private:
  unsigned int xml_file_amount{};
  std::vector<std::string> xml_file_list_test;
  void setXmlListAmount();
  void bringXmlList();
//  std::string xml_file_list

 public:
  [[nodiscard]] unsigned int getXmlListAmount() const;
  std::vector<std::string> getXmlListNames();

  xmlListManager()
  {
    setXmlListAmount();
    xml_file_list_test.resize(xml_file_amount);
    bringXmlList();
  }
};

