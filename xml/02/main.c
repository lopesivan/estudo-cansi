#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main() {
  xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
  xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "root");
  xmlDocSetRootElement(doc, root);

  xmlNodePtr child1 = xmlNewChild(root, NULL, BAD_CAST "child1", NULL);
  xmlNodePtr child2 = xmlNewChild(root, NULL, BAD_CAST "child2", NULL);

  xmlSaveFormatFileEnc("exemplo.xml", doc, "UTF-8", 1);

  xmlFreeNode(child1);
  xmlFreeNode(child2);
  xmlFreeNode(root);
  xmlFreeDoc(doc);

  return 0;
}
