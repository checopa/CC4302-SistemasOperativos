#define FALSE 0
#define TRUE 1

typedef struct Nodo {
    int valor;
    struct Nodo* izq;
    struct Nodo* der;
} Nodo;

int buscarSeq1(Nodo *node, int num) {
    if (if node == NULL) {
        return FALSE;
    }
    else if (node->valor == num) {
        return TRUE;
    }
    else {
        return buscarSeq1(node->izq, num) || buscarSeq1(node->der, num);
    }
}