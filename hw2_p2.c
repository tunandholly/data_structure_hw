#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// rbtree.c

#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#define RED        0    // 紅色節點
#define BLACK    1    // 黑色節點

typedef int Type;

// 紅黑樹的節點
typedef struct RBTreeNode {
    unsigned char color;        // 顏色(RED 或 BLACK)
    Type key;                    // 關鍵字(鍵值)
    struct RBTreeNode *left;    // 左孩子
    struct RBTreeNode *right;   // 右孩子
    struct RBTreeNode *parent;  // 父節點
} Node, *RBTree;

// 紅黑樹的根
typedef struct rb_root {
    Node *node;
} RBRoot;

// 創建红黑树，返回"红黑樹的根"！
RBRoot* create_rbtree();

// 銷毁红黑树
void destroy_rbtree(RBRoot *root);

// 將節點插入到红黑树中。插入成功，返回0；失敗返回-1。
int insert_rbtree(RBRoot *root, Type key);

// 删除節點(key為節點的值)
void delete_rbtree(RBRoot *root, Type key);


// (遞迴實現)查找"红黑樹"中键值为key的節點。找到的話，返回0；否則，返回-1。
int rbtree_search(RBRoot *root, Type key);





#endif


#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

/*
 * 創建红黑树，返回"红黑树的根"！
 */
RBRoot* create_rbtree()
{
    RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
    root->node = NULL;

    return root;
}


/*
 * (遞迴實現)查找"红黑樹x"中鍵值為key的節點
 */
static Node* search(RBTree x, Type key)
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}
int rbtree_search(RBRoot *root, Type key)
{
    if (root)
        return search(root->node, key)? 0 : -1;
}






static void rbtree_left_rotate(RBRoot *root, Node *x)
{
    // 設置x的右孩子為y
    Node *y = x->right;

    // 將 “y的左孩子” 設為 “x的右孩子”；
    // 如果y的左孩子非空，將 “x” 設為 “y的左孩子的父親”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 將 “x的父親” 設為 “y的父親”
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        //tree = y;            // 如果 “x的父親” 是空節點，則將y設為根節點
        root->node = y;            // 如果 “x的父親” 是空節點，則將y設為根節點
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父節點的左孩子，則將y設為“x的父節點的左孩子”
        else
            x->parent->right = y;    // 如果 x是它父節點的左孩子，則將y設為“x的父節點的左孩子”
    }

    // 將 “x” 設為 “y的左孩子”
    y->left = x;
    // 將 “x的父節點” 設為 “y”
    x->parent = y;
}


static void rbtree_right_rotate(RBRoot *root, Node *y)
{
    // 设置x是当前节点的左孩子。
    Node *x = y->left;

    // 將 “x的右孩子” 設為 “y的左孩子”；
    // 如果"x的右孩子"不為空的話，將 “y” 設為 “x的右孩子的父親”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 將 “y的父親” 設為 “x的父親”
    x->parent = y->parent;

    if (y->parent == NULL)
    {
        //tree = x;            // 如果 “y的父親” 是空節點，則將x設為根節點
        root->node = x;            // 如果 “y的父親” 是空節點，則將x設為根節點
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父節點的右孩子，則將x設為“y的父節點的右孩子”
        else
            y->parent->left = x;    // (y是它父節點的左孩子) 將x設為“x的父節點的左孩子”
    }

    // 將 “y” 設為 “x的右孩子”
    x->right = y;

    // 將 “y的父節點” 設為 “x”
    y->parent = x;
}

/*
 * 红黑樹插入修正函數
 *
 * 在向红黑树中插入節點之后(失去平衡)，再調用該函數；
 * 目的是將它重新塑造成一颗红黑樹。
 *
 * 参數說明：
 *     root 红黑樹的根
 *     node 插入的節點       // 對應《算法導論》中的z
 */
static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;

    // 若“父節點存在，并且父節點的顏色是紅色”
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //若“父節點”是“祖父節點的左孩子”
        if (parent == gparent->left)
        {
            // Case 1條件：叔叔節點是红色
            {
                Node *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2條件：叔叔是黑色，且當前節點是右孩子
            if (parent->right == node)
            {
                Node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3條件：叔叔是黑色，且當前節點是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        }
        else//若“z的父節點”是“z的祖父節點的右孩子”
        {
            // Case 1条件：叔叔節點是红色
            {
                Node *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且當前節點是左孩子
            if (parent->left == node)
            {
                Node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且當前節點是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root->node);
}

/*
 * 添加節點：將節點(node)插入到红黑樹中
 *
 * 参數說明：
 *     root 红黑樹的根
 *     node 插入的節點        // 對應《算法導論》中的z
 */
static void rbtree_insert(RBRoot *root, Node *node)
{
    Node *y = NULL;
    Node *x = root->node;

    // 1. 将红黑樹當作一颗二叉查找树，将節點添加到二叉查找樹中。
    while (x != NULL)
    {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    rb_parent(node) = y;

    if (y != NULL)
    {
        if (node->key < y->key)
            y->left = node;                // 情况2：若“node所包含的值” < “y所包含的值”，則將node設為“y的左孩子”
        else
            y->right = node;            // 情况3：(“node所包含的值” >= “y所包含的值”)將node設為“y的右孩子”
    }
    else
    {
        root->node = node;                // 情况1：若y是空節點，則將node設為根
    }

    // 2. 設置節點的顏色為红色
    node->color = RED;

    // 3. 將它重新修正為一颗二叉查找樹
    rbtree_insert_fixup(root, node);
}

/*
 * 創建節點
 *
 * 參數說明：
 *     key 是鍵值。
 *     parent 是父節點。
 *     left 是左孩子。
 *     right 是右孩子。
 */
static Node* create_rbtree_node(Type key, Node *parent, Node *left, Node* right)
{
    Node* p;

    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->key = key;
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = BLACK; // 默認為黑色

    return p;
}

/*
 * 新建節點(節點鍵值為key)，並将其插入到红黑樹中
 *
 * 参數說明：
 *     root 红黑樹的根
 *     key 插入節點的鍵值
 * 返回值：
 *     0，插入成功
 *     -1，插入失敗
 */
int insert_rbtree(RBRoot *root, Type key)
{
    Node *node;    // 新建節點

    // 不允许插入相同键值的节点。
    // (若想允許插入相同鍵值的節點，注釋掉下面两句話即可！)
    if (search(root->node, key) != NULL)
        return -1;

    // 如果新建節點失敗，則返回。
    if ((node=create_rbtree_node(key, NULL, NULL, NULL)) == NULL)
        return -1;

    rbtree_insert(root, node);

    return 0;
}

/*
 * 紅黑樹删除修正函數
 *
 * 在從红黑树中删除插入節點之后(紅黑樹失去平衡)，再調用該函數；
 * 目的是將它重新塑造成一颗红黑樹。
 *
 * 参數說明：
 *     root 紅黑樹的根
 *     node 待修正的節點
 */
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是紅色的
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的兩個孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x的兄弟w是黑色的，並且w的左孩子是紅色，右孩子為黑色。
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；並且w的右孩子是紅色的，左孩子任意顏色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的兩個孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子為黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意顏色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * 删除節點
 *
 * 參數說明：
 *     tree 紅黑樹的根節點
 *     node 删除的節點
 */
void rbtree_delete(RBRoot *root, Node *node)
{
    Node *child, *parent;
    int color;

    // 被删除節點的"左右孩子都不為空"的情况。
    if ( (node->left!=NULL) && (node->right!=NULL) )
    {
        // 被删節點的後繼節點。(稱為"取代節點")
        // 用它来取代"被删節點"的位置，然后再將"被删節點"去掉。
        Node *replace = node;

        // 獲取後繼節點
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node節點"不是根節點(只有根節點不存在父節點)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            // "node節點"是根節點，更新根節點。
            root->node = replace;

        // child是"取代節點"的右孩子，也是需要"調整的節點"。
        // "取代節點"肯定不存在左孩子！因為它是一个後繼節點。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代節點"的顏色
        color = rb_color(replace);

        // "被删除節點"是"它的後繼節點的父節點"
        if (parent == node)
        {
            parent = replace;
        }
        else
        {
            // child不為空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child, parent);
        free(node);

        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    // 保存"取代節點"的顏色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node節點"不是根節點
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root->node = child;

    if (color == BLACK)
        rbtree_delete_fixup(root, child, parent);
    free(node);
}

/*
 * 删除鍵值為key的節點
 *
 * 參數說明：
 *     tree 紅黑樹的根結點
 *     key 鍵值
 */
void delete_rbtree(RBRoot *root, Type key)
{
    Node *z, *node;

    if ((z = search(root->node, key)) != NULL)
        rbtree_delete(root, z);
}

/*
 * 銷毀紅黑樹
 */
static void rbtree_destroy(RBTree tree)
{
    if (tree==NULL)
        return ;

    if (tree->left != NULL)
        rbtree_destroy(tree->left);
    if (tree->right != NULL)
        rbtree_destroy(tree->right);

    free(tree);
}

void destroy_rbtree(RBRoot *root)
{
    if (root != NULL)
        rbtree_destroy(root->node);

    free(root);
}


int main() {
    RBRoot *root = create_rbtree();
    char input[20];
    Type number;

    while (1) {
        scanf("%s", input);

        if (strcmp(input, "insert") == 0) {
            scanf("%d", &number);
            insert_rbtree(root, number);
    
        } else if (strcmp(input, "search") == 0) {
            scanf("%d", &number);
            int result = rbtree_search(root, number);
            if (result == 0) {
                Node *foundNode = search(root->node, number);
                printf("%s\n", (foundNode->color == RED) ? "red" : "black");
            } else {
                printf("Not found\n");
        }

        } else if (strcmp(input, "delete") == 0) {
            scanf("%d", &number);
            delete_rbtree(root, number);

        } else if (strcmp(input, "quit") == 0) {
            break;
        
        } else {
            printf("Invalid operation. Please enter insert, search, delete, or quit.\n");
        }
    }

    destroy_rbtree(root);
    return 0;
}
