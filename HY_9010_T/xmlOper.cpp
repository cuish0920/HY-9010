#include "xmlOper.h"

#include <afxdisp.h> 
#include <comutil.h>
/*仅仅做为一个XML文档解析的例子*/


xmlOPer::xmlOPer()
{
	spDoc = NULL;
}

xmlOPer::~xmlOPer()
{
	CoUninitialize();
}

CComPtr<IXMLDOMElement> xmlOPer::openXmlDoc()
{
	////做为一个例子
	//CComPtr<IXMLDOMDocument> spDoc; //DOM
	//HRESULT  hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER/*CLSCTX_ALL*/, IID_IXMLDOMDocument, (LPVOID*)&spDoc);
	//if (hr != S_OK)
	//	return;
	//VARIANT_BOOL vb;
	//spDoc->load(CComVariant(OLESTR("./systemini.xml")), &vb); //加载XML文件
	//CComPtr<IXMLDOMElement> spRootEle;
	//spDoc->get_documentElement(&spRootEle); //根节点
	//CComPtr<IXMLDOMNodeList> spNodeList;
	//spRootEle->get_childNodes(&spNodeList); //子节点列表
	//long nLen;
	//spNodeList->get_length(&nLen); //子节点数
	//for (long i = 0; i != nLen; ++i) //遍历子节点
	//{
	//	CComPtr<IXMLDOMNode> spNode;
	//	spNodeList->get_item(i, &spNode);
	//	ProcessNode(spNode); //节点处理函数
	//}


	//做为一个例子
	CoInitialize(NULL);//用完后要释放CoUninitialize();
					   //CoUninitialize();
	//CComPtr<IXMLDOMDocument> spDoc; //DOM
	HRESULT  hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER/*CLSCTX_ALL*/, IID_IXMLDOMDocument, (LPVOID*)&spDoc);
	if (hr != S_OK)
		return nullptr;
	//spDoc.CoCreateInstance(CLSID_DOMDocument);
	VARIANT_BOOL vb;
	//spDoc->load(CComVariant(OLESTR("./rx10.xml")), &vb); //加载XML文件
	//BSTR xml = _bstr_t("< ? xml version = \"1.0\" encoding = \"utf - 8\" ? ><root></root>");
	hr = spDoc->load(CComVariant(OLESTR("./systemini.xml")), &vb);



	CComPtr<IXMLDOMElement> spRootEle;
	spDoc->get_documentElement(&spRootEle); //根节点


	return spRootEle;
}
void xmlOPer::ProcessNode(CComPtr<IXMLDOMNode>& spNode)
{
	CComBSTR bsNodeName;
	spNode->get_nodeName(&bsNodeName); //节点名
	AfxMessageBox(COLE2CT(bsNodeName));
	CComVariant varVal;
	spNode->get_nodeValue(&varVal); //节点值
	AfxMessageBox(COLE2CT(varVal.bstrVal));
	DOMNodeType eNodeType;
	spNode->get_nodeType(&eNodeType);
	if (eNodeType == NODE_ELEMENT) //只有NODE_ELEMENT类型才能包含有属性和子节点
	{
		//递归遍历节点属性
		CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long nLength;
		spNameNodeMap->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spNameNodeMap->get_item(i, &spNodeAttrib);
			ProcessNode(spNodeAttrib);
		}
		//递归遍历子节点
		CComPtr<IXMLDOMNodeList> spNodeList;
		spNode->get_childNodes(&spNodeList);
		spNodeList->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spChildNode;
			spNodeList->get_item(i, &spChildNode);
			ProcessNode(spChildNode);
		}
	}
}

void xmlOPer::writeXmlDoc(CComPtr<IXMLDOMElement> spRootEle, CComPtr<IXMLDOMDocument> spDoc, CComPtr<IXMLDOMNode>& spNode)
{
	//做为一个例子
	/*< ? xml version = "1.0" encoding = "utf-8" ? >
	<root>
	<node1>text1< / node1>
	<node2>
	<childnode1 attrib1 = "value1" attrib2 = "value2" / >
	<childnode2 attrib1 = "value1" attrib2 = "value2">childtext1< / childnode2>
	< / node2>
	< / root>*/

	//写入XML
	//CComPtr<IXMLDOMNode> spNode;
	spRootEle->selectSingleNode(OLESTR("/root/node1"), &spNode);
	spNode->put_text(OLESTR("newText")); //写入text
	spRootEle->selectSingleNode(OLESTR("/root/node2/childnode1/@attrib1"), &spNode);
	spNode->put_nodeValue(CComVariant(OLESTR("newValue"))); //写入value
	CComPtr<IXMLDOMNode> spNewNode;
	spDoc->createNode(CComVariant(NODE_ELEMENT), OLESTR("childnode3"), OLESTR(""), &spNewNode); //创建新节点
	spRootEle->selectSingleNode(OLESTR("/root/node2"), &spNode);
	spNode->appendChild(spNewNode, &spNewNode); //将新节点加为node2的子节点
	spNewNode->put_text(OLESTR("childtext2")); //写入新节点text
	CComQIPtr<IXMLDOMElement> spEle = spNewNode; //注意这里使用CComQIPtr
	spEle->setAttribute(OLESTR("attrib1"), CComVariant(OLESTR("value1")));//给新节点添加属性
	spDoc->save(CComVariant(OLESTR("stocks.xml")));

}
void xmlOPer::selectNodes(CComPtr<IXMLDOMElement> spRootEle)
{
	CComPtr<IXMLDOMNodeList> spNodeList;
	CComPtr<IXMLDOMNode> spNode;
	spRootEle->selectNodes(OLESTR("/root/node2/*"), &spNodeList); //得到node2下的所有子节点
	spRootEle->selectSingleNode(OLESTR("/root/node2/childnode1/@attrib1"), &spNode); //得到childnode1的attrib1属性
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



CComPtr<IXMLDOMElement> xmlOPer::openXmlDocFromMemory(CString xmlContent)
{
	//做为一个例子
	CoInitialize(NULL);//用完后要释放CoUninitialize();
	//CoUninitialize();
	CComPtr<IXMLDOMDocument> spDoc; //DOM
	HRESULT  hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER/*CLSCTX_ALL*/, IID_IXMLDOMDocument, (LPVOID*)&spDoc);
	if (hr != S_OK)
		return nullptr;
	//spDoc.CoCreateInstance(CLSID_DOMDocument);
	VARIANT_BOOL vb;
	//spDoc->load(CComVariant(OLESTR("./rx10.xml")), &vb); //加载XML文件
	//BSTR xml = _bstr_t("< ? xml version = \"1.0\" encoding = \"utf - 8\" ? ><root></root>");
	spDoc->loadXML(_bstr_t(xmlContent), &vb);



	CComPtr<IXMLDOMElement> spRootEle;
	spDoc->get_documentElement(&spRootEle); //根节点

	//CComPtr<IXMLDOMNodeList> spNodeList;
	//spRootEle->get_childNodes(&spNodeList); //子节点列表
	//long nLen;
	//spNodeList->get_length(&nLen); //子节点数
	//for (long i = 0; i != nLen; ++i) //遍历子节点
	//{
	//	CComPtr<IXMLDOMNode> spNode;
	//	spNodeList->get_item(i, &spNode);
	//	ProcessNode(spNode); //节点处理函数
	//}
	return spRootEle;
}

CComPtr<IXMLDOMNodeList> xmlOPer::selectNodes(CComPtr<IXMLDOMElement> spRootEle, CString node)
{
	CComPtr<IXMLDOMNodeList> spNodeList;

	//spRootEle->selectNodes(OLESTR("/root/node2/*"), &spNodeList); //得到node2下的所有子节点
	spRootEle->selectNodes(_bstr_t(node), &spNodeList); //得到node2下的所有子节点
	return spNodeList;
}

CComPtr<IXMLDOMNode> xmlOPer::selectSingleNode(CComPtr<IXMLDOMElement> spRootEle, CString node)
{

	CComPtr<IXMLDOMNode> spNode;
	//spRootEle->selectSingleNode(OLESTR("/root/node2/childnode1/@attrib1"), &spNode); //得到childnode1的attrib1属性
	spRootEle->selectSingleNode(_bstr_t(node), &spNode); //得到childnode1的attrib1属性
	return spNode;
}



key_value xmlOPer::selectSingleNodeKeyValue(CComPtr<IXMLDOMNode> xmlOPer)
{
	key_value m_key_value = {  };
	CComBSTR bsNodeName;
	xmlOPer->get_nodeName(&bsNodeName); //节点名
	m_key_value.xml_key = COLE2CT(bsNodeName);
	//AfxMessageBox(COLE2CT(bsNodeName));



	long nLength;						 //递归遍历子节点
	CComPtr<IXMLDOMNodeList> spNodeList;
	xmlOPer->get_childNodes(&spNodeList);
	spNodeList->get_length(&nLength);

	CComPtr<IXMLDOMNode> spChildNode;
	spNodeList->get_item(nLength - 1, &spChildNode);


	CComVariant varVal;
	spChildNode->get_nodeValue(&varVal); //节点值
	//AfxMessageBox(COLE2CT(varVal.bstrVal));
	m_key_value.xml_value = COLE2CT(varVal.bstrVal);
	return m_key_value;
}


list<X_ScalarWebAPI_ServiceList> xmlOPer::ProcessNodeList(CComPtr<IXMLDOMNodeList>nodeList)
{
	//X_ScalarWebAPI_ServiceList cc;// = new X_ScalarWebAPI_ServiceList();
	list<X_ScalarWebAPI_ServiceList> ServiceList;
	//ServiceList.push_back(cc);
	int index = 0;
	long nLen;
	nodeList->get_length(&nLen); //子节点数
	for (long i = 0; i != nLen; ++i) //遍历子节点
	{
		CComPtr<IXMLDOMNode> spNode;
		nodeList->get_item(i, &spNode);
		ProcessNodes(spNode, ServiceList, NULL, index); //节点处理函数
	}
	list<X_ScalarWebAPI_ServiceList> m_ServiceList;
	X_ScalarWebAPI_ServiceList * m_X_ScalarWebAPI_ServiceList = nullptr;
	for (int index = 0; ServiceList.size() > 0; index++)
	{
		if (index % 2 == 0)
		{
			m_X_ScalarWebAPI_ServiceList = new X_ScalarWebAPI_ServiceList();
		}
		X_ScalarWebAPI_ServiceList m_tmp = ServiceList.front();

		if ((index == 0) || (index % 2 == 0))
		{
			m_X_ScalarWebAPI_ServiceList->X_ScalarWebAPI_AccessType = m_tmp.X_ScalarWebAPI_AccessType;
		}
		else
		{
			m_X_ScalarWebAPI_ServiceList->X_ScalarWebAPI_ActionList_URL = m_tmp.X_ScalarWebAPI_AccessType;
			m_ServiceList.push_back(*m_X_ScalarWebAPI_ServiceList);
		}
		ServiceList.pop_front();

	}



	return m_ServiceList;
}


void xmlOPer::ProcessNodes(CComPtr<IXMLDOMNode>& spNode, list<X_ScalarWebAPI_ServiceList> &ServiceList, X_ScalarWebAPI_ServiceList *m_ServiceList, int &index)
{
	if (ServiceList.size() % 2 == 0)
	{
		//已经没有子节点了，应取其值
		m_ServiceList = new X_ScalarWebAPI_ServiceList();
	}

	CComBSTR bsNodeName;
	spNode->get_nodeName(&bsNodeName); //节点名
	//AfxMessageBox(COLE2CT(bsNodeName));
	//CComVariant varVal;
	//spNode->get_nodeValue(&varVal); //节点值
	//AfxMessageBox(COLE2CT(varVal.bstrVal));

	DOMNodeType eNodeType;
	spNode->get_nodeType(&eNodeType);
	if (eNodeType == NODE_ELEMENT) //只有NODE_ELEMENT类型才能包含有属性和子节点
	{
		//递归遍历节点属性
		CComPtr<IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long nLength;
		spNameNodeMap->get_length(&nLength);
		for (long i = 0; i != nLength; ++i)
		{
			CComPtr<IXMLDOMNode> spNodeAttrib; //注意属性也是一个IXMLDOMNode
			spNameNodeMap->get_item(i, &spNodeAttrib);
			ProcessNodes(spNodeAttrib, ServiceList, m_ServiceList, index);
		}
		//递归遍历子节点
		CComPtr<IXMLDOMNodeList> spNodeList;
		spNode->get_childNodes(&spNodeList);
		spNodeList->get_length(&nLength);
		if (nLength == 1)
		{

			key_value m_key_value = selectSingleNodeKeyValue(spNode);

			m_ServiceList->X_ScalarWebAPI_AccessType = m_key_value.xml_value;

			m_ServiceList->index = ServiceList.size();
			ServiceList.push_back(*m_ServiceList);
			//AfxMessageBox(m_key_value.xml_key+" "+ m_key_value.xml_value);
		}
		else
		{

			for (long i = 0; i != nLength; ++i)
			{
				CComPtr<IXMLDOMNode> spChildNode;
				spNodeList->get_item(i, &spChildNode);
				//ProcessNodes(spChildNode);
				ProcessNodes(spChildNode, ServiceList, m_ServiceList, index);
			}
		}


	}
}
CComPtr<IXMLDOMDocument> xmlOPer::getDocument()
{
	if (spDoc != NULL)
		return spDoc;
}