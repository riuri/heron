#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define eno(x, s) (0==xmlStrcmp((x)->name, (xmlChar*)(s)))
#define econceito(x) (0==xmlStrcmp((x)->name, (xmlChar*)"Concept"))

xmlNodePtr raiz;

typedef struct conceito{
	char* notation;
	char* uri;
	char* broaderuri;
	//xmlNodePtr broader;
	char *caption;
	char *including;
	char *application;
	char *scope;
	char *example;
	//char *relateduri;
	//xmlNodePtr *related;
} *con;

void pegalang(char** dest, xmlNodePtr n)
{
	xmlChar *lang;
	lang = xmlGetProp(n, "lang");
	if(xmlStrcmp(lang, (xmlChar*)"pt")==0) *dest = n->children->content;
	else if(xmlStrcmp(lang, (xmlChar*)"en")==0 && *dest==NULL) *dest = n->children->content;
	xmlFree(lang);
}

con xml2con(xmlNodePtr n)
{
	con ret;
	if(!econceito(n)) return NULL;
	ret = malloc(sizeof(struct conceito));
	memset((void*)ret, 0, sizeof(struct conceito));
	ret->uri = (char*) xmlGetProp(n, "about");
	for(n = n->children; n !=NULL; n = n->next) {
		if(eno(n, "broader")) ret->broaderuri = (char*)xmlGetProp(n, "resource");
		if(n->children == NULL) continue;
		if(eno(n, "notation")) ret->notation = (char*)n->children->content;
		else if(eno(n, "prefLabel")) pegalang(&(ret->caption), n);
		else if(eno(n, "includingNote")) pegalang(&(ret->including), n);
		else if(eno(n, "scopeNote")) pegalang(&(ret->scope), n);
		else if(eno(n, "applicationNote")) pegalang(&(ret->application), n);
		else if(eno(n, "example")) pegalang(&(ret->example), n);
		//else if(eno(n, "related")) ret->relateduri = (char*)n->children->content;
	}
	return ret;
}

void liberacon(con c)
{
	if(c == NULL) return;
	xmlFree(c->broaderuri);
	free((void*)c);
}

con pegauri(char *uri)
{
	int count=0;
	xmlNodePtr n = raiz;
	con cc;
	for(n = n->children; n!=NULL; n = n->next) {
		if(!econceito(n)) continue;
		cc = xml2con(n);
		if(0==strcmp(uri, cc->uri)) return cc;
		if(cc!=NULL) liberacon(cc);
	}
}

void resumocon(con c)
{
	assert(c!=NULL);
	printf("\x1b[32m%s\x1b[0m %s\n", c->notation, c->caption);
}
void pairecursivo(con c)
{
	con pai;
	if(c->broaderuri==NULL) return;
	pai = pegauri(c->broaderuri);
	if(pai==NULL) return;
	pairecursivo(pai);
	printf("\x1b[1m");
	resumocon(pai);
	liberacon(pai);
}
#define campo(s,ca) if(c->ca) printf("\x1b[1m" s ":\x1b[0m \t%s\n", c->ca)
con printacon(con c)
{
	xmlNodePtr n;
	con filho;
	if(c==NULL) return;
	pairecursivo(c);
	printf("\x1b[1mNotação: \t\x1b[32m%s\x1b[0m\n\x1b[1mURI:\x1b[0m \t\t%s\n", c->notation, c->uri);
	//campo("Broader",broaderuri);
	if(c->caption) printf("\x1b[1mDescrição:\x1b[33m \t%s\x1b[0m\n", c->caption);
	campo("Incluindo", including);
	campo("Aplicação", application);
	campo("Escopo", scope);
	campo("Exemplo", example);
	//campo("Veja também", relateduri);
	n = raiz;
	for(n = n->children; n!=NULL; n = n->next) {
		if(!econceito(n)) continue;
		filho = xml2con(n);
		if(filho == NULL) continue;
		if(filho->broaderuri!=NULL) {
			if(0==strcmp(filho->broaderuri, c->uri)) {
				printf("\x1b[1mVer também: \t");
				resumocon(filho);
			}
		}
		liberacon(filho);
	}
	return c;
}
#undef campo

con peganotacao(char* notation)
{
	int count=0;
	xmlNodePtr n = raiz;
	con cc;
	assert(n!=NULL);
	for(n = n->children; n!=NULL; n = n->next) {
		if(!econceito(n)) continue;
		cc = xml2con(n);
		if(0==strcmp(notation, cc->notation)) return cc;
		if(cc!=NULL) liberacon(cc);
	}
	return NULL;
}

void proc(xmlNodePtr n)
{
	int count=0;
	con cc;
	assert(n!=NULL);
	for(n = n->children; n!=NULL; n = n->next) {
		if(!econceito(n)) continue;
		cc = xml2con(n);
		if(count++ == 1240) printacon(cc);
		if(cc!=NULL) liberacon(cc);
	}
}

int main()
{
	xmlDocPtr doc;
	doc = xmlParseFile("udcsummary-skos.rdf");
	assert(doc!=NULL);
	raiz = xmlDocGetRootElement(doc);
//	proc(raiz);
	while(1) {
		char entra[80];
		con res;
		printf("\n> ");
		if(scanf("%80s", entra)==EOF) break;
		res = peganotacao(entra);
		if(res == NULL) printf("Não encontrei %s\n", entra);
		else {
			liberacon(printacon(res));
		}
	}
	printf("Tchau\n");
	xmlFreeDoc(doc);
	return 0;
}
