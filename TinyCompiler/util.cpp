#include "globals.h"
#include "util.h"

/*
��ӡһ���Ǻ�
������һ���Ǻŵ����ͣ�token ��һ���ǺŵĴ�ֵ tokenString
*/
void printToken(TokenType token, const char* tokenString){
	switch (token){
	case IF:
	case THEN:
	case ELSE:
	case END:
	case REPEAT:
	case UNTIL:
	case READ:
	case WRITE:
		fprintf(listing, "reserved word:%s\n", tokenString);
		break;

	case ASSIGN:fprintf(listing, ":=\n");	break;
	case LT:fprintf(listing, "<\n");		break;
	case EQ:fprintf(listing, "=\n");		break;
	case LPAREN:fprintf(listing, "(\n");	break;
	case RPAREN:fprintf(listing, ")\n");	break;
	case SEMI:fprintf(listing, ";\n");		break;
	case PLUS:fprintf(listing, "+\n");		break;
	case MINUS:fprintf(listing, "-\n");		break;
	case TIMES:fprintf(listing, "*\n");		break;
	case OVER:fprintf(listing, "/\n");		break;
	case ENDFILE:fprintf(listing, "EOF\n");	break;

	case NUM:
		fprintf(listing, "NUM,val=%s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID,name=%s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "ERROR:%s\n", tokenString);
		break;
	default:
		fprintf(listing, "Unknow token:%d\n", token);
	}
}

char* copyString(char* s){
	int n;
	char* t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL){
		fprintf(listing, "out of memory error at line %d\n", lineno);
	}
	else{
		strcpy(t, s);
	}
	return t;
}

/*����һ���µĽڵ㣬����ڵ��������������ͣ� ��Ϊ���࣬if, write, raed, repeat, assign
����ʽ�����֣��������������ʶ������ʽ
*/
TreeNode* newStmtNode(StmtKind kind){
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL){
		fprintf(listing, "out of memery error at line %d\n", lineno);
	}
	else{
		for (i = 0; i < MAXCHILDREN; i++){
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/*����һ������ʽ�ڵ㣬����������*/
TreeNode* newExpNode(ExpKind kind){
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL){
		fprintf(listing, "out of memery error at line %d\n", lineno);
	}
	else{
		for (i = 0; i < MAXCHILDREN; i++){
			t->child[i] = NULL;
			t->sibling = NULL;
			t->nodekind = ExpK;
			t->kind.exp = kind;
			t->lineno = lineno;
			t->type = Void;
		}
	}
	return t;
}

/*ʹ�ñ��� indentno ���洢printTree��ǰ����������*/
static int indentno = 0;
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/*��ӡ����*/
static void printSpaces(void){
	int i;
	for (i = 0; i < indentno; i++){
		fprintf(listing, " ");
	}
}

/*��ӡ�﷨��:������Ĵ�ӡ�кܶ�ֵ��ѧϰ�ĵط�
�ȴ�ӡ���ĸ��ڵ㣬Ȼ��������ӣ�Ȼ������һ���������
*/
void printTree(TreeNode* tree){
	int i;
	INDENT;
	while (tree != NULL){
		printSpaces();
		//����ڵ���������䣬˵����������
		if (tree->nodekind == StmtK){
			switch (tree->kind.stmt){
			case IfK:
				fprintf(listing, "If\n");
				break;
			case RepeatK:
				fprintf(listing, "Repeat\n");
				break;
			case AssignK:
				fprintf(listing, "Assign to: %s\n",tree->attr.name);
				break;
			case ReadK:
				fprintf(listing, "Read: %s\n",tree->attr.name);
				break;
			case WriteK:
				fprintf(listing, "Write\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if(tree->nodekind==ExpK){//�ڵ������Ǳ���ʽ����
			switch (tree->kind.exp){
			case OpK:
				fprintf(listing, "Op:");
				printToken(tree->attr.op, "\0");//���üǺŴ�ӡ��������ӡ���������
				break;
			case ConstK:
				fprintf(listing, "const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unkonwn ExpNode kind\n");
				break;
			}
		}
		else{
			fprintf(listing, "Unknown node kind\n");
		}
		//�����Լ��ĺ���
		for (i = 0; i < MAXCHILDREN; i++){
			printTree(tree->child[i]);
		}
		//�����Լ����ֵ�
		tree = tree->sibling;
	}
	//��������
	UNINDENT;
}