/********************************************************************
 * SubscribeXml.cpp
 * --------
 *
 * Author:  chenzixun0@163.com
 * Company: GXX Tech.
 * Date:    7/10/2016
 *
 * Copyright (c), 2016, GXX Tech.   
 ********************************************************************/

// -------------------------------------------------------------------
// includes
// -------------------------------------------------------------------
#include "SubscribeXml.h"
#include "common.h"

// -------------------------------------------------------------------
// definition of constructors and destructor
// -------------------------------------------------------------------
SubscribeXml::SubscribeXml()
{
	m_systemtype = "UTC";
	m_messagetype = "SUBSCRIPTION";
}

SubscribeXml::~SubscribeXml()
{
							
}

// -------------------------------------------------------------------
// definition of methods
// -------------------------------------------------------------------
string SubscribeXml::CreateXml()
{
	string xml = "<?xml version=\"1.0\" encoding=\"GB2312\"?><message System=\"ATMS\" Ver=\"1.0\"></message>";

	pugi::xml_document doc;
	if (!doc.load(xml.c_str()))                                        
	{  
		return NULL;                                                                                                                          
	}

	pugi::xml_node message = doc.child("message");

	pugi::xml_node new_node = message.append_child("systemtype");
	new_node.append_child(pugi::node_pcdata).set_value(m_systemtype.c_str());

	new_node = message.append_child("messagetype");                                                
	new_node.append_child(pugi::node_pcdata).set_value(m_messagetype.c_str());

	new_node = message.append_child("sourceIP");          
	new_node.append_child(pugi::node_pcdata).set_value(m_sourceIP.c_str());   

	new_node = message.append_child("targetIP");             
	new_node.append_child(pugi::node_pcdata).set_value(m_targetIP.c_str());

	new_node = message.append_child("user");
	new_node.append_child(pugi::node_pcdata).set_value(m_user.c_str());

	new_node = message.append_child("password");                     
	new_node.append_child(pugi::node_pcdata).set_value(m_password.c_str());

	xml_string_writer writer;                  
                                          
	doc.save(writer);
	xml = writer.result;                                                                   

	return xml;
}