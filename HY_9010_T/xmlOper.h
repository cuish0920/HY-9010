#pragma once
#ifndef  __xml_oper_h
#define __xml_oper_h
#include <list>
#include <afxdisp.h>        // MFC 自动化类



typedef struct key_value
{
	CString xml_key;
	CString xml_value;

}key_value;


typedef struct X_ScalarWebAPI_ServiceList
{
	CString X_ScalarWebAPI_ServiceType;
	CString X_ScalarWebAPI_ActionList_URL;
	CString X_ScalarWebAPI_AccessType;
	int index;

}X_ScalarWebAPI_ServiceList;
using namespace std;
class xmlOPer
{
public:
	xmlOPer();
	~xmlOPer();

public:
	//xml文档操作的例子
	CComPtr<IXMLDOMElement>  openXmlDoc();
private:

	void ProcessNode(CComPtr<IXMLDOMNode>& spNode);
	void writeXmlDoc(CComPtr<IXMLDOMElement> spRootEle, CComPtr<IXMLDOMDocument> spDoc, CComPtr<IXMLDOMNode>& spNode);
	void selectNodes(CComPtr<IXMLDOMElement> spRootEle);

public:
	//工程中正真使用的
	CComPtr<IXMLDOMElement> openXmlDocFromMemory(CString xmlContent);
	CComPtr<IXMLDOMNodeList> selectNodes(CComPtr<IXMLDOMElement> spRootEle, CString node);
	CComPtr<IXMLDOMNode> selectSingleNode(CComPtr<IXMLDOMElement> spRootEle, CString node);
	key_value selectSingleNodeKeyValue(CComPtr<IXMLDOMNode> xmlOPer);
	list<X_ScalarWebAPI_ServiceList> ProcessNodeList(CComPtr<IXMLDOMNodeList>nodeList);
private:
	void ProcessNodes(CComPtr<IXMLDOMNode>& spNode, list<X_ScalarWebAPI_ServiceList> &ServiceList, X_ScalarWebAPI_ServiceList *m_ServiceList, int &index);
private:
	CComPtr<IXMLDOMDocument> spDoc;
public:
	CComPtr<IXMLDOMDocument> getDocument();

};
#endif // ! __xml_oper_h
