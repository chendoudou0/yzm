/**
      @file   xml-config.h
      @brief  从XML读取/设置配置数据
      @date   2006/05/31
      @changelist
               2008/1/18  vermin增加从流中读取
 */

#ifndef __XML_CONFIG_H__
#define __XML_CONFIG_H__

#include "tinyxml.h"
#include <string>

///////////////////////////////////////////////////////////////////////////////

class XmlConfig
{
public:
  XmlConfig(const char* xmlFile = NULL) : m_good(false)
  {
    if (xmlFile != NULL)
      init(xmlFile);
  }

  ~XmlConfig()
  {}

  bool init(const char* xmlFile);
  bool save();

  operator bool()const
  {
    return m_good;
  }

  
  // Parse 将输入的流解析为xml格式
  bool Parse(const char * szBuf);

  // 错误后返回错误信息
  const char * getLastError();
  


  // getNode: 获取的一个与指定路径匹配的XML结点
  // path: 结点路径, eg: /root/parent/child, '/' is document root path
  // <root><parent><child> ... </child></parent></root>
  // return ptr to object Node if path is valid, otherwise NULL
  TiXmlNode* getNode(const char* path);

  // baseNode: base for the return node
  // path: relative path between baseNode and object node
  // return ptr to object Node if path is valid, otherwise NULLs
  TiXmlNode* getNode(TiXmlNode& baseNode, const char* path);
  
  // getValue: 返回指定路径结点的值
  std::string getValue(const char* path, const char* defaultValue = NULL);
  int getValue(const char* path, int defaultValue = 0);
  double getValue(const char* path, double defaultValue = 0.0);

  // getAttrValue: 获取指定结点的属性值
  std::string getAttrValue(const char* path, const char* attrName, const char* defaultValue = NULL);
  int getAttrValue(const char* path, const char* attrName, int defaultValue = 0);
  double getAttrValue(const char* path, const char* attrName, double defaultValue = 0.0);
  
  // setValue: 设置指定路径结点的值
  bool setValue(const char* path, const char* newValue);
  bool setValue(const char* path, int newValue);
  bool setValue(const char* path, double newValue);

  // setAttrValue: 设置指定结点的属性值
  bool setAttrValue(const char* path, const char* attrName, const char* newValue);
  bool setAttrValue(const char* path, const char* attrName, int newValue);
  bool setAttrValue(const char* path, const char* attrName, double newValue);
  
  std::string toString(std::string& outstr);
protected:  
  static std::string xmlNodeValue(const TiXmlNode& node)
  {
    const char* pValue = NULL;
    int t = node.Type();

    if (t == TiXmlNode::ELEMENT)
    {
      const TiXmlNode* pNode = node.FirstChild();
      if ( pNode )
      {
        t = pNode->Type();
        if (t == TiXmlNode::TEXT)
          pValue = pNode->Value();
      }     
    }
    else if (t == TiXmlNode::TEXT)
    {
      pValue = node.Value();
    }

    return (pValue)?pValue:"";
  }

protected:
  TiXmlDocument m_xmlDoc;
  bool m_good;
};

inline std::string XmlConfig::toString(std::string& outstr)
{
  return outstr<<m_xmlDoc;
}

inline bool XmlConfig::init(const char* xmlFile)
{
  m_good = m_xmlDoc.LoadFile(xmlFile);
  return m_good;
}

inline bool XmlConfig::save()
{
  return m_xmlDoc.SaveFile();
}

inline TiXmlNode* XmlConfig::getNode(const char* path)
{
  return getNode(m_xmlDoc, path);  
}

inline TiXmlNode* XmlConfig::getNode(TiXmlNode& baseNode, const char* path)
{
  char nodeName[256] = {0};
  TiXmlNode* pNode = &baseNode;

  while (path && (*path != '\0') && pNode) 
  {
    if (*path == '/') 
      ++path;

    char* temp = (char *)strchr(path, '/');
    memset(nodeName, 0, sizeof(nodeName));

    if (temp == NULL)
      strcpy(nodeName, path);
    else
      strncpy(nodeName, path, temp - path); 

    if (strcmp(nodeName, ".") == 0)
    {
      // no operation
    }
    else if (strcmp(nodeName, "..") == 0)
    {
      pNode = pNode->Parent();
    }
    else
    {
      pNode = pNode->FirstChild(nodeName);
    }

    if ( temp )
      path = temp + 1;
    else
      path = temp;
  }

  return pNode; 
}

inline std::string XmlConfig::getValue(const char* path, const char* defaultValue)
{
  TiXmlNode* pNode = getNode(path);
  std::string v;
  if (pNode)
    v = xmlNodeValue(*pNode);

  if (v.empty() && defaultValue)
    v = defaultValue;

  return v;
}

inline int XmlConfig::getValue(const char* path, int defaultValue)
{
  std::string v = getValue(path, "");
  if ( !v.empty() )
    return atoi(v.c_str());

  return defaultValue;
}

inline double XmlConfig::getValue(const char* path, double defaultValue)
{
  std::string v = getValue(path, "");
  if ( !v.empty() )
    return strtod(v.c_str(), NULL);

  return defaultValue;
}

inline std::string XmlConfig::getAttrValue(const char* path, const char* attrName, const char* defaultValue)
{  
  TiXmlNode* pNode = getNode(path);
  TiXmlElement* pElem = NULL;
  std::string v;

  if (pNode)
    pElem = pNode->ToElement();  
  
  if ( !pElem )
  {
    if (defaultValue)
      v = defaultValue;
    return v;
  }

  const char* pValue = pElem->Attribute(attrName);
  if ( pValue )
    v = pValue;
  else if ( defaultValue )
    v = defaultValue;

  return v; 
}

inline int XmlConfig::getAttrValue(const char* path, const char* attrName, int defaultValue)
{
  std::string v;
  v = getAttrValue(path, attrName, "");
  if ( !v.empty() )
    return atoi(v.c_str());

  return defaultValue;
}

inline double XmlConfig::getAttrValue(const char* path, const char* attrName, double defaultValue)
{
  std::string v;
  v = getAttrValue(path, attrName, "");
  if ( !v.empty() )
    return strtod(v.c_str(), NULL);

  return defaultValue;
}

inline bool XmlConfig::setValue(const char* path, const char* newValue)
{
  TiXmlNode* pNode = getNode(path);
  bool is_new = false;
  
  if (pNode && pNode->Type() == TiXmlNode::ELEMENT)
  {
    TiXmlNode* pContent = pNode->FirstChild();
    if ( !pContent )
    {
      //　结点内容不存在，则新建内容子结点
      pContent = pNode->InsertEndChild(TiXmlText(newValue));
      is_new = true;
    }

    pNode = pContent;
  }  

  if (!pNode || pNode->Type() != TiXmlNode::TEXT)
    return false;
  
  if ( !is_new )
    pNode->SetValue(newValue);

  return true;
}

inline bool XmlConfig::setValue(const char* path, int newValue)
{
  char buff[32] = {0};
  sprintf(buff, "%d", newValue);
  return setValue(path, buff);
}

inline bool XmlConfig::setValue(const char* path, double newValue)
{
  char buff[32] = {0};
  sprintf(buff, "%f", newValue);
  return setValue(path, buff);  
}

inline bool XmlConfig::setAttrValue(const char* path, const char* attrName, const char* newValue)
{
  TiXmlNode* pNode = getNode(path);
  if ( !pNode )
    return false; 

  TiXmlElement* pElem = pNode->ToElement();
  if ( !pElem )
    return false;

  pElem->SetAttribute(attrName, newValue);
  return true;
}

inline bool XmlConfig::setAttrValue(const char* path, const char* attrName, int newValue)
{
  char buff[32] = {0};
  sprintf(buff, "%d", newValue);
  return setAttrValue(path, attrName, buff);
}

inline bool XmlConfig::setAttrValue(const char* path, const char* attrName, double newValue)
{
  char buff[32] = {0};
  sprintf(buff, "%f", newValue);
  return setAttrValue(path, attrName, buff);  
}

///////////////////////////////////////////////////////////////////////////////


// Parse 将输入的流解析为xml格式
inline bool XmlConfig::Parse(const char * szBuf)
{	
	m_good = m_xmlDoc.Parse(szBuf);
	
	return m_good;
}

// 错误后返回错误信息
inline const char * XmlConfig::getLastError()
{
	return m_xmlDoc.ErrorDesc();
}


#endif // __XML_CONFIG_H__
