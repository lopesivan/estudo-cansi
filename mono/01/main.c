#include <stdio.h>
#include <libxml/xpath.h>

int main() {
  xmlDocPtr doc;
  xmlXPathContextPtr xpathCtx;
  xmlXPathObjectPtr xpathObj;

  // Carrega o documento XML a partir de um arquivo
  doc = xmlParseFile("exemplo.xml");

  // Cria um contexto XPath
  xpathCtx = xmlXPathNewContext(doc);

  // Executa a consulta XPath para obter os nós com a tag "nome"
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"//nome", xpathCtx);

  // Extrai o valor de cada nó encontrado
  int i;
  for (i = 0; i < xpathObj->nodesetval->nodeNr; i++) {
    xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
    xmlChar* value = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
    printf("%s\n", value);
    xmlFree(value);
  }

  // Libera memória
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);

  return 0;
}
