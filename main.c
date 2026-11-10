#include <stdio.h>
#include <stdlib.h>

// AVL Ağaç Düğüm Yapısı
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Yükseklik alma fonksiyonu
int getHeight(Node *n) {
    if (n == NULL)
        return 0;
    return n->height;
}

// İki değerden büyük olanı döndürür
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Yeni düğüm oluşturma
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Denge Faktörünü Hesaplanması: BF = height(left) - height(right)
int getBalanceFactor(Node *n) {
    if (n == NULL)
        return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Yükseklik güncelleme yardımcı fonksiyonu
void updateHeight(Node *n) {
    if (n != NULL) {
        n->height = 1 + max(getHeight(n->left), getHeight(n->right));
    }
}

// Sağ Rotasyon (LL durumu için)
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// Sol Rotasyon (RR durumu için)
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// SORU 1 İÇİN: Rotasyonsuz BST Ekleme (Rotasyon öncesi BF = -2 değerini görmek için)
Node* bstInsert(Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = bstInsert(node->left, key);
    else if (key > node->key)
        node->right = bstInsert(node->right, key);

    updateHeight(node);
    return node;
}

// SORU 2 İÇİN: Tam Otomatik AVL Ekleme ve Dengeleme (LL, RR, LR, RL)
Node* insert(Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    updateHeight(node);
    int balance = getBalanceFactor(node);

    // Sol Sol (LL) Durumu -> Sağ Rotasyon
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Sağ Sağ (RR) Durumu -> Sol Rotasyon
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Sol Sağ (LR) Durumu -> Önce Sol, Sonra Sağ Rotasyon
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Sağ Sol (RL) Durumu -> Önce Sağ, Sonra Sol Rotasyon
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Inorder Gezinme
void inorder(Node *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// Preorder Gezinme
void preorder(Node *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

// Ağacı bellekten temizleme
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    printf("=== SORU 1: AVL Agacina Ekleme ve Tek Rotasyon ===\n\n");

    Node *root1 = NULL;
    int elemanlar1[] = {10, 20, 30};

    // Soru 1 adımları: Dengesizlik anındaki BF değerlerini basmak için bstInsert kullanıldı
    for (int i = 0; i < 3; i++) {
        int val = elemanlar1[i];
        root1 = bstInsert(root1, val);
        printf("%d eklendi. BF(kok) = %d\n", val, getBalanceFactor(root1));
    }

    // Rotasyon manuel çağrılarak sonuc gösterilir
    printf("RR dengesizligi: Sol rotasyon\n");
    root1 = leftRotate(root1);

    printf("Kok: %d\n", root1->key);
    printf("Inorder: ");
    inorder(root1);
    printf("\nPreorder: ");
    preorder(root1);
    printf("\n\n");

    freeTree(root1);

    printf("=== SORU 2: AVL Agacinda Dort Rotasyon Durumu Testleri ===\n\n");

    int testler[4][3] = {
        {30, 20, 10}, // A (LL)
        {10, 20, 30}, // B (RR)
        {30, 10, 20}, // C (LR)
        {10, 30, 20}  // D (RL)
    };
    char testAdlari[] = {'A', 'B', 'C', 'D'};
    char* rotasyonTurleri[] = {"LL (Sag Rotasyon)", "RR (Sol Rotasyon)", "LR (Cift Rotasyon)", "RL (Cift Rotasyon)"};

    for (int t = 0; t < 4; t++) {
        Node *root2 = NULL;
        printf("Test %c (%s) - Ekleme sirasi: %d, %d, %d\n",
                testAdlari[t], rotasyonTurleri[t], testler[t][0], testler[t][1], testler[t][2]);

        for (int i = 0; i < 3; i++) {
            root2 = insert(root2, testler[t][i]);
        }

        printf("Sonuc Kok: %d\n", root2->key);
        printf("Inorder: ");
        inorder(root2);
        printf("\nPreorder: ");
        preorder(root2);
        printf("\n----------------------------------------\n");

        freeTree(root2);
    }

    return 0;
}
