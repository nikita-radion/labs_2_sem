#include <stdio.h>
#include <stdlib.h>

struct Node{
    float data;
    struct Node *right;
    struct Node *left;
};

struct Node* Add(struct Node* root, float x)
{
    if(root==NULL)
    {
        root=(struct Node *)malloc(sizeof(struct Node));
        root->data=x;
        root->left=NULL;
        root->right=NULL;
        return root;
    }
    if(x>root->data)
        if(root->right!=NULL)
            Add(root->right,x);
        else
        {
            root->right=(struct Node*)malloc(sizeof(struct Node));
            root->right->data=x;
            root->right->left=NULL;
            root->right->right=NULL;
        }
    else if(root->left!=NULL)
            Add(root->left,x);
        else
        {
            root->left=(struct Node*)malloc(sizeof(struct Node));
            root->left->data=x;
            root->left->left=NULL;
            root->left->right=NULL;
        }
    return root;
}

#define N 1000
void print(struct Node *root)
{
    struct Node *buf[N];
    int head=0, tail=1;
    int width=1;
    int f=0;
    int wc=0;
    buf[0]=root;
    for(;; head=(head+1)%N, wc++)
    {
        if(wc==width)
        {
            if(!f)break;
            f=0;
            printf("\n");
            wc=0;
            width*=2;
        }
        if(buf[head]==NULL)
            printf("_ ");
        else
            printf("%f ",buf[head]->data);
        if(buf[head]==NULL)
        {
            buf[tail]=NULL;
            tail=(tail+1)%N;
            buf[tail]=NULL;
            tail=(tail+1)%N;
        }
        else
        {
            if(f==0)
                f=buf[head]->left!=NULL || buf[head]->right!=NULL;
            buf[tail]=buf[head]->left;
            tail=(tail+1)%N;
            buf[tail]=buf[head]->right;
            tail=(tail+1)%N;
        }
    }
    printf("\n");
}

int similarity(struct Node *root1, struct Node *root2)
{
    if(root1->left==NULL && root2->right!=NULL) return 0;
    if(root1->left!=NULL && root2->right==NULL) return 0;
    if(root1->right==NULL && root2->left!=NULL) return 0;
    if(root1->right!=NULL && root2->left==NULL) return 0;
    int s=(root1->left==NULL) + (root1->right==NULL);

    if(root1->left!=NULL)
        s+=similarity(root1->left,root2->right);
    if(root1->right!=NULL)
        s+=similarity(root1->right,root2->left);
    return (int)(s==2);
}

int similarityStart(struct Node *root)
{
    if(root==NULL) return 1;
    if(root->left==NULL && root->right==NULL) return 1;
    if(root->left==NULL || root->right==NULL) return 0;
    return similarity(root->left, root->right);
}

struct Node* NodeSearch(struct Node* root, float x)
{
    if(root==NULL) return NULL;
    if(root->data==x) return root;
    struct Node* tl=NodeSearch(root->left, x);
    struct Node* tr=NodeSearch(root->right, x);
    return (tl!=NULL)?tl:tr;
}

struct Node* NodePreSearch(struct Node* root, float x)
{
    if(root==NULL) return NULL;
    if((root->left!=NULL && root->left->data==x) || (root->right!=NULL && root->right->data==x)) return root;
    struct Node* tl=NodePreSearch(root->left, x);
    struct Node* tr=NodePreSearch(root->right, x);
    return (tl!=NULL)?tl:tr;
}

void shift(struct Node* root)
{
    if(root->right!=NULL)
    {
        root->data=root->right->data;
        if(root->right->left!=NULL || root->right->right!=NULL)
            shift(root->right);
        else
            {
                free(root->right);
                root->right=NULL;
            }
    }
    else if(root->left!=NULL)
    {
        root->data=root->left->data;
        if(root->left->left!=NULL || root->left->right!=NULL)
            shift(root->left);
        else
            {
                free(root->left);
                root->left=NULL;
            }
    }
}

struct Node* Erase(struct Node* root, float x)
{
    if(root==NULL) return root;
    if(root->data==x && root->left==NULL && root->right==NULL)
    {
        free(root);
        return NULL;
    }
    struct Node* uk=NodeSearch(root,x);
    if(uk==NULL)
    {
        printf("Data not found on tree\n");
        return root;
    }
    if(uk->left==NULL && uk->right==NULL)
    {
        uk=NodePreSearch(root,x);
        if(uk->left!=NULL && uk->left->data==x)
        {
            free(uk->left);
            uk->left=NULL;
        }
        else
        {
            free(uk->right);
            uk->right=NULL;
        }
    }
    else
    {
        shift(uk);
    }
    return root;
}

void clearTree(struct Node* root) {
    if (root != NULL) {
        clearTree(root->left);
        clearTree(root->right);
        free(root);
    }
}

int main()
{

    struct Node * root=NULL;

    char command;
    int data;
    Erase(NULL,0);
    do {
        printf("Enter the comand\n(C (CREATE) for add node,\n d (DEL) for delete node,\nV (VIEW) for watching tree,\nS for cheking tree on simetry, Q (QUIT) for exit): ");
        scanf(" %c", &command);

        switch(command) {

            case 'C':
            case 'c':
                printf("Print data of new node: ");
                scanf("%d", &data);
                root = Add(root, data);
            break;

            case 'D':
            case 'd':
                printf("Print data of node: ");
                scanf("%d", &data);
                root = Erase(root, data);
                break;
            case 'V':
            case 'v':
                print(root);
            break;

            case 'S':
            case 's':
                printf("\nsimilarity: %s\n", similarityStart(root)==1?"Yes":"No");

            break;

        }
    } while (command != 'Q' && command != 'q');
    clearTree(root);
    return 0;
}
