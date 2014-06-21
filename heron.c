#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define isnode(x, s) (0==xmlStrcmp((x)->name, (xmlChar*)(s)))
#define isconcept(x) (0==xmlStrcmp((x)->name, (xmlChar*)"Concept"))

xmlNodePtr root;

typedef struct concept{
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

void checklang(char** dest, xmlNodePtr n)
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
	if(!isconcept(n)) return NULL;
	ret = malloc(sizeof(struct concept));
	memset((void*)ret, 0, sizeof(struct concept));
	ret->uri = (char*) xmlGetProp(n, "about");
	for(n = n->children; n !=NULL; n = n->next) {
		if(isnode(n, "broader")) ret->broaderuri = (char*)xmlGetProp(n, "resource");
		if(n->children == NULL) continue;
		if(isnode(n, "notation")) ret->notation = (char*)n->children->content;
		else if(isnode(n, "prefLabel")) checklang(&(ret->caption), n);
		else if(isnode(n, "includingNote")) checklang(&(ret->including), n);
		else if(isnode(n, "scopeNote")) checklang(&(ret->scope), n);
		else if(isnode(n, "applicationNote")) checklang(&(ret->application), n);
		else if(isnode(n, "example")) checklang(&(ret->example), n);
		//else if(isnode(n, "related")) ret->relateduri = (char*)n->children->content;
	}
	return ret;
}

void freecon(con c)
{
	if(c == NULL) return;
	xmlFree(c->broaderuri);
	free((void*)c);
}

con getfromuri(char *uri)
{
	int count=0;
	xmlNodePtr n = root;
	con cc;
	for(n = n->children; n!=NULL; n = n->next) {
		if(!isconcept(n)) continue;
		cc = xml2con(n);
		if(0==strcmp(uri, cc->uri)) return cc;
		if(cc!=NULL) freecon(cc);
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
	pai = getfromuri(c->broaderuri);
	if(pai==NULL) return;
	pairecursivo(pai);
	printf("\x1b[1m");
	resumocon(pai);
	freecon(pai);
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
	n = root;
	for(n = n->children; n!=NULL; n = n->next) {
		if(!isconcept(n)) continue;
		filho = xml2con(n);
		if(filho == NULL) continue;
		if(filho->broaderuri!=NULL) {
			if(0==strcmp(filho->broaderuri, c->uri)) {
				printf("\x1b[1mVer também: \t");
				resumocon(filho);
			}
		}
		freecon(filho);
	}
	return c;
}
#undef campo

con peganotacao(char* notation)
{
	int count=0;
	xmlNodePtr n = root;
	con cc;
	assert(n!=NULL);
	for(n = n->children; n!=NULL; n = n->next) {
		if(!isconcept(n)) continue;
		cc = xml2con(n);
		if(0==strcmp(notation, cc->notation)) return cc;
		if(cc!=NULL) freecon(cc);
	}
	return NULL;
}

void proc(xmlNodePtr n)
{
	int count=0;
	con cc;
	assert(n!=NULL);
	for(n = n->children; n!=NULL; n = n->next) {
		if(!isconcept(n)) continue;
		cc = xml2con(n);
		if(count++ == 1240) printacon(cc);
		if(cc!=NULL) freecon(cc);
	}
}

int main(int argn, char*argv[])
{
	xmlDocPtr doc;
	doc = xmlParseFile("udcsummary-skos.rdf");
	assert(doc!=NULL);
	root = xmlDocGetRootElement(doc);
	assert(root!=NULL);
	if(argn>1) {
		con res;
		int i;
		for(i=1; i<argn; i++) {
			res = peganotacao(argv[i]);
			if(res == NULL) printf("Não encontrei %s\n", argv[i]);
			else freecon(printacon(res));
			putchar('\n');
		}
		xmlFreeDoc(doc);
		return 0;
	}
	while(1) {
		char entra[80];
		con res;
		printf("\n> ");
		if(scanf("%80s", entra)==EOF) break;
		res = peganotacao(entra);
		if(res == NULL) printf("Não encontrei %s\n", entra);
		else {
			freecon(printacon(res));
		}
	}
	printf("Tchau\n");
	xmlFreeDoc(doc);
	return 0;
}
